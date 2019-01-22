#include <LPC21xx.H>
#include <math.h>
#include "spi.h"
#include "delay.h"
#include "mcp4921.h"   

//================== private function ==================
static void Wait(void){
  while(!(S0SPSR & SPIF_ON_S0SPSR)){}; 
}

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
	
	unsigned char ucBytesCounter;
  for(ucBytesCounter=0 ; ucBytesCounter<ucTransactionNrBytes ; ucBytesCounter++){
		 
		 if(ucBytesCounter>=sSPI_TransPar.ucTxBytesOffset){
			 S0SPDR= *(sSPI_TransPar.pucBytesForTx++);
		 }
		 else {
			 S0SPDR= *(sSPI_TransPar.pucBytesForTx);
		 }
		 
		Wait();
	 
		if(ucBytesCounter>=sSPI_TransPar.ucRxBytesOffset){
			*(sSPI_TransPar.pucBytesForRx++) = S0SPDR; 
		 }
		 else {
			*(sSPI_TransPar.pucBytesForRx) = S0SPDR;
		 }
	}
}



