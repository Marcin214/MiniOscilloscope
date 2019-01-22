#define NULL 0
#define RECIEVER_SIZE 20
#define TRANSMITER_SIZE 20
//**************************************************
typedef enum {EMPTY, READY, OVERFLOW}eRecieverStatus;

typedef struct{
char cData[RECIEVER_SIZE];
unsigned char ucCharCtr;
eRecieverStatus eStatus;
}RecieverBuffer;

typedef enum {FREE, BUSY}eTransmiterStatus;

typedef struct{
char cData[TRANSMITER_SIZE];
eTransmiterStatus eStatus;
unsigned char fLastCharacter;
unsigned char cCharCtr;
}TransmiterBuffer;

//**************************************************
void UART_InitWithInt(unsigned int uiBaudRate);
//**************************************************
void Reciever_PutCharacterToBuffer(char cCharacter);
eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
//**************************************************

char Transmiter_GetCharacterFromBuffer(void);
void Transmiter_SendString(char cString[]);
eTransmiterStatus Transmiter_GetStatus(void);




