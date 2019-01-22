#include <LPC210X.H>
#include "uart.h"
#include "string.h"
#include <stdlib.h>


#define TERMINATOR 0xD 

/*************************** UART ***************************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************************ INTERRUPTS ************************/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////////////// Zmienne globalne //////////////////////
RecieverBuffer sRecieverBuffer;
TransmiterBuffer sTransmiterBuffer;

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
		 Reciever_PutCharacterToBuffer(U0RBR);	 
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
			if(Transmiter_GetStatus() == BUSY)
					U0THR=Transmiter_GetCharacterFromBuffer();
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}


////////////////////////////////////////////
void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | 0x00000005;                                          // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT;             // dlugosc slowa, DLAB = 1
   U0DLL   = ((15000000)/16)/uiBaudRate;                                    // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                                   // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE|mTHRE_INTERRUPT_ENABLE ;   // Enable the Receive Data Available interrupt for UART0

   // INT
   VICVectAddr1  = (unsigned long) UART0_Interrupt;                         // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;                 // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);                           // Enable UART 0 Interrupt Channel
	
	sRecieverBuffer.ucCharCtr = 0;
	sTransmiterBuffer.cCharCtr = 0;
}



void Reciever_PutCharacterToBuffer(char cCharacter){
	if(sRecieverBuffer.ucCharCtr == RECIEVER_SIZE){
			sRecieverBuffer.ucCharCtr = 0;
			sRecieverBuffer.eStatus = OVERFLOW;
		}
		else if(cCharacter==TERMINATOR){
			sRecieverBuffer.cData[sRecieverBuffer.ucCharCtr] = NULL;
			sRecieverBuffer.ucCharCtr = 0;
			sRecieverBuffer.eStatus=READY;
		}
		else{
			sRecieverBuffer.cData[sRecieverBuffer.ucCharCtr] = cCharacter;
			sRecieverBuffer.ucCharCtr = sRecieverBuffer.ucCharCtr + 1;
			sRecieverBuffer.eStatus = EMPTY;
		}
}
//************************************************************
	
eRecieverStatus eReciever_GetStatus(void){
	return sRecieverBuffer.eStatus;
}
//************************************************************

void Reciever_GetStringCopy(char * ucDestination){
	CopyString(sRecieverBuffer.cData,ucDestination);
	sRecieverBuffer.eStatus = EMPTY;
}

char Transmiter_GetCharacterFromBuffer(void){

	if(1==sTransmiterBuffer.fLastCharacter){
		sTransmiterBuffer.fLastCharacter = 0;
		sTransmiterBuffer.eStatus = FREE;
		return NULL;
	}
	else if(NULL==sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr]){
		sTransmiterBuffer.fLastCharacter = 1;
		return TERMINATOR;
	}
	else{
		return sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr++];
	}

}

void Transmiter_SendString(char cString[]){
	sTransmiterBuffer.cCharCtr = 0;
	CopyString(cString,sTransmiterBuffer.cData);
	U0THR = sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr++];
	sTransmiterBuffer.eStatus = BUSY;
}
eTransmiterStatus Transmiter_GetStatus(void){
	return sTransmiterBuffer.eStatus;
}
