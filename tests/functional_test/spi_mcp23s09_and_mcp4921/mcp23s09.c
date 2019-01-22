#include<LPC21xx.h>
#include "spi.h"
#include "mcp23s09.h"

#define BUFFER_SIZE 3

//================== MCP23S09 ==================
#define REGISTER_WRITE_OPCODE 0x40
#define REGISTER_READ_OPCODE  0x41

#define IODIR_REGISTER_ADDR   0x00
#define GPIO_REGISTER_ADDR    0x09

#define CLEAR_CS_PIN          (IO0CLR |=	CS_IO)
#define IODIR_PINS_OUTPUT     0

static unsigned char aucTxBuffer[BUFFER_SIZE];
static unsigned char aucRxBuffer[BUFFER_SIZE];

struct SPI_FrameParams MCP23S09_SPI_FrameParams;
struct SPI_TransactionParams MCP23S09_SPI_TransactionParams;


void Port_MCP23S09_Init(void){

	MCP23S09_SPI_FrameParams.ucCpha=0;
	MCP23S09_SPI_FrameParams.ucCpol=0;
	MCP23S09_SPI_FrameParams.ucClsbf=0;
	MCP23S09_SPI_FrameParams.ClkDivider=254;
	SPI_ConfigMaster(MCP23S09_SPI_FrameParams);
	
}

void Port_MCP23S09_InitCSPin(void){
	
	IO0DIR |= CS_IO;  
	IO0SET  = CS_IO; 	

}
void Port_MCP23S09_Set(unsigned char ucData){
	
	MCP23S09_SPI_TransactionParams.ucNrOfBytesForRx=0;
	MCP23S09_SPI_TransactionParams.ucNrOfBytesForTx=3;
	MCP23S09_SPI_TransactionParams.ucRxBytesOffset=0;
	MCP23S09_SPI_TransactionParams.ucTxBytesOffset=0;

	aucTxBuffer[0]  =  REGISTER_WRITE_OPCODE;
	aucTxBuffer[1]  =  IODIR_REGISTER_ADDR;
	aucTxBuffer[2]  =  IODIR_PINS_OUTPUT;
	MCP23S09_SPI_TransactionParams.pucBytesForTx= aucTxBuffer;
	MCP23S09_SPI_TransactionParams.pucBytesForRx= aucRxBuffer;
	
	CLEAR_CS_PIN;
	SPI_ExecuteTransaction(MCP23S09_SPI_TransactionParams);
	Port_MCP23S09_InitCSPin();

	aucTxBuffer[0]  =  REGISTER_WRITE_OPCODE;
	aucTxBuffer[1]  =  GPIO_REGISTER_ADDR;
	aucTxBuffer[2]  =  ucData;
	MCP23S09_SPI_TransactionParams.pucBytesForTx= aucTxBuffer;
	MCP23S09_SPI_TransactionParams.pucBytesForRx= aucRxBuffer;
	
	CLEAR_CS_PIN;
	SPI_ExecuteTransaction(MCP23S09_SPI_TransactionParams);
	Port_MCP23S09_InitCSPin();
	
}

unsigned char Port_MCP23S09_Get(void){
	
	MCP23S09_SPI_TransactionParams.ucNrOfBytesForRx=0;
	MCP23S09_SPI_TransactionParams.ucNrOfBytesForTx=3;
	MCP23S09_SPI_TransactionParams.ucRxBytesOffset=0;
	MCP23S09_SPI_TransactionParams.ucTxBytesOffset=0;

	aucTxBuffer[0]  =  REGISTER_WRITE_OPCODE;
	aucTxBuffer[1]  =  IODIR_REGISTER_ADDR;
	aucTxBuffer[2]  =  IODIR_PINS_OUTPUT;
	MCP23S09_SPI_TransactionParams.pucBytesForTx= aucTxBuffer;
	MCP23S09_SPI_TransactionParams.pucBytesForRx= aucRxBuffer;
	
	CLEAR_CS_PIN;
	SPI_ExecuteTransaction(MCP23S09_SPI_TransactionParams);
	Port_MCP23S09_InitCSPin();

	MCP23S09_SPI_TransactionParams.ucNrOfBytesForRx=1;
	MCP23S09_SPI_TransactionParams.ucNrOfBytesForTx=2;
	MCP23S09_SPI_TransactionParams.ucRxBytesOffset=2;
	MCP23S09_SPI_TransactionParams.ucTxBytesOffset=0;

	aucTxBuffer[0]  =  REGISTER_READ_OPCODE;
	aucTxBuffer[1]  =  GPIO_REGISTER_ADDR;
	MCP23S09_SPI_TransactionParams.pucBytesForTx= aucTxBuffer;
	MCP23S09_SPI_TransactionParams.pucBytesForRx= aucRxBuffer;
	
	CLEAR_CS_PIN;
	SPI_ExecuteTransaction(MCP23S09_SPI_TransactionParams);
	Port_MCP23S09_InitCSPin();

	return 	aucRxBuffer[0];
}
