#include <stdlib.h>
#include <math.h>

#include "efm32gg.h"
#include "header.h"

#define PERIOD 20000
//Predefined amplitude
#define AMPL 1000

void setupSong()
{
	setupDAC();
	setupTimer(PERIOD);
	/*start timer? */
}

void playRandomSound()
{
	int sample = (int)(rand() % 10000);

	*DAC0_CH0DATA = sample;
	*DAC0_CH1DATA = sample;
}

//Function that returns sine value with given percentage and amplitude
int sineSample(float perc){
	float result = AMPL * sin(perc);
	return result;
}

//Notes procured from http://www.phy.mtu.edu/~suits/notefreqs.html
#define C0 16.35
#define Cs0 17.32
#define D0 18.35
#define Ds0 19.45
#define E0 20.60
#define F0 21.83
#define Fs0 23.12
#define G0 24.50
#define Gs0 25.96
#define A0 29.14
#define As0 29.14
#define B0 30.87

#define C1 32.70
#define Cs1 34.65
#define D1 36.71
#define Ds1 38.89
#define E1 41.20
#define F1 43.65
#define Fs1 46.25
#define G1 49.00
#define Gs1 51.91
#define A1 55.00
#define As1 58.27
#define B1 61.74

#define C2 65.41
#define Cs2 69.30
#define D2 73.42
#define Ds2 77.78
#define E2 82.41
#define F2 87.31
#define Fs2 92.50
#define G2 98.00
#define Gs2 103.83
#define A2 110.00
#define As2 116.54
#define B2 123.47
