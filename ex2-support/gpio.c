#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
	/* TODO set input and output pins for the joystick */

	/* Example of HW access from C code: turn on joystick LEDs D4-D8
	   check efm32gg.h for other useful register definitions
	 */
	/* enable GPIO clock */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	/*
	 * GPIO PA -> LEDS
	 */

	/* set low drive strength */
	*GPIO_PA_CTRL = 0;

	/* set pins A8-15 as output */
	*GPIO_PA_MODEH = 0x55555555;

	/* Turn off all leds (8-15) */
	*GPIO_PA_DOUT = 0xfe00;	// [1111 1111 0000 0000] only the most left 8 bits affects the leds because they are mapped to pins A8-A15 (see GPIO_MODEH). 

	/* set pins C0-C7 as input */
	*GPIO_PC_MODEL = 0x33333333;

	/* Enable data output on port C, pins 0-7 */
	*GPIO_PC_DOUT = 0xFF;

	/* enable external interrupts for port C, pins 0-7 */
	*GPIO_EXTIPSELL = 0x22222222;

	/* activate interrupt on button pushed down */
	*GPIO_EXTIFALL = 0xFF;

	/* Enable interrupt generation */
	*GPIO_IEN = 0xFF;
}
