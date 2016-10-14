#ifndef __tunes_h
#define __tunes_h

struct tone {
	int frequency;
	int sampleCycles;
};

/**/

struct tone hitblock[3] = {
	{0, 500},
	{200, 5000},
	{100, 5000},
	
};

struct tone gameover[5] = {
	{0, 500},
	{505, 20000},
	{535, 20000},
	{600, 20000},
	{674, 20000}
};

struct tone hit[2] = {
	{0, 500},
	{100, 1000}
};

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
