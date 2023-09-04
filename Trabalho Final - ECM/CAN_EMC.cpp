//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
// Residência Tecnológica em Software Automotivo      			  
// Módulo: SWT1 - RTOS e Sistemas Distribuídos 
//////////////////////////////////////////////////

//////////////////////////////////////////////////
/*
ESTA ECU DEVERA FAZER AS SEGUINTES FUNCOES:
RECEBER ROTACAO DO MOTOR. NESTE CASO, PROVENIENTE DA SERIAL.
RECEBER MENSAGEM (M1) CONTENDO A MARCHA ATUAL
		----M1 EH ENVIADA A PARTIR DA ECU TCM.
ENVIAR MSG CAN (M2) COM A ROTACAO DO MOTOR.
		----M2 EH ENVIADA PARA A ICM.
ENVIAR MSG CAN (M3) COM A VELOCIDADE CALCULADA (CALCULADA NESTA MESMA ECU) APOS CALCULAR SUA VELOCIDADE
		----M3 EH ENVIADA PARA A ICM.
		----CALCULAR VELOCIDADE DO VEICULO ANTES DE ENCAPSULAR E ENVIAR A MENSAGEM CAN.


*/
//////////////////////////////////////////////////




#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"


//Macro para definicoes
#define CAN_ID_M1			0x18F00503
#define CAN_ID_M2 			0x0CF00400
#define CAN_ID_M3 			0x18FEF100  
#define mEEC1_ID			0x0CF00400
#define mEEC1_DLC			8
#define mEEC1_EXT_FRAME		1

//Variavel que armazena o FRAME_DATA
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char mDATA[8];
unsigned char mDLC = 0;
long unsigned int mID;

float velocidadeFLOAT=0X00;


byte rotacaoH = 0x00;
byte rotacaoL = 0x00;
byte marchaAtual =0x01;

//Constroi um objeto MCP_CAN e configura o chip selector para o pino 10.
MCP_CAN CAN1(10);  //A própria biblioteca já define este pino (parametro) como output.

void setup()
{
	pinMode(2, INPUT); 
	//Inicializa a interface serial: baudrate = 115200
	Serial.begin(115200);
	
	//Inicializa o controlador can : baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
    {         
        delay(200);        
    }
	//Serial.println("MCP2515 can_send inicializado com sucesso!");
	//Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
}



//---------------------------------ENVIAR M2
TASK(SendCANM2)
{
	static byte ret = 0;
	
	unsigned int rotacaoFullM2 = 0x00;
	GetResource(res1);

	rotacaoFullM2 = ((rotacaoH<<8) | (rotacaoL))*8; //Encapsulando: rotacao * (1/ res) ===> rotacao * (1/0.125);

	mEEC1_data[3]= rotacaoFullM2;
	mEEC1_data[4]= rotacaoFullM2>>8;
	//mEEC1_data[3]= rotacaoL;
	//mEEC1_data[4]= rotacaoH;
	ReleaseResource(res1);

	//Envia mensagem para o barramento
	ret=CAN1.sendMsgBuf(CAN_ID_M2,CAN_EXTID,mEEC1_DLC,mEEC1_data);


	if (ret==CAN_OK)
	{
		Serial.print("CAN M2 transmitida com sucesso. Valor encapsulado: "); 
		Serial.println(rotacaoFullM2); 
		
	}
		/*
	else if (ret == CAN_SENDMSGTIMEOUT)
	{
		Serial.println("can_send: Message timeout!");      
	}
	else 
	{    
      Serial.println("can_send: Error to send!");      
	}
	*/
	TerminateTask();  
}

