#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer - never used */
void setupNormalTimer(uint16_t period) {
  //  This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked. 
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_TOP = period;
  *TIMER1_IEN = 1;
  *TIMER1_CMD = 1;
}

void setupLETimer(uint16_t period) {
	*CMU_OSCENCMD |= (1 << 8);					//Enables low frequency oscillator (8 or 6=??????)
	*CMU_HFCORECLKEN0 |= (1 << 4);				//Enable clock for the Low Energy Peripheral Interface
	*CMU_LFACLKEN0 |= CMU2_LFACLKEN0_LETIMER0;	//Enables clock for low energy timer
	*CMU_LFCLKSEL &= ~(0x3 << 0); 				// clear bits first
	*CMU_LFCLKSEL |= (2 << 0);	
	*CMU_CTRL &= ~(3 << 18);					// Sets wake up time of LFXO clock for quick wakeup
	//*LETIMER0_TOP = period;					//Sets the frequency of interrupt generation
  	*LETIMER0_IEN = 1;							//Enables interrupt generation
  	*LETIMER0_CMD = 1;							//Starts the low energy timer
}

void enableLETimer() {
	*LETIMER0_CTRL |= (1 << 9);
}

void disableLETimer() {
	*LETIMER0_CTRL &= ~(1 << 9);
}

