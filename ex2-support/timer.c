#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "header.h"

/* function to setup the timer */
void setupTimer(uint16_t period)
{

	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;	/*(1 << 6);. Enable the clock for timer1*/

	*TIMER1_TOP = period; /*set the timer highest value, for interrupts this is number of clock cycles between each timer1 interrupt, and for polling this is the value the timer stops at*/

	/*if interrupt is enabled, setup interrupt generation for timer1*/
	if (INTERRUPT_ENABLED) {
		*TIMER1_IEN = 1;
	}

	/*start timer */
	//*TIMER1_CMD = 1;

}

void start_timer(){
	*TIMER1_CMD = 1; /*the value of TIMER1_CMD indicates the state of the timer, where 1 is on and 2 is off*/
}
void stop_timer () {
	*TIMER1_CMD = 2; /*the value of TIMER1_CMD indicates the state of the timer, where 1 is on and 2 is off*/
}
