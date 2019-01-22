#include "delay.h"

void Delay(unsigned long ulTime){
	for(ulTime;ulTime>0;ulTime--){
		unsigned long ulCounter;
		for(ulCounter=11995;ulCounter>0;ulCounter--){}
	}
}
