#ifndef HEADER_H
#define HEADER_H

#define MAX_AMPLITUDE 250
#define PI 3.14
#define SAMPLES_PER_SECOND 44100
#define SAMPLE_PERIOD 317
#define CLOCK_FREQUENCY 14000000	// 14MHz (default)

#define BUTTON_1 0xfe
#define BUTTON_2 0xfd
#define BUTTON_3 0xfb
#define BUTTON_4 0xf7
#define BUTTON_5 0xef
#define BUTTON_6 0xdf
#define BUTTON_7 0xbf
#define BUTTON_8 0x7f

void setupGPIO(void);
void setupTimer(uint32_t period);
void setupDAC(void);
void setupNVIC(void);

void pushPeriod(void);
void do_timer(void);
int sawTooth(float perc);

#endif				/* HEADER_H */
