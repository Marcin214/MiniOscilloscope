#include <LPC21xx.H>
#include "spi.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"
#include "mcp4921.h" 

#define MAX_SIZE 20

typedef struct{
	unsigned char fSendID:1;
	unsigned char fSendNoCommand:1;
	unsigned char fSendUnhandled:1;
	unsigned char fSendUnhandledValue:1;
}Event;

extern Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;

Event sEvents;
char ucRxString[MAX_SIZE];
char ucTxString[MAX_SIZE];

int main(){
	
DAC_MCP4921_Init();
UART_InitWithInt(9600);
	
	while(1){
		
		if(eReciever_GetStatus() == READY){
			
			Reciever_GetStringCopy(ucRxString);
			DecodeMsg(ucRxString);
			
			if( ( ucTokenNr != 0 ) && ( asToken[0].eType == KEYWORD ) ){
         switch(asToken[0].uValue.eKeyword){
					 case DACSET:
						 if((asToken[1].eType==NUMBER)&&(asToken[1].uValue.uiNumber<0x0FFF))
								DAC_MCP4921_Set_Adv(asToken[1].uValue.uiNumber);
						 else
							 sEvents.fSendUnhandledValue = 1;
					 break;
					 case ID:
					    sEvents.fSendID = 1;
					 break;							 
					 default:
						 sEvents.fSendUnhandled = 1;
					 break;
					}		 
			}
			else{	
			  sEvents.fSendNoCommand = 1;
			}
		}	
		
		if(Transmiter_GetStatus()== FREE){
			
		 if(sEvents.fSendID == 1 ){
			  CopyString("id_siri\n",ucTxString);	
				Transmiter_SendString(ucTxString);
				sEvents.fSendID =0;
			}
			else if(sEvents.fSendUnhandled== 1 ){
				CopyString("unhandled_command\n",ucTxString);
				Transmiter_SendString(ucTxString);
				sEvents.fSendUnhandled =0;
			}
			else if(sEvents.fSendNoCommand == 1 ){
				CopyString("no_command\n",ucTxString);
				Transmiter_SendString(ucTxString);
				sEvents.fSendNoCommand =0;
			}
			else if(sEvents.fSendUnhandledValue == 1 ){
				CopyString("unhandled_value\n",ucTxString);
				Transmiter_SendString(ucTxString);
				sEvents.fSendUnhandledValue =0;
			}
		}		
	 }			
}

