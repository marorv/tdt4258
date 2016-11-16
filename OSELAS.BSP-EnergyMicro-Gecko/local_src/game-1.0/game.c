#include "framebuffer_painter.h"
#include "breakout.h"
#include <signal.h>

void test();

int main(int argc, char *argv[])
{
	/*setup up fb0, memorymapping the fb0 dev to frambuffer variable in framebuffer_painter.h*/
	setup_framebuffer();

	/*clear the screen*/
	fillRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT, 0x0000);

	startGame();

	signal(SIGINT, test);

	exit(EXIT_SUCCESS);
}