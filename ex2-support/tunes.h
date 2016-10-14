#ifndef __tunes_h
#define __tunes_h

struct tone {
	int frequency;
	int sampleCycles;
};

/**/

struct tone test[4] = {
	{200, 20000},
	{100, 10000},
	{0, 40000},
	{200, 10000}
};

struct tone lightTest[4] = {
	{600, 20000},
	{1000, 20000},
	{800, 20000},
	{400, 20000}
};

struct tone gameover[4] = {
	{505, 20000},
	{535, 20000},
	{600, 20000},
	{674, 20000}
};

struct tone hit[1] = {
	{100, 1000}
};
#define SONGS  4

struct tone *songStartPointers[SONGS] = {test, lightTest, gameover, hit};
int songLengths[SONGS] = {4, 4, 4, 1};

#endif
