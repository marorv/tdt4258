#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
	/*
	   TODO enable and set up the Digital-Analog Converter

	   1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0
	   2. Prescale DAC clock by writing 0x50010 to DAC0_CTRL
	   3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL
	   4. Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt
	 */

	*CMU_HFPERCLKEN0 |= (1 << 17);

	/* Comp: Prescale DAC clock to 437,5 KHz, and enable the DAC output to the amplifier.
	* Write 1 to OUTMODE, which enables DAC output to pin
	* Write 5 to PRESC, which meens the clock division factor is 2^5=32 
	* write 1 to CONVMODE to set DAC in sample/old mode
	*/
	*DAC0_CTRL = 0x50010;

	/* Enable (automatic refresh for) channel 0 */
	*DAC0_CH0CTRL = 1;

	/* Enable (automatic refresh for) channel 1 */
	*DAC0_CH1CTRL = 1;
}
