#include "mc24lc64.h"
#include "i2c.h"

#define BUFFER_SIZE 3


static unsigned char aucTxBuffer[BUFFER_SIZE];
static unsigned char aucRxBuffer[BUFFER_SIZE];
struct I2C_Params MC24LC64_I2C_Params;

void MC24LC64_RandomRead (unsigned int WordAddress){

}
void MC24LC64_ByteWrite( unsigned int WordAddress, unsigned char Data){
		
	  MC24LC64_I2C_Params.eI2CTransmisionMode = TX_AFTER_RX;
		MC24LC64_I2C_Params.ucSlaveAddress = 0x40;
		MC24LC64_I2C_Params.ucNrOfBytesForTx = 1;
		MC24LC64_I2C_Params.ucNrOfBytesForRx = 0;
		MC24LC64_I2C_Params.ucDone = 0;
	
		aucTxBuffer[0] = Data;
	
		MC24LC64_I2C_Params.pucBytesForRx = aucRxBuffer;
		MC24LC64_I2C_Params.pucBytesForTx = aucTxBuffer;
		
		I2C_ExecuteTransaction(MC24LC64_I2C_Params);


}