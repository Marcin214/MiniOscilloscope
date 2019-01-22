enum LedState{LED_RIGHT, LED_STOP, LED_LEFT};
static enum LedState eLedState = LED_STOP;

void LedOn(unsigned char ucLedIndeks);
void LedInit(void);
void LedStepLeft(void);
void LedStepRight(void);
