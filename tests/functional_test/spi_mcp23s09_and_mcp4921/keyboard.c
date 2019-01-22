#include <LPC21xx.H>
#include "keyboard.h"
#include "delay.h"

#define BUTT1   (1<<4)
#define BUTT2   (1<<6)
#define BUTT3   (1<<5)
#define BUTT4   (1<<7)



enum KeyboardState eKeyboardRead(void){

  if((IO0PIN&BUTT1) == 0){ 
			return BUTTON_1;
	}
  else if((IO0PIN&BUTT2) == 0){ 
			return BUTTON_2;
	}
	
	else if((IO0PIN&BUTT3) == 0){ 
			return BUTTON_3;
	}
	else if((IO0PIN&BUTT4) == 0){ 
			return BUTTON_4;
	}
  return RELASED;
}

void KeyboardInit(void){
IO0DIR &= ~( BUTT1|BUTT2|BUTT3|BUTT4);

}
