#include <LPC21xx.H>
#include "pcf8574.h"
#include "i2c.h"

#define BUFFER_SIZE 3

unsigned char ucPCF8574_Input;
static unsigned char aucTxBuffer[BUFFER_SIZE];
static unsigned char aucRxBuffer[BUFFER_SIZE];
struct I2C_Params PCF8574_I2C_Params;

void PCF8574_Read(void){
	  PCF8574_I2C_Params.eI2CTransmisionMode = RX;
		PCF8574_I2C_Params.ucSlaveAddress = 0x40;
		PCF8574_I2C_Params.ucNrOfBytesForTx = 0;
		PCF8574_I2C_Params.ucNrOfBytesForRx = 1;
		PCF8574_I2C_Params.ucDone = 0;
		
		I2C_ExecuteTransaction(PCF8574_I2C_Params);
}

void PCF8574_Write(unsigned char ucData){
	
		PCF8574_I2C_Params.eI2CTransmisionMode = TX;
		PCF8574_I2C_Params.ucSlaveAddress = 0x40;
		PCF8574_I2C_Params.ucNrOfBytesForTx = 1;
		PCF8574_I2C_Params.ucNrOfBytesForRx = 0;
		PCF8574_I2C_Params.ucDone = 0;
	
		aucTxBuffer[0] = ucData;
	
		PCF8574_I2C_Params.pucBytesForRx = aucRxBuffer;
		PCF8574_I2C_Params.pucBytesForTx = aucTxBuffer;
		
		I2C_ExecuteTransaction(PCF8574_I2C_Params);

}

