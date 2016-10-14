#ifndef HEADER_H
#define HEADER_H

#define INTERRUPT_ENABLED 1

#define MAX_AMPLITUDE 250
#define PI 3.14
#define SAMPLES_PER_SECOND 44100 /*the sample rate*/
#define SAMPLE_PERIOD 317 /*the sample period, found by doing (SAMPLES_PER_SECOND / CLOCK_FREQUENCY) = SAMPLE_PERIOD*/
#define CLOCK_FREQUENCY 14000000	/*the clock frequency, which is 14MHz by default*/

/*the value of PC_GPIO_DIN that indicates the different buttons*/
#define BUTTON_1 0xfe
#define BUTTON_2 0xfd
#define BUTTON_3 0xfb
#define BUTTON_4 0xf7
#define BUTTON_5 0xef
#define BUTTON_6 0xdf
#define BUTTON_7 0xbf
#define BUTTON_8 0x7f

/*the value of PA_GPIO_DOUT that sets only the LED defined on*/
#define LED_1 0x0100
#define LED_2 0x0200
#define LED_3 0x0400
#define LED_4 0x0800

/*define setup functions*/
void setupGPIO(void);
void setupTimer(uint16_t period);
void setupDAC(void);
void setupNVIC(void);

/*define sound functions*/
void on_sample_timer_interrupt(void);
void set_current_song(int song);
void increase_volume(void);
void decrease_volume(void);

/*start and stop timer1 functions*/
void start_timer(void);
void stop_timer(void);

/*button handler functions*/
void onButton1Click(void);
void onButton2Click(void);
void onButton3Click(void);
void onButton4Click(void);
void onButton5Click(void);

/*interrupt handler helper functions*/
void set_deep_sleep_mode(void);
void set_run_mode(void);
void did_click(void);

#endif				/* HEADER_H */
