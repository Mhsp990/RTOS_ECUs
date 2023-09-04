////////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
// Residência Tecnológica em Software Automotivo      			  
// Módulo: SWT1 - RTOS e Sistemas Distribuídos 
//////////////////////////////////////////////////

//ESTA VERSAO OBSERVA UTILIZA O "LOOP()" PARA OBSERVAR SEMPRE O BARRAMENTO CAN.
//POREM, UTILIZA A TASK PARA IMPRIMIR O SEU VALOR. OU SEJA, SEMPRE ATUALIZA OS VALORES CONFORME CHEGAM, MAS TEM UM PERIODO PARA IMPRIMIR O VALOR.

//Testar esse define abaixo.
#define CAN_ID_M1 0x18F00503
#define CAN_ID_M2 0x0CF00400
#define CAN_ID_M3 0x18FEF100   


#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"


//Variavel para armazenar informacoes do frame recebido
unsigned char 		mDLC = 0;
unsigned char 		mDATA[8];
long unsigned int 	mID;

byte rotacaoH = 0x00; //Rotacao utiliza dois bytes para armazenar. Portanto, separou-se em dois bytes (high e low).
byte rotacaoL = 0x00;



byte velocidadeCalculadaH = 0x00; //Velocidade calculada utiliza dois bytes.
byte velocidadeCalculadaL = 0x00;

unsigned int marchaAtual = 0x00;





//char msgString[128]; 

//Constroi um objeto MCP_CAN e configura o chip selector para o pino 10.
MCP_CAN CAN1(10); 

void setup()
{	
	//Inicializa a interface serial: baudrate = 115200
	Serial.begin(115200);
	
	//Inicializa o controlador can : baudrate = 500K, clock=08MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
    {         
        delay(200);        
    }	
	//Serial.println("Modulo CAN inicializado!");
	//Configura o controlador MCP2515 no modo normal.
	CAN1.setMode(MCP_NORMAL);  
	//Configura o pino de interrupção para recepção	
	pinMode(2, INPUT);   
	//Serial.println("ECU ICM INITIALIZING");

	//Serial.print("ID\t\tType\tDLC\tByte0\tByte1\tByte2");
	//Serial.println("\tByte3\tByte4\tByte5\tByte6\tByte7");
}
//////////////////////////////////////////////////////////////////////////////////				 					   
//  Periodo de execucao da task: 450 ms								   
//////////////////////////////////////////////////////////////////////////////////

//--------------------------SHOW DATA TASK---------------------------------
TASK(ShowDataTask)
{
unsigned int RotacaoFull = 0;
unsigned int VelocidadeFull = 0;
//unsigned int MarchaFull = 0;

GetResource(res1);
//Juntando os bytes e "decodificando" a rotacao
RotacaoFull = (rotacaoH<<8) | (rotacaoL); //Juntando os dois Bytes para formar um inteiro apenas, no qual serão executadas as operações.
RotacaoFull = RotacaoFull*0.125; //Desfazendo encapsulamento

//Juntando os bytes e "decodificando" a velocidade
//VelocidadeAtual = (Raio_Pneu * RotacaoAtual)/(Taxa_Trans_D * 3.83);
VelocidadeFull = (velocidadeCalculadaH<<8) | (velocidadeCalculadaL); //0xAA e 0XBB   0XAABB
VelocidadeFull = VelocidadeFull/256; //Desfazendo o encapsulamento

ReleaseResource(res1);

//Serial.println("-------------");
//Serial.println("Show DATA ICM:");

Serial.print("Velocity:"); 
Serial.print(VelocidadeFull);
Serial.print(","); 

Serial.print("Rotation:");
Serial.print(RotacaoFull);
Serial.print(","); 

Serial.print("Marcha Atual:");
Serial.println(marchaAtual-125);
//Serial.print(","); 

//Serial.print("Referencia:"); //Valor constante apenas para o grafico.
//Serial.println(10);
//Serial.println(","); 

//Serial.println("-------------");

TerminateTask();
	
}


//-----------------------TASK DE RECEBER M1--------------------
TASK(ReceiveCANM1)
{

CAN1.readMsgBuf(&mID, &mDLC, mDATA);
GetResource(res1);
if((mID & CAN_ID_M1) == CAN_ID_M1)
{
	marchaAtual = mDATA[3];
}
ReleaseResource(res1);

TerminateTask();
}

//-----------------------TASK DE RECEBER M2--------------------
TASK(ReceiveCANM2)
{

CAN1.readMsgBuf(&mID, &mDLC, mDATA);
GetResource(res1);
if((mID & CAN_ID_M2) == CAN_ID_M2) //MENSAGEM M2: Rotacao do motor
{
	rotacaoL = mDATA[3];
	rotacaoH = mDATA[4];
}
ReleaseResource(res1);
TerminateTask();
}


//-----------------------TASK DE RECEBER M3--------------------
TASK(ReceiveCANM3)
{

CAN1.readMsgBuf(&mID, &mDLC, mDATA);
GetResource(res1);
if((mID & CAN_ID_M3) == CAN_ID_M3) //MENSAGEM M3
{
	velocidadeCalculadaL = mDATA[1];
	velocidadeCalculadaH = mDATA[2];
}
ReleaseResource(res1);

TerminateTask();
}

/*
void loop()
{

	if(!digitalRead(2))                        
	{
		CAN1.readMsgBuf(&mID, &mDLC, mDATA);
		
		if( (mID & CAN_ID_M1) == CAN_ID_M1) //Verifica se o identificador da mensagem é a mensagem M1.	
			{
				marchaAtual = mDATA[3]; //Marcha atual esta no byte 4 (índice 3) do campo byte. Atribui este valor para a variavel.
				
			}
			else
			{
				if((mID & CAN_ID_M2) == CAN_ID_M2) //MENSAGEM M2: Rotacao do motor
				{
					rotacaoL = mDATA[3];
					rotacaoH = mDATA[4];
				}
				else
				{
					if((mID & CAN_ID_M3) == CAN_ID_M3) //MENSAGEM M3
					{
						velocidadeCalculadaL = mDATA[1];
						velocidadeCalculadaH = mDATA[2];
					}
				}
			}

	}//FIM IF digital read

}
*/
