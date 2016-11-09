#include "framebuffer_painter.h"

int main(int argc, char *argv[])
{
	
	setup_framebuffer();
	drawRect(0, 0, 20, 20, 0xffff);
	drawRect(200, 40, 130, 20, 0xffff);

	exit(EXIT_SUCCESS);
}