//---------------------------------ENVIAR M3---------------------------
TASK(CalculateVelocityCANM3)
{
	static byte ret = 0;
	float taxaMarcha =0;
	unsigned int velocidade =0;
	unsigned int rotacaoINT =0;

	GetResource(res1);

	rotacaoINT = (rotacaoH<<8) | (rotacaoL);
//------------------------
	if(marchaAtual==1)	taxaMarcha=3.83;
	else
		if(marchaAtual==2)	taxaMarcha=2.36;
		else
			if(marchaAtual==3) taxaMarcha=1.69;
			else
				if(marchaAtual==4) taxaMarcha=1.31;
				else
					if(marchaAtual==5) taxaMarcha=1.00;
					else
						if(marchaAtual==0) taxaMarcha=1;
//----------------------

	//velocidade = (0.326* ((rotacaoH<<8) | (rotacaoL)) ) VER SE O BIT SHIFTING ESTA CORRETO, CREIO QUE NAO
	///////////VelocidadeAtual = (Raio_Pneu * RotacaoAtual)/(Taxa_Trans_D * 3.55);

	//velocidade = (rotacaoH<<8) | (rotacaoL);
	//velocidade *= (0.326);
	//velocidade = velocidade /(taxaMarcha*3.55);
	velocidadeFLOAT = (rotacaoINT*0.326)/(taxaMarcha*3.55);
	if(velocidadeFLOAT>254) velocidadeFLOAT = 254;
	velocidade=velocidadeFLOAT*256; //Transformando de float para inteiro para em seguida, separar os bytes.

	//if(velocidade>254) velocidade = 254;

	
	mEEC1_data[1] = velocidade;
	mEEC1_data[2] = velocidade>>8;
	
	ReleaseResource(res1);
	//Envia mensagem para o barramento
	ret=CAN1.sendMsgBuf(CAN_ID_M3,CAN_EXTID,mEEC1_DLC,mEEC1_data);
	
	if (ret==CAN_OK)
	{
		Serial.print("Valor da velocidade enviada: "); 
		Serial.println(velocidade); 
		Serial.print("Valor real: "); 
		Serial.println(velocidade/256);
	}
	/*
	else if (ret == CAN_SENDMSGTIMEOUT)
	{
		Serial.println("can_send: Message timeout!");      
	}
	else 
	{    
      Serial.println("can_send: Error to send!");      
	}	  
		*/

	TerminateTask();
}

//----------------------------------RECEBER M1------------------------------
TASK(ReceiveCANM1)
{
	 //Se uma interrupção ocorreu interrupção (CAN_INT pino = 0), lê o buffer de recepção
	if(!digitalRead(2))                        
	{
		unsigned int var = 0;
		
		//Lê os dados: mID = identificador, mDLC = comprimento, mDATA = dados do freame
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);

		GetResource(res1);
		var = (rotacaoH<<8) | (rotacaoL);
		if((mID & CAN_ID_M1)==CAN_ID_M1) //Verificando se é a msg M1
		{
			marchaAtual = mDATA[3] - 125;
		}		
		


		if(marchaAtual == 0 && var>8000)
		{
			var= 8000;
		}
		else if(marchaAtual == 1 && var>4000)
			{
				var= 4000;
			}
			else if(marchaAtual == 2 && var>4800)
				{
					var = 4800;
				}
				else if(marchaAtual == 3 && var>5600)
					{
						var = 5600;
					}
					else if(marchaAtual == 4 && var>6400)
						{
							var = 6400;
						}
						else if(marchaAtual == 5 && var>7200)
							{
								var=7200;
							}

		rotacaoL=var; //0xBB
		rotacaoH=var>>8; //0xAA
	ReleaseResource(res1);


	}
	


	TerminateTask();
}

//----------------------------------RECEBER SERIAL ROTACAO------------------------------
TASK(EngRotTask)
{
		if (Serial.available() > 0) //Verifica o buffer da serial.
	{
		//FALTA CONSERTAR, O RESULTADO EH UM FLOAT E POSSUI 4 BYTES
		unsigned int var;
		var = Serial.parseInt(); //Lê a serial. Como o intervalo da marcha é suficiente para um byte, não há perdas na "conversão" para byte
		//var = var*8; //A mesma coisa de var * 8. 
		// 0xAABB   
		GetResource(res1);
		if(marchaAtual == 0 && var>8000)
		{
			var= 8000;
		}
		else if(marchaAtual == 1 && var>4000)
			{
				var= 4000;
			}
			else if(marchaAtual == 2 && var>4800)
				{
					var = 4800;
				}
				else if(marchaAtual == 3 && var>5600)
					{
						var = 5600;
					}
					else if(marchaAtual == 4 && var>6400)
						{
							var = 6400;
						}
						else if(marchaAtual == 5 && var>7200)
							{
								var=7200;
							}

		rotacaoL=var; //0xBB
		rotacaoH=var>>8; //0xAA
		ReleaseResource(res1);
		Serial.read(); //Limpa dados indesejados da serial, tal como o new line ou outros caracteres que não faziam parte da informacao original
		
	}

	TerminateTask();
}

/*
void loop()
{
	if (Serial.available() > 0) //Verifica o buffer da serial.
	{
		//FALTA CONSERTAR, O RESULTADO EH UM FLOAT E POSSUI 4 BYTES
		unsigned int var;
		var = Serial.read(); //Lê a serial. Como o intervalo da marcha é suficiente para um byte, não há perdas na "conversão" para byte
		//var = var*8; //A mesma coisa de var * 8.
		
		rotacaoL=var;
		rotacaoH=var>>8;

		Serial.read(); //Limpa dados indesejados da serial, tal como o new line ou outros caracteres que não faziam parte da informacao original
		
	}
}
*/