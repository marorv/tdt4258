#include <stdlib.h>

#include "efm32gg.h"
#include "header.h"

void setupSong() {
	setupDAC();
	setupTimer();
	/*start timer?*/
}

void playRandomSound() {
	int sample = rand() % 10000

	*DAC0_CH0DATA = sample;
	*DAC1_CH0DATA = sample;
}

