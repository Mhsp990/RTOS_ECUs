//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
// Residência Tecnológica em Software Automotivo      			  
// Módulo: SWT1 - RTOS e Sistemas Distribuídos 
//////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"


//Macro para definicoes
#define CAN_ID_M1			0x18F00503
//#define mEEC1_ID			0x0CF00400
#define mEEC1_DLC			8
#define mEEC1_EXT_FRAME		1

//Variavel que armazena o FRAME_DATA
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
byte marchaAtual = 0x01;

//Constroi um objeto MCP_CAN e configura o chip selector para o pino 10.
MCP_CAN CAN1(10);  //A própria biblioteca já define este pino como output.

void setup()
{
	//Inicializa a interface serial: baudrate = 115200
	Serial.begin(115200);
	
	//Inicializa o controlador can : baudrate = 250K, clock=8MHz
	while(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) 
    {         
        delay(200);        
    }
	Serial.println("MCP2515 can_send inicializado com sucesso!");
	//Modifica para o modo normal de operação
	CAN1.setMode(MCP_NORMAL);
}


TASK(SendCANM1)
{
	static byte ret = 0;
	
	//Envia mensagem para o barramento, nesse caso, envia a marcha atual (1 byte, na posicao 4 (indice 3) )
	GetResource(res1);
	mEEC1_data[3]= marchaAtual +125; //125 DEVIDO AO OFFSET
	ReleaseResource(res1);

	ret=CAN1.sendMsgBuf(CAN_ID_M1, CAN_EXTID, mEEC1_DLC, mEEC1_data);
	if (ret==CAN_OK)
	{
		Serial.println("can_send M1: mensagem transmitida com sucesso"); 
	}
	else if (ret == CAN_SENDMSGTIMEOUT)
	{
		Serial.println("can_send M1: Message timeout!");      
	}
	else 
	{    
      Serial.println("can_send M1: Error to send!");      
	}	   
	
	TerminateTask();
}


//TASK DE ENVIAR MSG M1 CONTENDO A MARCHA ATUAL
TASK(ReceberMarchaAtual)
{
	GetResource(res1);


	if (Serial.available() > 0) //Verifica o buffer da serial.
	{
		unsigned int var;
		var = Serial.parseInt(); //Lê a serial. Como o intervalo da marcha é suficiente para um byte, não há perdas na "conversão" para byte
		if(var >5)
			marchaAtual=5;
		/*else
			if(var ==0) var =1;
		*/

		marchaAtual=var;
		ReleaseResource(res1);
		Serial.read(); //Limpa dados indesejados da serial, tal como o new line ou outros caracteres que não faziam parte da informacao original
		Serial.print("Valor da marcha recebido pela serial: ");
		Serial.println(marchaAtual);
		
		
	}
	TerminateTask();
  
}