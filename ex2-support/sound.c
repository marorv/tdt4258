#include <stdlib.h>
#include <math.h>

#include "efm32gg.h"
#include "header.h"
#include "tunes.h"

#ifndef __sound_c
#define __sounc_c

#define MAX_VOLUME 200 /*define the max volume allowed*/
#define SILENCE_FREQUENCY 0 /*if the frequency is this value, a sample should not be played, creating silence. We used 0 because this is a frequency that is not possible to create*/

int sampleCounter = 0; /*counter that keeps track of where in a sound period we are*/
int toneCounter = 0; /*counter that keeps track of where in a tone we are*/

int volume = 100; /*variable defining the current volume (amplitude) of the samples*/
int frequency = 0; /*variable that defines the frequency of the current tone*/
int high = 0; /*variable that keeps track of wether the value of a sample should be high or low in a squared period*/

int currentTone = 0; /*variable that keeps track of the current tone in a song (e.g tone 4 in a song that has 10 tones)*/

struct tone *currentSongStartPointer; /*a pointer to the start of the current song*/
int currentSong = 0; /*int number that keeps track of the index of the current song (in the list of songs located in tunes.h)*/

/*function that stops that timer, sets the song number as current song, then starts the song.*/
void play_song (int number) {
	//stop_timer();
	set_current_song(number);
	start_timer();
}

/*
	function that feeds the dac channels with a sample, making it produce sound.
	the sound will be played with a squared period.
*/
void play_sample() {
	/*the sample value oscillated from the amplitude (volume) to 0 to make a squared period*/
	if (high) {
		/*set the sample value to the amplitude (volume)*/
		*DAC0_CH0DATA = volume;
		*DAC0_CH1DATA = volume;
	} else {
		/*set the sample value to 0 */
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
	}

	sampleCounter++;

	/*if the sampleCounter has reached half a frequency, change the high value and reset the sampleCounter.*/
	if (sampleCounter >= (frequency/2)) {
		high = (high) ? 0:1;
		sampleCounter = 0;
	}
}

/*function that should be called on every sample timer interrupt.*/
void on_sample_timer_interrupt () {
	/*increase the toneCounter, so that it can keep track on how far into a tone it is.*/
	toneCounter++;

	/*if the tone counter has reached the number of samples the current tone has, the tone should change to the next tone*/
	if (toneCounter >= (currentSongStartPointer[currentTone].sampleCycles)) {
		currentTone += 1; /*go to the next tone*/
		if (currentTone >= songLengths[currentSong]) { /*if the tone is the last tone in the current song, stop the timer*/
			currentTone = 0; /*reset current tone so the same song can be played again*/
			stop_timer(); /*stop the sample timer*/
			set_deep_sleep_mode(); /*enable deep sleep mode if possible*/
		}

		frequency = currentSongStartPointer[currentTone].frequency; /*set the frequency to the frequency of the new tone*/
		toneCounter = 0; /*reset the tone counter, so that it can keep track of the new tone*/
	}

	/*if the frequency is the same as the defined SILENCE value, the sample should not be played (to save power)*/
	if (frequency != SILENCE_FREQUENCY) {
		play_sample(); /*play the sample*/
	}
}

/*function used to reset all the timers*/
void reset () {
	sampleCounter = 0;
	toneCounter = 0;
	currentTone = 0;
}

/*function used to change the current song, to a new song defined by an int number*/
void set_current_song (int song) {
	song-=1; /*decrement because the songs are defined as 1,2,3 and so on, while the list starts at 0*/
	reset(); /*reset the timers, so that the new song start from the beginning*/

	/*if the song is outside the scope of the song list, one should return false*/
	if (song < 0 || song >= SONGS) {
		return; //0;
	}
	currentSongStartPointer = songStartPointers[song]; /*set the currentSongPointer to point to the new selected song*/
	currentSong = song; /*set the int value of the current song*/
	frequency = currentSongStartPointer[currentTone].frequency; /*set the start frequency of the new song*/

	//return 1; /*return success indication*/
}


/*function that will decrease the volume value by 10*/
void decrease_volume () {
	volume -= 10; /*decrement volume by 10*/

	/*if this decreases the volume below 10, set the value to 10*/
	if (volume < 0) {
		volume = 0;
	}
}
void increase_volume () {
	volume += 10; /*increment volume by 10*/

	/*if this increases the volume above 10, set the value to the max volume defined earlier in this file.*/
	if (volume > MAX_VOLUME) {
		volume = MAX_VOLUME;
	}
}

#endif