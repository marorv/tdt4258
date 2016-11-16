#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>	
#include <math.h>
#include "framebuffer_painter.h"

/*function to refresh the pixels inside a rectangle defined by x y width height*/
void refresh_framebuffer_rect (int x, int y, int width, int height) {

	struct fb_copyarea rect;
	if (x - 1 > 0) {
		x -= 1;
		width += 1;
	}
	if (y - 1 > 0) {
		y -= 1;
		height += 1;
	}
	if (x + width + 1 < SCREEN_WIDTH) {
		width += 1;
	}
	if (y + height + 1 < SCREEN_HEIGHT) {
		height += 1;
	}

    rect.dx = x;
    rect.dy = y;
    rect.width = width;
    rect.height = height;

    ioctl(fb0file, 0x4680, &rect);
}

/*setup a memory map to the fb0 device and link it to the pointer framebuffer, located in the header file*/
void setup_framebuffer () {

	fb0file = open("/dev/fb0", O_RDWR, 0);
    assert(fb0file != -1);
    framebuffer = mmap(0, 320*240, PROT_READ | PROT_WRITE, MAP_SHARED, fb0file, 0);
    assert(framebuffer != MAP_FAILED);
}

/*function that fills up a rectangle with a specified color*/
void fillRect(int x, int y, int width, int height, int color) {

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
	for (i = 0; i <= height; i++)
	{
		int j;
		for (j = 0; j <= width; j++)
		{
			framebuffer[totalOffset + (i*SCREEN_WIDTH) + (j)] = color;
		}
	}

	/*refresh only the area that this rect habits*/
	refresh_framebuffer_rect(x, y, width+1, height+1);
}

/*set the color of the specified pixel (does not update the screen)*/
void setPixel (int x, int y, int color) {
	if (x < 0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT) {
		return;
	}

	int heightOffset = (y*SCREEN_WIDTH);
	int widthOffset = (x);
	int totalOffset = heightOffset + widthOffset;

	framebuffer[totalOffset] = color;
}

/*function that only draws horizontal lines, more energy efficient than the drawLine function*/
void drawHorizontalLine(int x, int y, int length, int color) {
	int i;
	for (i = 0; i<=length; i++) {
		setPixel(x+i, y, color);
	}
	refresh_framebuffer_rect(x, y, length, 1);
}

void drawCircle (int x0, int y0, int radius, int color) {
	int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        setPixel(x0 + x, y0 + y, color);
        setPixel(x0 + y, y0 + x, color);
        setPixel(x0 - y, y0 + x, color);
        setPixel(x0 - x, y0 + y, color);
        setPixel(x0 - x, y0 - y, color);
        setPixel(x0 - y, y0 - x, color);
        setPixel(x0 + y, y0 - x, color);
        setPixel(x0 + x, y0 - y, color);

        y += 1;
        err += 1 + 2*y;
        if (2*(err-x) + 1 > 0)
        {
            x -= 1;
            err += 1 - 2*x;
        }
    }
    refresh_framebuffer_rect(x0-radius, y0-radius, 2*radius + 1, 2*radius + 1);
}

/*function that only draws vertical lines, more energy efficient than the drawLine function*/
void drawVerticalLine(int x, int y, int length, int color) {
	int i;
	for (i = 0; i<=length; i++) {
		setPixel(x, y+i, color);
	}
	refresh_framebuffer_rect(x, y, 1, length);
}
/*function that can draw lines in any direction using Bresenhams algorithm*/
void drawLine(int x0, int y0, int x1, int y1, int color) {
	int startx = (x0 < x1) ? x0: x1;
	int starty = (y0 < y1) ? y0: y1;
	int endx = (x0 >= x1) ? x0: x1;
	int endy = (y0 >= y1) ? y0: y1;

	int dx;
	int dy;
	int err = (dx > dy ? dx: -dy)/2, e2;
	int sx = (x0 < x1) ? 1: -1;
	int sy = (y0 < y1) ? 1: -1;
	if (x0 >= x1) {
		dx = x0 - x1;
	}
	else {
		dx = x1 - x0;
	}
	if (y0 >= y1) {
		dy = y0 - y1;
	}
	else {
		dy = y1 - y0;
	}

	/*continue loop forever*/
	for(;;){
		/*if the end point is meet, break the loop*/
		if (x0 >= x1 || y0 >= y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
		setPixel(x0, y0, color);
	}

	refresh_framebuffer_rect(startx, starty, endx-startx+1, endy-starty+1);
}