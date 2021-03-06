#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   LOW_ENERGY_FREQUENCY    32768         //Clock frequency of the low energy oscillator
#define   HIGH_ENERGY_FREQUENCY   14000000      //Clock frequency of the normal clcok
#define   BITRATE                 32768         //Preferred number of 

/* Declaration of peripheral setup functions */
void setupLETimer();
void enableLETimer();
void setupNormalTimer(uint32_t period);
void enableDAC();
void setupDAC();
void setupNVIC();

bool energyEfficient = true;

/* Your code will start executing here */
int main(void) 
{  
  /* Call the peripheral setup functions */
  setupDAC();
  enableDAC();
  setupGPIO();

  if (energyEfficient) {
    setupLETimer();   //We don't supply a period here because we want the default value
    enableLETimer();
  }
  else {
    setupNormalTimer(HIGH_ENERGY_FREQUENCY / BITRATE);
  }
  
  /* Enable interrupt handling */
  setupNVIC();
  
  /* TODO for higher energy efficiency, sleep while waiting for interrupts
     instead of infinite loop for busy-waiting
  */
  if (energyEfficient)  *SCR = 6;
  else                  __asm("wfi");

  return 0;
}

void setupNVIC() {
  if (energyEfficient) {
    *ISER0 |= 1 << 26; 
  }	
  else {
    *ISER0 |= 1 << 12;
  }
  *ISER0 |= 1 << 11;
  *ISER0 |= 1 << 1;

  /* TODO use the NVIC ISERx registers to enable handling of interrupt(s)
     remember two things are necessary for interrupt handling:
      - the peripheral must generate an interrupt signal
      - the NVIC must be configured to make the CPU handle the signal
     You will need TIMER1, GPIO odd and GPIO even interrupt handling for this
     assignment.
  */
}

/* if other interrupt handlers are needed, use the following names: 
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/
