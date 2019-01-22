#include <LPC21xx.H>
#include <math.h>
#include "spi.h"
#include "delay.h"
#include "mcp4921.h"   

//================== public function ==================
void SPI_ConfigMaster(struct SPI_FrameParams sSPI_FramePar){
	
	PINSEL0 = SCK_ON_PIN0_4 | MISO_ON_PIN0_5 | MOSI_ON_PIN0_5 ;
	S0SPCR  = MSTR_ON_S0SPCR;   
	S0SPCR  |= (sSPI_FramePar.ucCpol << CPOL) | (sSPI_FramePar.ucCpha <<CPHA) | (sSPI_FramePar.ucClsbf<<LSBF);
	S0SPCCR = sSPI_FramePar.ClkDivider;	
}


void SPI_ExecuteTransaction(struct SPI_TransactionParams sSPI_TransPar){
	
	unsigned char ucRxNrBytes = sSPI_TransPar.ucNrOfBytesForRx   +   sSPI_TransPar.ucRxBytesOffset;
	unsigned char ucTxNrBytes = sSPI_TransPar.ucNrOfBytesForTx   +   sSPI_TransPar.ucTxBytesOffset;
	unsigned char ucTransactionNrBytes = (ucTxNrBytes  >  ucRxNrBytes)  ?   ucTxNrBytes : ucRxNrBytes;
	
	unsigned char ucValue;
	
	unsigned char ucBytesCounter;
  for(ucBytesCounter=0 ; ucBytesCounter<ucTransactionNrBytes ; ucBytesCounter++){
		 
		 if(ucBytesCounter>=sSPI_TransPar.ucTxBytesOffset){
			 ucValue= *(sSPI_TransPar.pucBytesForTx++);
		 }
		 
		S0SPDR = ucValue;
		while(!(S0SPSR & SPIF_ON_S0SPSR)){};
		ucValue = S0SPDR;
	 
		if(ucBytesCounter>=sSPI_TransPar.ucRxBytesOffset){
			*(sSPI_TransPar.pucBytesForRx++) = ucValue; 
		 }		 
	}
}



