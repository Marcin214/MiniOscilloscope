#include <LPC21xx.H>
#include "i2c.h"
#include "pcf8574.h"
#include "mc24lc64.h"

extern unsigned char ucPCF8574_Input;

int main(){
	unsigned long ulCounter;
	unsigned char counter = 0;
	IO0DIR = (1<<8);
	I2C_Init();

	 while(1){
			IO0SET = (1<<8);
		 for(ulCounter=100;ulCounter>0;ulCounter--){
			};
			PCF8574_Write(counter);
		  IO0CLR = (1<<8);
			for(ulCounter=1000000;ulCounter>0;ulCounter--){
			};
			counter++;
	 };

	
}

