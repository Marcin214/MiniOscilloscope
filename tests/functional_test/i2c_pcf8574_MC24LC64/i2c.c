#include <LPC21xx.H>
#include "i2c.h"

#define VIC_I2C_CHANNEL_NR  9
#define IRQ_SLOT_ENABLE    0x00000020

extern unsigned char ucPCF8574_Input;
struct I2C_Params I2C_ParamsExecuted;
unsigned char ucTxByteCounter;
unsigned char ucRxByteCounter;

void I2C_ExecuteTransaction(struct I2C_Params sI2C_Params){

		I2C_ParamsExecuted = sI2C_Params;
		I2CONCLR = 0xFF;     //Clear all I2C settings
		I2CONSET = 0x40;     //Enable the I2C interface
		I2CONSET = 0x20;     //Start Condition
	
}


__irq void I2C_Interrupt(void){
	
	switch(I2STAT){
		case (0x08):     			                                          //Start Bit
			I2CONCLR =0x20; 		                                          //Clear start bit
			ucRxByteCounter = 0;
		  ucTxByteCounter = 0;
			if((I2C_ParamsExecuted.eI2CTransmisionMode == TX) || (I2C_ParamsExecuted.eI2CTransmisionMode == RX_AFTER_TX)){
				  I2DAT = I2C_ParamsExecuted.ucSlaveAddress;                //Send address and WRITE Bit
			}	
			else
					I2DAT = (I2C_ParamsExecuted.ucSlaveAddress | 0x01);       //Send address and READ Bit
		break;
		
		case (0x10):     			                                          //Second Start Bit
			I2CONCLR =0x20; 		                                          //Clear start bit
		  if(I2C_ParamsExecuted.eI2CTransmisionMode == TX_AFTER_RX){
				  I2C_ParamsExecuted.ucNrOfBytesForRx = 0;
				  I2DAT = I2C_ParamsExecuted.ucSlaveAddress;                //Send address and WRITE Bit
			}	
			else{
				  I2C_ParamsExecuted.ucNrOfBytesForTx = 0;
					I2DAT = (I2C_ParamsExecuted.ucSlaveAddress | 0x01);       //Send address and READ Bit
			}
		break;
		
		case (0x18):   																								  //Slave address+W, ACK
			I2DAT = I2C_ParamsExecuted.pucBytesForTx[0];							    //Write data to tx register
		break;		
		case (0x20):                                                    //Slave address +W,Not ACK
			I2CONSET = 0x10;								                             //Stop Condition
		break;	
		case (0x28):    							                                  //data sent, ACK
		 if(ucTxByteCounter < (I2C_ParamsExecuted.ucNrOfBytesForTx - 1)){
				I2DAT = I2C_ParamsExecuted.pucBytesForTx[1+ucTxByteCounter];
				ucTxByteCounter++;
		 }
		 else{
			 if(I2C_ParamsExecuted.ucNrOfBytesForRx > 0){
			 		I2CONSET = 0x20;    																	     //Again Start Condition
			 }
			 else
				I2CONSET = 0x10;								                             //Stop Condition
			  I2C_ParamsExecuted.ucDone =1;
		 }	
		break;
		
		case (0x40):     									                               // Slave address +R, ACK
			if (ucRxByteCounter < I2C_ParamsExecuted.ucNrOfBytesForRx - 1){
         I2CONSET = 0x04;                                            // Enable ACK for data byte
      }
      else
         I2CONCLR = 0x04;
		break;	
		case (0x48):                                                      // Slave address +R, NOT ACK
			I2CONSET = 0x10;								                                //Stop Condition
		break;		
		case (0x50):                                                      // Data received, ACK
			I2C_ParamsExecuted.pucBytesForRx[ucRxByteCounter] = I2DAT;
			ucPCF8574_Input = I2DAT;
			ucRxByteCounter++;
		break;		
		case (0x58):     										                              // Data received, NOT ACK	
		 if(I2C_ParamsExecuted.ucNrOfBytesForTx > 0){
			 		I2CONSET = 0x20;    																	   	  //Again Start Condition
			 }
		 else{
			I2C_ParamsExecuted.pucBytesForRx[ucRxByteCounter] = I2DAT;
			I2CONSET = 0x10;								                                //Stop Condition
		  I2C_ParamsExecuted.ucDone = 1;
		 }
		break;	
		default:                                                          //Disable the I2C interface
			  I2CONCLR = 0x40;
        I2C_ParamsExecuted.ucDone =1;
			break;
	}
	I2CONCLR = 0x08;																										// Clear I2C interrupt flag
	VICVectAddr = 0; 																										// Acknowledge Interrupt
}

	

void I2C_Init(void){

	VICVectAddr1  = (unsigned long) I2C_Interrupt;         
  VICVectCntl1  = IRQ_SLOT_ENABLE | VIC_I2C_CHANNEL_NR;        
  VICIntEnable |= (0x1 << VIC_I2C_CHANNEL_NR);                         
	
	PINSEL0 |= 0x50;
	I2SCLH  =  0x80;
	I2SCLL  =  0x80;

}