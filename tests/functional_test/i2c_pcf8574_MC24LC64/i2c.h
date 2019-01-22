
enum I2CTransmisionMode {TX,RX,RX_AFTER_TX,TX_AFTER_RX } ;

struct I2C_Params{
	enum I2CTransmisionMode eI2CTransmisionMode;
	unsigned char ucSlaveAddress;
	unsigned char *pucBytesForTx;
	unsigned char ucNrOfBytesForTx;
	unsigned char *pucBytesForRx;
	unsigned char ucNrOfBytesForRx;
	unsigned char ucDone;
};

void I2C_Init(void);
void I2C_ExecuteTransaction(struct I2C_Params);
