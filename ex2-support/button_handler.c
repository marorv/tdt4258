#include <stdint.h>
#include "header.h"
#include "efm32gg.h"

int current_led = 1; /*variable keeping track of which led light is currently on*/

/*function that takes in a led number, and returns the hex value that sets this light to be off*/
int get_led(int lednr){
	switch (lednr){ /*switch on the lednr*/
	 case(1): /*if lednr = 1*/
	 	return LED_1; /*return hex value for LED_1*/
	 case(2):
	 	return LED_2;
	 case(3):
	 	return LED_3;
	 case(4):
	 	return LED_4;
	 
	}
	return -1; /*defualt value (should not be returned)*/
}

/*button 1 was clicked*/
void onButton1Click(){
	/*check if the current led is at the start position, and if not, set it to the previous led light*/
	if (current_led > 1){
		current_led--; /*decrement the current_led variable*/
		*GPIO_PA_DOUT = ~get_led(current_led); /*set the GPIO_PA_DOUT register to turn on the light that is current led*/
		stop_timer(); /*stop the timer to stop the music currently playing*/
	}
}

/*called when button 2 is clicked*/
void onButton2Click(){
	increase_volume();
	
}

/*called when button 3 is clicked*/
void onButton3Click(){
	/*check if current_led is at the end position, and if it is not set it to the next led light*/
	if (current_led < 4){
		current_led++; /*increment the current_led variable*/
		*GPIO_PA_DOUT =  ~get_led(current_led); /*set the GPIO_PA_DOUT register to turn on the light that is current led*/
		stop_timer(); /*stop the timer to stop the music currently playing*/
	}
}

/*called when button 4 is clicked*/
void onButton4Click(){
	decrease_volume();
}

/*called when button 5 is clicked*/
void onButton5Click() {
	/*this button starts playing the the song corresponding to the current led light*/
	set_run_mode(); /*set the energy mode to run mode (required for sample timer)*/
	set_current_song(current_led); /*set current song to the song of the current led light*/
	start_timer(); /*start the timer1, so that the song starts playing. */
}
