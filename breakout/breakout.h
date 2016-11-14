#ifndef FILENAME_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

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

Ball triangulate(Ball ball, Frame breakout, Pad pad); 
Ball move_ball(Ball ball, Frame breakout, Pad pad);
Pad move_pad(Frame breakout, Pad pad, char input);
Ball hit_brick(int x, int y, Brick bricks[], int size_of_bricks, Ball ball);
void draw_board(Frame breakout, Pad pad, Ball ball, Brick bricks[], int no_of_bricks);
Ball hit_something(Ball ball, int x, int y, Brick bricks[], Frame breakout, int size_of_bricks, Pad pad);
Pad read_input(Pad, Frame, char);
Ball hit_pad(Ball ball, Pad pad);
Frame gameover(Frame breakout);
Ball hit_wall(Frame breakout, Ball ball);
Frame hit_floor(Frame breakout, Ball ball);

#endif