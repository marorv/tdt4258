#ifndef __tunes_h
#define __tunes_h

struct tone {
	int frequency;
	int sampleCycles;
};

/**/

struct tone test[1] = {
	{200, 20000}
};

struct tone lightTest[4] = {
	{600, 20000},
	{1000, 20000},
	{800, 20000},
	{400, 20000}
};


#define SONGS  2

struct tone *songStartPointers[SONGS] = {test, lightTest};
int songLengths[SONGS] = {1, 4};

#endif