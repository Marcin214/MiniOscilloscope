#include <LPC21xx.H>
#include "i2c.h"


int main(){
	I2C_Init();
	PCF8574_Write(0xAA);
	
	 while(1){};

	
}

