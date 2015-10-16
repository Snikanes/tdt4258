#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer */
void setupTimer(uint16_t period)
{
  /*
    This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.
  */  
  
  //  1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
  uint16_t timer_bit = 0x06;
  *CMU_HFPERCLKEN0 |= timer_bit;
  
  //  2. Write the period to register TIMER1_TOP
  *TIMER1_TOP = period;
  
  //  3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
  *TIMER1_IEN = 0x01;
  
  //  4. Start the timer by writing 1 to TIMER1_CMD
  *TIMER1_CMD = 0x01;
  
}


