#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <linux/fb.h>

#ifndef __framebuffer_h__
#define __framebuffer_h__

#define SCREEN_WIDTH 319
#define SCREEN_HEIGHT 239

int fb0file;
uint16_t *framebuffer;

void refresh_framebuffer_rect(int x, int y, int width, int height);
void setup_framebuffer(void);

void fillRect(int x, int y, int width, int height, int color);
void drawLine(int x0, int y0, int x1, int y1, int color);
void drawHorizontalLine(int x, int y, int length, int color);
void drawVerticalLine(int x, int y, int length, int color);
void setPixel (int x, int y, int color);
void drawCircle (int x0, int y0, int radius, int color);

#endif