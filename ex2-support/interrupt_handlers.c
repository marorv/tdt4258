#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "header.h"

#include "efm32gg.h"
#include "header.h"

int counter = 0;
bool isHigh = false;

void did_press();
void clear_gpio_interrupt();
/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1; /*reset the interrupt signal*/
	do_timer(); /**/
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
	/* check which button is pressed, and call the function corresponding to that button */
	switch (*GPIO_PC_DIN){ /*switch on the value of the buttons currently held down*/
		case BUTTON_1: /*if the value is the same as that of button one pressed down (as defined in the header.h file), call the button1 clicked function*/
			onButton1Click();
			break;
		case BUTTON_2:
			onButton2Click();
			break;

		case BUTTON_3:
			onButton3Click();
			break;

		case BUTTON_4:
			onButton4Click();
			break;
		case BUTTON_5:
			onButton5Click();
			break;
	}
}

/*function that clears the gpio interrupt*/
void clear_gpio_interrupt()
{
	*GPIO_IFC = 0xffff;
}

void set_deep_sleep_mode () {
	*SCR = 6;
}
void set_run_mode () {
	*SCR = 0;
}
