#ifndef FILENAME_H

#include <iostream>
#include <cmath>

struct Ball;
struct Pad;
struct Frame;
struct Brick;

Ball triangulate(Ball, Frame, Pad); 
Ball move_ball(Ball, Frame, Pad);
float radians(float);
Pad move_pad(Frame, Pad, char);
float hit_brick(int, int, Brick, int, Ball);
void draw_board(Frame, Pad, Ball, Brick);
Ball hit_something(Ball, int, int, Brick, Frame, int, Pad);
Pad read_input(Pad, Frame, char);
Ball hit_pad(Ball, Pad);
Frame gameover();
Ball hit_wall(Frame, Ball);

#endif