#include <iostream>
#include <cmath>
#include "breakout.h"
#include <unistd.h>
#include <string>
#include <string.h>

#define PI 3.14159265359

#define MIN_ANGLE 3 *  PI / 4
#define MAX_ANGLE PI / 4

using namespace std;

struct Ball {
	float x;
	float y;
	float angle; //This angle should be in radians, because the math functions we use, use radians.
};

struct Pad {
	float x;
	float y;
	float length;
	float height;
};

struct Frame {
	float height;
	float width;
	bool gameover;
};

struct Brick {
	float x;
	float y;
	float width;
	float height;
};

Ball triangulate(Ball ball, Frame breakout, Pad pad) {

	float angle = ball.angle;

	//I fucking hate it when the angle goes above PI or under 0, fix it to 0 < angle < PI;
	while(angle > 2 * PI){
		angle -= 2 * PI;
	}
	while (angle < 0){
		angle += 2 * PI;
	}

	//We always want to move the distance 1 of the hypotenuse --> h = 1
	float h = 1.0;

	float displacement_y = h * sin(angle); 
	float displacement_x = h * cos(angle); 

	//Hit wall
	if ((int)(ball.x + displacement_x) >= (int)breakout.width or 
		(int)(ball.x + displacement_x) <= 0) {

		ball = hit_wall(breakout, ball);
	} 
	//Hit pad
	else if((int)ball.x >= (int)pad.x and 
			(int)ball.x < (int)(pad.x + pad.length) and 
			(int)ball.y == (int)(pad.y - pad.height) and
			ball.angle > PI and ball.angle < 2 * PI){
		ball = hit_pad(ball, pad);
	}

	else {
		ball.x += displacement_x;
		ball.y -= displacement_y;
	}
	return ball;
}

Ball move_ball(Ball ball, Frame breakout, Pad pad) {

	ball = triangulate(ball, breakout, pad);

	return ball;
}

Pad read_input(Pad pad, Frame breakout, char input){

	if (input == 'a' or input == 'd' or input == 's'){
		pad = move_pad(breakout, pad, input);
	}
	return pad;
}

Pad move_pad(Frame breakout, Pad pad, char input) {
	//Control the pad with A and D
	//Check that the pad is still in the image
	if (input == 'a'){
		if (pad.x - 1 > 0){
			pad.x -= 1;
		}
	}
	else if (input == 'd'){
		if (pad.x + 1 < breakout.width - pad.length){
			pad.x += 1;
		}
	}
	else if (input == 's'){
		pad.x = pad.x;
	}

	return pad;
}

Ball hit_brick(int x, int y, Brick bricks[], int size_of_bricks, Ball ball){
	int hit_x;
	int hit_y;

	for (int i = 0; i < size_of_bricks; i++) {
		//This only detects the hit from the underside
		if (bricks[i].x <= x and bricks[i].x + bricks[i].width > x and y == bricks[i].y + 1){

			hit_x = bricks[i].x;
			hit_y = bricks[i].y;
			//Instead of doing a lot of array-rearranging, just move the block to a negative position, i.e. out of the board
			bricks[i].x = -1;
			bricks[i].y = -1;
			
			//Return new angle the ball is travelling at. For the bricks (and the walls), angle_in = angle_out.

			ball.angle = -ball.angle;
		}
		//Detection from the sides
		else if(bricks[i].y <= y and bricks[i].y + bricks[i].height > y and x == bricks[i].x ){

			hit_x = bricks[i].x;
			hit_y = bricks[i].y;
			bricks[i].x = -1;
			bricks[i].y = -1;
			
			ball.angle = PI - ball.angle;
		}
	}
	return ball;
}

Frame gameover(Frame breakout){
	breakout.gameover = true;
	return breakout;
}

Ball hit_pad(Ball ball, Pad pad){

	float out; 
	float offset;
	float percentage;

	out = 2 * PI - ball.angle;

	//Hit the middle of the pad
	if (ball.x == pad.x + (float)(pad.length / 2)) {
		ball.angle = -ball.angle;

	} 
	//Hit somewhere else on the pad: out = pi-in+percentage of how far out on the pad we hit
	else {
		offset = (pad.x + ((float)pad.length / 2)) - ball.x;
		out = 2 * PI - ball.angle + offset / (float)pad.length * PI;
	}
	
	//I still hate it when the angle goes above PI;
	while(out > 2 * PI){
		out -= 2 * PI;
	}
	while(out < 0){
		out += 2 * PI;
	}

	//Ball should always be between PI/4, 3*PI/4 when returned from the pad
	if (out > PI) {
		out = MAX_ANGLE;
	}
	else if (out < 0) {
		out = MIN_ANGLE;
	}

	ball.angle = out;
	return ball;
}

