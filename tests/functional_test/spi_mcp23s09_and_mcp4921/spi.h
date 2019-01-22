#define BYTE               0xFF

//SCK-P0.4; MISO-P0.5; MOSI-P0.6 CS_DAC-P0.10 CS_IO-P0.9
//=================== SPI0 ===================
// Pin Function Select in Pin Connect Block

#define SCK_ON_PIN0_4      (1 << 8) //P0.4 
#define MISO_ON_PIN0_5    (1 << 10) //P0.5 
#define MOSI_ON_PIN0_5    (1 << 12) //P0.6
#define SSEL_ON_PIN0_5    (1 << 14) //P0.7

// Chip Select
#define CS_DAC  		     		  (1 << 10) //P0.10
#define CS_IO  		     		    (1 << 9) //P0.9

//SPCR Control Register
#define CPHA	  3
#define CPOL    4 
#define MSTR	  5  
#define LSBF	  6  

//SPCR Control Register
#define CPHA_ON_S0SPCR	    (1 << 3)  // Clock phase control
#define CPOL_ON_S0SPCR	    (1 << 4)  // Clock polarity control
#define MSTR_ON_S0SPCR	    (1 << 5)  // Master mode select

//SPSR Status Register
#define SPIF_ON_S0SPSR 	     (1 << 7) // SPI transfer complete flag

//SPCCR Clock Counter Register
#define DIVIDER_ON_S0SPCCR      0xFE   // Divider PLCK


struct SPI_FrameParams{ 
unsigned char  ucCpha;
unsigned char  ucCpol;
unsigned char  ucClsbf;
unsigned char  ClkDivider;
};


struct SPI_TransactionParams{ 
unsigned char *pucBytesForTx;    // wskaznik na tablice z bajtami do wyslania
unsigned char  ucNrOfBytesForTx; // ilosc bajtów do wyslania
unsigned char  ucTxBytesOffset;  // offset bajtów do wyslania
unsigned char *pucBytesForRx;    // wskaznik na tablice na odebrane bajty
unsigned char  ucNrOfBytesForRx; // ilosc bajtów do odebrania
unsigned char  ucRxBytesOffset;  // offset bajtów do odebrania
};


void SPI_ConfigMaster(struct SPI_FrameParams);
void SPI_ExecuteTransaction(struct SPI_TransactionParams);

