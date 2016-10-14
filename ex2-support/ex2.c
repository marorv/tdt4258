#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "header.h"

void start_busy_waiting();
void start_interrupt_solution();

int main(void)
{
   set_current_song(0); /*set the current song to 0 by default*/

   /*call the setup functions*/
	setupGPIO();
	setupDAC();
   setupTimer(SAMPLE_PERIOD);
   /*if interruptions are enable, setup NVIC*/
   if (INTERRUPT_ENABLED) {
      setupNVIC();
   }
	
   /*check if interrupt is enabled and runs the corresponding mode*/
   if (INTERRUPT_ENABLED) {
      start_interrupt_solution();
   }
   else {
      start_busy_waiting();
   }

	return 0;
}

/*start the program with busy waiting*/
void start_busy_waiting () {
   int lastButtonsValue = *GPIO_PC_DIN; /*variable to keep track of which was the last button value*/
   int period = SAMPLE_PERIOD*0.9; 
   while(1) {
      /*check the last button value against the current button value, to see if a new button was clicked*/
      if (*GPIO_PC_DIN != lastButtonsValue) {
            did_click();
            lastButtonsValue = *GPIO_PC_DIN; /*set the last button value to the current one*/
      }
      /*check if the timer has reached the period counter, so that a new sample should be played*/
      if (*TIMER1_CNT >= period) {
         on_sample_timer_interrupt(); /*do the actions that should be done on each sample period*/
         *TIMER1_CNT = 0; /*reset the counter by setting the timer count to 0*/
      }
   }
}

/*run the program with interrupt and energy save*/
void start_interrupt_solution () {

   set_deep_sleep_mode(); 

   __asm__("wfi"); /*call the assmebly operation wfi to start wait-for-interrupt*/
}

void setupNVIC()
{
   *ISER0 |= (1 << 12); /*inclusive or on the value of ISER0 and 0000 1000 0000 0000 */
	*ISER0 |= 0x802; /*inclusive or on the value of ISER0 and 0x802 to enable gpio interrupt and keep the timer1 interrupt*/
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
