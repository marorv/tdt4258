#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function that sets up the GPIO pins*/
void setupGPIO()
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	*GPIO_PA_CTRL = 0; /*set the drive strength of the LED pins to low*/

	*GPIO_PA_MODEH = 0x55555555; /*set the LED pins port A 8-15 to output pins*/

	*GPIO_PA_DOUT = 0xfe00;	/*setting the LED value to 0xfe00, which indicates that only the leftmost light should be on at start time*/

	*GPIO_PC_MODEL = 0x33333333; /*set button pins to input pins*/

	*GPIO_PC_DOUT = 0xff; /*enable data output for button pins*/

	*GPIO_EXTIPSELL = 0x22222222; /*set GPIO_EXTIPSELL to 0x22222222 for the button pins*/

	*GPIO_EXTIFALL = 0xff; /*set GPIO_EXTIFALL TO 0xff to active interrupts on button down*/

	*GPIO_IEN = 0xff; /* set GPIO_IEN to 0xff to enable interrupt generation */
}
