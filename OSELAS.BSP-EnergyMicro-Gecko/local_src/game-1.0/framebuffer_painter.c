#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>	
#include "framebuffer_painter.h"

void refresh_framebuffer_rect (int x, int y, int width, int height) {

	struct fb_copyarea rect;
    rect.dx = x;
    rect.dy = y;
    rect.width = width;
    rect.height = height;

    ioctl(fb0file, 0x4680, &rect);
}

void setup_framebuffer () {

	fb0file = open("/dev/fb0", O_RDWR, 0);
    assert(fb0file != -1);
    framebuffer = mmap(0, 320*240, PROT_READ | PROT_WRITE, MAP_SHARED, fb0file, 0);
    assert(framebuffer != MAP_FAILED);

}

void drawRect(int x, int y, int width, int height, int color) {

	/*if whole the rect is outside the screen, nothing needs to be drawn*/
	if (x > SCREEN_WIDTH) {
		return;
	}
	/*if whole the rect is outside the screen, nothing needs to be drawn*/
	if (y > SCREEN_HEIGHT) {
		return;
	}

	/*if only some of the rect is outside the screen in x direction, chop the width so that everything is inside*/
	if (x + width > SCREEN_WIDTH) {
		width -= x + width - SCREEN_WIDTH;
	}
	/*if only some of the rect is outside the screen in y direction, chop the height so that everything is inside*/
	if (y + height > SCREEN_HEIGHT) {
		height -= y + height - SCREEN_HEIGHT;
	}

	int heightOffset = (y*SCREEN_WIDTH);
	int widthOffset = (x);
	int totalOffset = heightOffset + widthOffset;

	int i;
	for (i = 0; i < height; i++)
	{
		int j;
		for (j = 0; j < width; j++)
		{
			framebuffer[totalOffset + (i*SCREEN_WIDTH) + (j)] = color;
		}
	}

	/*refresh only the area that this rect habits*/
	refresh_framebuffer_rect(x, y, width, height);
}