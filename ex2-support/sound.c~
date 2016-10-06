#include <stdlib.h>

#include "efm32gg.h"
#include "header.h"

#define PERIOD 20000

void setupSong() {
	setupDAC();
	setupTimer(PERIOD);
	/*start timer?*/
}

void playRandomSound() {
	int sample = (int) (rand() % 10000);

	*DAC0_CH0DATA = sample;
	*DAC0_CH1DATA = sample;
}

