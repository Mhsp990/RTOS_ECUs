//////////////////////////////////////////////////
// Universidade Federal de Pernambuco - UFPE		  
// Residência Tecnológica em Software Automotivo      			  
// Módulo: SWT1 - RTOS e Sistemas Distribuídos 
//////////////////////////////////////////////////

#include "tpl_os.h"
#include "Arduino.h"
//#include "board.h"


//Macro para definicoes
#define mEEC1_ID			0x0CF00400
#define mEEC1_DLC			8
#define mEEC1_EXT_FRAME		1

//Variavel que armazena o FRAME_DATA
unsigned char mEEC1_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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
//////////////////////////////////////////////////////////////////////////////////
//	Task SendCanFrame   			    									   
//  Utilizada enviar as mensagens CAN 					 					   
//  Taxa de transmissao e repeticao: 1024ms									   
//////////////////////////////////////////////////////////////////////////////////
TASK(SendCanFrame)
{
	static byte ret = 0;
	
	//Envia mensagem para o barramento
	ret=CAN1.sendMsgBuf(mEEC1_ID,CAN_EXTID,mEEC1_DLC,mEEC1_data);
	if (ret==CAN_OK)
	{
		Serial.println("can_send: mensagem transmitida com sucesso"); 
	}
	else if (ret == CAN_SENDMSGTIMEOUT)
	{
		Serial.println("can_send: Message timeout!");      
	}
	else 
	{    
      Serial.println("can_send: Error to send!");      
	}	   
}