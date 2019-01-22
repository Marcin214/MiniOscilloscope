#include <LPC21xx.H>
#include "i2c.h"

#define VIC_I2C_CHANNEL_NR  9
#define IRQ_SLOT_ENABLE    0x00000020

unsigned char ucI2cAddr;
unsigned char ucI2cData;
unsigned char ucPCF8574_Input;
static unsigned char waitForEndTransmission;

__irq void I2C_Interrupt(void){
	
	switch(I2STAT){
		case (0x08):     			             //Start Bit
			I2CONCLR =0x20; 		             //Clear start bit
		  I2DAT = ucI2cAddr;               //Send address and write Bit
		break;
		case (0x18):   											//Slave address+W, ACK
			I2DAT = ucI2cData;							  //Write data to tx register
		break;		
		case (0x20):                        //Slave address +W,Not ACK
			I2DAT = ucI2cAddr;                // Resend address and write bit 
		break;	
		case (0x28):    							      //data sent, ACK
		  I2CONSET = 0x10;								  //Stop Condition
		break;
		
		case (0x40):     									  // Slave address +R, ACK
			I2CONSET =0x04; 									// Enable ACK for data byte
		break;	
		case (0x48):                        // Slave address +R, NOT ACK
			I2CONSET =0x20;                   // Resend Start Condition
		break;		
		case (0x50):                        // Data received, ACK
			ucPCF8574_Input= I2DAT;
			I2CONSET =0x10; 									// Stop Condition
			waitForEndTransmission =0;        
		break;
		
		case (0x58):     										 // Data received, NOT ACK
			I2CONSET =0x20; 									 // Resend Start Condition
		break;
		
		default: 
			break;
	}
	I2CONCLR = 0x08;												// Clear I2C interrupt flag
	VICVectAddr = 0; 												// Acknowledge Interrupt
}

	

void I2C_Init(void){

	VICVectAddr1  = (unsigned long) I2C_Interrupt;         
  VICVectCntl1  = IRQ_SLOT_ENABLE | VIC_I2C_CHANNEL_NR;        
  VICIntEnable |= (0x1 << VIC_I2C_CHANNEL_NR);                         
	
	PINSEL0 |= 0x50;
	I2SCLH  =  0x80;
	I2SCLL  =  0x80;

}

void PCF8574_Write(unsigned char ucData){
	
	  ucI2cAddr =0x40;
	  ucI2cData = ucData;
	
	  I2CONCLR = 0xFF;     //Clear all I2C settings
		I2CONSET = 0x40;     //Enable the I2C interface
		I2CONSET = 0x20;     //Start Condition
}

