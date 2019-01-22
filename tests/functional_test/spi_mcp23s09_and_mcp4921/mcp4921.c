#include <LPC21xx.H>
#include "mcp4921.h"   
#include "spi.h"

#define BUFFER_SIZE 2

//================== MCP4921 ==================
//4 config bits 
#define SEL_ON_MCP      (1 << 7)     // DACA or DACB Select bit
#define BUF_ON_MCP      (1 << 6)     // VREF Input Buffer Control bit
#define GA_ON_MCP       (1 << 5)     // Output Gain Select bit
#define SHDN_ON_MCP     (1 << 4)     // Output Power Down Control bit

static unsigned char aucTxBuffer[BUFFER_SIZE];
static unsigned char aucRxBuffer[BUFFER_SIZE];

struct SPI_FrameParams MCP4921_SPI_FrameParams;
struct SPI_TransactionParams MCP4921_SPI_TransactionParams;

void DAC_MCP4921_Init(void){

	MCP4921_SPI_FrameParams.ucCpha=0;
	MCP4921_SPI_FrameParams.ucCpol=0;
	MCP4921_SPI_FrameParams.ucClsbf=0;
	MCP4921_SPI_FrameParams.ClkDivider=8;
	SPI_ConfigMaster(MCP4921_SPI_FrameParams);
	
	MCP4921_SPI_TransactionParams.ucNrOfBytesForRx=0;
	MCP4921_SPI_TransactionParams.ucNrOfBytesForTx=2;
	MCP4921_SPI_TransactionParams.ucRxBytesOffset=0;
	MCP4921_SPI_TransactionParams.ucTxBytesOffset=0;
	MCP4921_SPI_TransactionParams.pucBytesForTx= aucTxBuffer;
	MCP4921_SPI_TransactionParams.pucBytesForRx= aucRxBuffer;
  DAC_MCP4921_InitCSPin();
	
}

void DAC_MCP4921_InitCSPin(void){
	
	IO0DIR |= CS_DAC;  
	IO0SET  = CS_DAC; 	
	
}

void DAC_MCP4921_Set_Adv(unsigned int uiData){
	
	IO0CLR |=	CS_DAC;
	
	aucTxBuffer[0]  = (uiData >> 8) & BYTE;              // store uiVoltage[11..8] to ucTempByteValue [3..0]
	aucTxBuffer[0] |= GA_ON_MCP | SHDN_ON_MCP;           // store control word to ucTempByteValue[7..4]
	
	aucTxBuffer[1] = uiData & BYTE;                      //store uiVoltage[7..0] to ucTempByteValue [7..0]
	
	SPI_ExecuteTransaction(MCP4921_SPI_TransactionParams);
	DAC_MCP4921_InitCSPin();
	
}
