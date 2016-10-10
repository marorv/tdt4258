#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

int counter = 0;
bool isHigh = false;

void did_press();
void clear_gpio_interrupt();
//void playRandomSound();
/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	

	*TIMER1_IFC = 1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */

	did_press();
	clear_gpio_interrupt();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	did_press();
	clear_gpio_interrupt();
}

void did_press()
{
	/*todo on press */
}

void clear_gpio_interrupt()
{
	*GPIO_IFC = 0xffff;
}
