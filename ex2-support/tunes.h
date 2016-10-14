#ifndef __tunes_h
#define __tunes_h

struct tone {
	int frequency;
	int sampleCycles;
};

/*

the songs are defined with the sample frequency and sample cycles duration
The frequency is calculated by the following formula: (SAMPLES_PER_SECOND / <ACTUAL SOUND FREQUENCY>) = frequency, where the actual
sound frequency is the sound frequency of the tone.
The sample cycles are calculated by the following formula: (SAMPLES_PER_SECOND/1000)* MILLISECONDS = sampleCycles

*/

/*sound that indicates a block has been hit*/
struct tone hitblock[3] = {
	{0, 500},
	{200, 5000},
	{100, 5000},
	
};

/*song that indicates the game is over*/
struct tone gameover[5] = {
	{0, 500},
	{505, 20000},
	{535, 20000},
	{600, 20000},
	{674, 20000}
};

/*sound that indicates that the ball has hit something*/
struct tone hit[2] = {
	{0, 500},
	{100, 1000}
};

/*the start of the game of thrones song*/
struct tone got[11] = {
	{0, 500},
	{113, 15000},
	{169, 15000},
	{134, 7500},
	{126, 7500},
	{113, 15000},
	{169, 15000},
	{134, 7500},
	{126, 7500},
	{113, 15000},
	{169, 15000},
};

#define SONGS  4

struct tone *songStartPointers[SONGS] = {hitblock, gameover, hit, got};
int songLengths[SONGS] = {3, 5, 2, 11};

#endif
