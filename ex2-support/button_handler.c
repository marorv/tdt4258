#include <stdint.h>
#include "header.h"
#include "efm32gg.h"
int current_led = 1;

int get_led(int lednr){
	switch (lednr){
	 case(1):
	 	return LED_1;
	 case(2):
	 	return LED_2;
	 case(3):
	 	return LED_3;
	 case(4):
	 	return LED_4;
	 
	}
	return -1;
}

void onButton1Click(){
	if (current_led > 1){
		current_led--;
		*GPIO_PA_DOUT = ~get_led(current_led);
		stop_timer();
	}
}

void onButton2Click(){
	increase_volume();
	
}

void onButton3Click(){
	if (current_led < 4){
		current_led++;
		*GPIO_PA_DOUT =  ~get_led(current_led);
		stop_timer();
	}
	
}

void onButton4Click(){
	decrease_volume();
}

void onButton5Click(){
	set_current_song(current_led);
	start_timer()
}
