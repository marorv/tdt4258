#include <stdlib.h>

#include "efm32gg.h"
#include "header.h"

#define maxVolume

int volume = 100;
int sampleCounter = 0;

int getFrequencyOfNote (char c) {
	switch (c) {
		case 'C':
		return 190;
	}
}

int pushPeriod () {
	sampleCounter += 1;
}
