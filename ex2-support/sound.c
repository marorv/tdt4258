#include <stdlib.h>
#include <math.h>

#include "efm32gg.h"
#include "header.h"
#include "tunes.h"

#ifndef __sound_c
#define __sounc_c

#define MAX_VOLUME 200
#define SILENCE 0

int sampleCounter = 0;
int tuneCounter = 0;

int volume = 100;
int frequency = 0;
int high = 0;

int currentTone = 0;

struct tone *currentSongStartPointer;
int currentSong = 0;

int getAmplitude () {
	return (volume);
}
void play_sample() {

	if (high) {
		*DAC0_CH0DATA = volume;
		*DAC0_CH1DATA = volume;
	} else {
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
	}

	sampleCounter++;

	/* toggle high/low? */
	if (sampleCounter >= (frequency/2)) {
		high = (high) ? 0:1;
		sampleCounter = 0;
	}
}

void do_timer () {

	tuneCounter++;

	if (tuneCounter >= (currentSongStartPointer[currentTone].sampleCycles)) {
		currentTone += 1;
		if (currentTone >= songLengths[currentSong]) {
			currentTone = 0;
			stop_timer();
		}

		frequency = currentSongStartPointer[currentTone].frequency;
		tuneCounter = 0;
	}

	if (frequency != SILENCE) {
		play_sample();
	}
}

void reset () {
	sampleCounter = 0;
	tuneCounter = 0;
	currentTone = 0;
}

void set_current_song (int song) {
	song-=1;
	reset();
	if (song < 0 || song >= SONGS) {
		return;
	}
	currentSongStartPointer = songStartPointers[song];
	currentSong = song;
	frequency = currentSongStartPointer[currentTone].frequency;
}



void decrease_volume () {
	volume -= 10;

	if (volume < 0) {
		volume = 0;
	}
}
void increase_volume () {
	volume += 10;

	if (volume > MAX_VOLUME) {
		volume = MAX_VOLUME;
	}
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

#endif