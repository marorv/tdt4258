#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

int counter = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
	
	counter = counter + 1;

	if (counter >= 317) {
		counter = 0;
		*GPIO_PA_DOUT = 0x0000;
	}

	   		
	*TIMER1_IFC = 1;
}
/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	

	*GPIO_PA_DOUT = 0xfe00; 
	*GPIO_IFC = 0xffff;
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	*GPIO_PA_DOUT = 0x5500;
	*GPIO_IFC = 0xffff;
}