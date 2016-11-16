<<<<<<< HEAD
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/fb.h>
#include <sys/mman.h>

#define button1 241;
#define button2 245;

#define BUF_SIZE 1;

int driver_fp;
char buffer[1];


int gamepadInit(void);
void gpio_handler(int signo);
int signalInit(void);
int main(int argc, char *argv[]);



int gamepadInit(){
    driver_fp = open("/dev/gamepad", O_RDONLY);
    if(driver_fp == -1){
        printf("Error opening gamepad driver\n");
        return -1;
    }

    return 0;
}


//Interrupt handler    
void gpio_handler(int signo){
    
    int gpio_reg;
    gpio_reg = read(driver_fp, buffer, 1);
    int button = buffer[0]; // read the value of the button that was pressed
    printf("gpio_handler:%i \n", button);
    switch(button){
    	case (0xfe): 
    	printf("Button sw1 pressed\n");
    	break;
    	case (0xfb): 
    	printf("Button sw3 pressed\n");
    	break;
    	
    	}        
}

int signalInit(){
    int oflags;
    int errno;
    printf("test%i \n", SIGIO);
    if (signal(SIGIO, &gpio_handler) == SIG_ERR){
        printf("Error on signal setup\n");
        return -1;
    }    
    
    errno = fcntl(driver_fp, F_SETOWN, getpid());
    printf("getpid%i \n", getpid());
    
    oflags = fcntl(driver_fp, F_GETFL);

    errno = fcntl(driver_fp, F_SETFL, oflags | FASYNC);
    
    return 0;

}

int main(int argc, char *argv[])
{
	if (gamepadInit() < 0){
        printf("Error initializing game \n");
        return -1;
        }
	if (signalInit() < 0){
        printf("Error initializing signal \n");
    }
    printf("Initializing comnplete!\n");
	
	while(1);
	

	exit(EXIT_SUCCESS);
}