Frame hit_floor(Frame breakout, Ball ball){

	if ((int)ball.y > (int)(breakout.height - 1.0)){
		breakout = gameover(breakout);
	}
	return breakout;
}

Ball hit_wall(Frame breakout, Ball ball){
	
	ball.angle = PI - ball.angle;
	return ball;
}

Ball hit_something(Ball ball, int x, int y, Brick bricks[], Frame breakout, int size_of_bricks, Pad pad){
	
	//Bricks
	ball = hit_brick(x, y, bricks, size_of_bricks, ball);
	
	//Roof
	if ((int)ball.y == 0.0 ){
		ball.angle = -ball.angle;
	}

	//The walls
	else if ((int)ball.x == 0.0 or (int)ball.x == (int)breakout.width - 1){
		ball = hit_wall(breakout, ball);
	}

	//The pad
	else if ((int)ball.x >= (int)pad.x and 
			(int)ball.x < (int)(pad.x + pad.length) and 
			(int)ball.y == (int)(pad.y - pad.height)){
		ball = hit_pad(ball, pad);
	}

	return ball;
}

void draw_board(Frame breakout, Pad pad, Ball ball, Brick bricks[], int no_of_bricks) {
	int width = (int)breakout.width;
	int height = (int)breakout.height;

	/* 
		   (0, 0)____________ (0, width-1)
				|[_][_][_][_]|
				|            | 
				|     o      |
				|____[=]_____|
	(height-1,0)          	  (height-1, width-1)
	*/

	char board [height][width];
	//height = rows = y = i
	//width  = cols = x = j

	//Fill the board with spaces
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			board[i][j] = ' ';
		}
	}

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			//Draw the bricks
			for (int k = 0; k < no_of_bricks; k++){
				if (bricks[k].x == j and bricks[k].y == i){
					board[i][j] = '[';
					board[i][j+1] = '_';
					board[i][j+2] = ']';
				}
			}
			//Draw the pad
			if (j == (int)pad.x and i == (int)pad.y){
				board[i][j] = '[';
				board[i][j+1] = '=';
				board[i][j+2] = ']';	
			}
			//Draw the ball
			if (j == (int)ball.x and i == (int)ball.y){
				board[i][j] = 'O';	
			}
		}
	}

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

}

int main() {

	struct Ball ball;
	struct Pad pad;
	struct Frame breakout;

	breakout.height = 15;
	breakout.width = 21;
	breakout.gameover = false;

	pad.x = breakout.width / 2 - 1;
	pad.y = breakout.height - 1; //Should always be at the bottom, i.e. breakout's height
	pad.length = 3;
	pad.height = 1;

	ball.x = pad.x + 1;
	ball.y = breakout.height - pad.height - 1;
	ball.angle = PI / 2; //This angle should be in radians, because the math functions we use, use radians.

	int brickwidth = 3;
	int no_of_bricks = breakout.width / brickwidth * 3; //Number of bricks = fill the width of the board * three rows
	Brick bricks [no_of_bricks];
	
	for (int i = 0; i < no_of_bricks; i++){
		Brick brick;
		brick.x = i * 3 % no_of_bricks;
		brick.y = int (i / (breakout.width / brickwidth));
		brick.height = 1;
		brick.width = 3;

		bricks[i] = brick;
	}

	unsigned int microseconds = 50000;
	char gamestring[200] = "ssssssssssssasssssssssssssssssddddddddsssssssssssssssssssssssssssssaaaaaaaasssssssssssssssssssssssssssssssssssssssddddd";


	for (int i = 0; i < 200; i++){
		ball = move_ball(ball, breakout, pad);
		pad = read_input(pad, breakout, gamestring[i]);
		usleep(microseconds);
		ball = hit_something(ball, ball.x, ball.y, bricks, breakout, no_of_bricks, pad);
		breakout = hit_floor(breakout, ball);
		draw_board(breakout, pad, ball, bricks, no_of_bricks);
		if (breakout.gameover){
			cout << endl << "Game over" << endl;
			break;
		}
	}


	return 0;
}