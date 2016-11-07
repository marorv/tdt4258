#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <linux/fb.h>

#ifndef __framebuffer_h__
#define __framebuffer_h__

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

int fb0file;
uint16_t *framebuffer;

void refresh_framebuffer_rect(int x, int y, int width, int height);
void setup_framebuffer(void);
void drawRect(int x, int y, int width, int height, int color);


#endif