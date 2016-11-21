#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "framebuffer_painter.h"

typedef struct{
	float x;
	float y;
	float angle; //This angle should be in radians, because the math functions we use, use radians.
} Ball;

typedef struct {
	float x;
	float y;
	float length;
	float height;
} Pad;

typedef struct {
	float height;
	float width;
	bool gameover;
} Frame;

typedef struct {
	float x;
	float y;
	float width;
	float height;
} Brick;

Pad pad;

Pad read_input(Pad, Frame, char);
Pad move_pad(Frame breakout, Pad pad, char input);
Ball triangulate(Ball ball, Frame breakout, Pad pad); 
Ball move_ball(Ball ball, Frame breakout, Pad pad);
Ball hit_brick(int x, int y, Brick bricks[], int size_of_bricks, Ball ball);
Ball hit_something(Ball ball, int x, int y, Brick bricks[], Frame breakout, int size_of_bricks, Pad pad);
Ball hit_pad(Ball ball, Pad pad);
Ball hit_wall(Frame breakout, Ball ball);
void startGame(void);
void onLeftButtonClicked(void);
void onRightButtonClicked(void);
Frame gameover(Frame breakout);
Frame hit_floor(Frame breakout, Ball ball);

#endif
