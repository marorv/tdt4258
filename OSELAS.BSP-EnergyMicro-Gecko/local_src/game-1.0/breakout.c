#include "breakout.h"
#include <unistd.h>
#include <time.h>

#define PI 3.14159265359

#define MIN_ANGLE 3 *  PI / 4
#define MAX_ANGLE PI / 4


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
	if ((int)(ball.x + displacement_x) >= (int)breakout.width || 
		(int)(ball.x + displacement_x) <= 0) {

		ball = hit_wall(breakout, ball);
	} 
	//Hit pad
	else if((int)ball.x >= (int)pad.x && 
			(int)ball.x < (int)(pad.x + pad.length) && 
			(int)ball.y == (int)(pad.y - pad.height) &&
			ball.angle > PI && ball.angle < 2 * PI){
		ball = hit_pad(ball, pad);
	}

	else {
		ball.x += displacement_x;
		ball.y -= displacement_y;
	}
	return ball;
}

Ball move_ball(Ball ball, Frame breakout, Pad pad) {

	drawCircle((int)ball.x, (int)ball.y, 10, 0x0000);

	ball = triangulate(ball, breakout, pad);

	drawCircle((int)ball.x, (int)ball.y, 10, 0xffff);

	return ball;
}

Pad read_input(Pad pad, Frame breakout, char input){

	if (input == 'a' || input == 'd' || input == 's'){
		pad = move_pad(breakout, pad, input);
	}
	return pad;
}

Pad move_pad(Frame breakout, Pad pad, char input) {
	//Control the pad with A and D
	//Check that the pad is still in the image

	fillRect((int)pad.x, (int)pad.y, (int)pad.length, (int)pad.height, 0x0000);

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

	fillRect((int)pad.x, (int)pad.y, (int)pad.length, (int)pad.height, 0xffff);

	return pad;
}
bool ballIsInsideRect(Ball ball, float x, float y, float width, float height) {
	bool xDirectionIsInside = ((ball.x+10) > x && (ball.x+10) < (x+width)) || ((ball.x-10) < (x+width) && (ball.x-10) > x);
	bool yDirectionIsInside = ((ball.y-10) > y && (ball.y-10) < (y+height)) || ((ball.y+10) < (y+height) && (ball.y+10) > y);
	
	return (xDirectionIsInside && yDirectionIsInside);
}

bool isPositiveAndSmallest(float a, float b, float c, float d) {
	return (a >= 0) && (a <= b) && (a <= c) && (a <= d);
}

Ball hit_brick(int x, int y, Brick bricks[], int size_of_bricks, Ball ball){
	int hit_x;
	int hit_y;

	for (int i = 0; i < size_of_bricks; i++) {
		//This only detects the hit from the underside
		if (ballIsInsideRect(ball, bricks[i].x, bricks[i].y, bricks[i].width, bricks[i].height)){
			printf("hit\n");
			hit_x = bricks[i].x;
			hit_y = bricks[i].y;
			
			fillRect((int)bricks[i].x, (int)bricks[i].y, (int)bricks[i].width, (int)bricks[i].height, 0x0000);
			
			//Instead of doing a lot of array-rearranging, just move the block to a negative position, i.e. out of the board
			bricks[i].x = -1;
			bricks[i].y = -1;
			bricks[i].width = 0;
			bricks[i].height = 0;

			//Return new angle the ball is travelling at. For the bricks (and the walls), angle_in = angle_out.
			float bottomDiff = (ball.y-10) - (bricks[i].y + bricks[i].height);
			float topDiff = (ball.y + 10) - (bricks[i].y);
			float rightDiff = (ball.x - 10) - (bricks[i].x + bricks[i].width);
			float leftDiff = (ball.x + 10) - (bricks[i].x);

			bool hitBottom = isPositiveAndSmallest(bottomDiff, topDiff, rightDiff, leftDiff);
			bool hitTop = isPositiveAndSmallest(bottomDiff, topDiff, rightDiff, leftDiff);
			bool hitRight = isPositiveAndSmallest(bottomDiff, topDiff, rightDiff, leftDiff);
			bool hitLeft = isPositiveAndSmallest(bottomDiff, topDiff, rightDiff, leftDiff);

			if (hitBottom) {
				if (ball.angle > 0 && ball.angle < PI) {
					ball.angle = -ball.angle;
				}
			}
			else if (hitTop) {
				if (ball.angle > PI && ball.angle < 2*PI) {
					ball.angle = -ball.angle;
				}
			}
			else if (hitRight) {
				if (ball.angle > (3*PI)/2) {
					ball.angle = PI - ball.angle;
				}
			}
			else if (hitLeft) {
				if (ball.angle > PI/2 && ball.angle < ((3*PI)/2)) {
					ball.angle = PI - ball.angle;
				}
			}

			while (!(ball.angle >= 0 && ball.angle <= 2*PI)) {
				if (ball.angle < 0) {
					ball.angle += 2*PI;
				}
				else if (ball.angle > 2*PI) {
					ball.angle -= 2*PI;
				}
			}
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

	offsetPercent = ((pad.x + ((float)pad.length / 2)) - ball.x)/pad.length;
	out = (PI/2) + (offsetPercent*PI);

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
	if ((int)ball.y - 10 <= 0.0 ){
		ball.angle = -ball.angle;
	}

	//The walls
	else if ((int)ball.x - 10 <= 0.0 || (int)ball.x + 10 >= (int)breakout.width - 1){
		ball = hit_wall(breakout, ball);
	}

	//The pad
	else if ((int)ball.y + 10 >= pad.y - pad.height &&
			((int)ball.x + 10 > pad.x && ball.x - 10 < pad.x + pad.length)
		){
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
				if (bricks[k].x == j && bricks[k].y == i){
					board[i][j] = '[';
					board[i][j+1] = '_';
					board[i][j+2] = ']';
				}
			}

			//Draw the pad
			if (j == (int)pad.x && i == (int)pad.y){
				board[i][j] = '[';
				board[i][j+1] = '=';
				board[i][j+2] = ']';	
			}

			//Draw the ball
			if (j == (int)ball.x && i == (int)ball.y){
				board[i][j] = 'O';	
			}
		}
	}

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void startGame() {

	Ball ball;
	Pad pad;
	Frame breakout;

	breakout.height = SCREEN_HEIGHT;
	breakout.width = SCREEN_WIDTH;
	breakout.gameover = false;

	pad.length = 25;
	pad.height = 3;
	pad.x = (breakout.width-pad.length) / 2 - 1;
	pad.y = breakout.height - pad.height; //Should always be at the bottom, i.e. breakout's height
	

	ball.x = pad.x + (pad.length/2);
	ball.y = breakout.height - 4;
	ball.angle = PI / 2; //This angle should be in radians, because the math functions we use, use radians.

	int brickwidth = 40;
	int bricksPerRow = breakout.width / brickwidth;
	int no_of_bricks = bricksPerRow * 3; //Number of bricks = fill the width of the board * three rows
	
	printf("bricks:%i\n", bricksPerRow);

	Brick bricks [no_of_bricks];
	
	int i;
	for (i = 0; i < no_of_bricks; i++){
		Brick brick;

		brick.height = 10;
		brick.width = 40;

		int col = i % bricksPerRow;
		int row = (int)(i / bricksPerRow);

		int offset = (SCREEN_WIDTH - (bricksPerRow*brick.width))/2;

		printf("i:%i\n", i);
		printf("col:%i\n", col);
		printf("row:%i\n", row);

		brick.x = offset + (col*brick.width);
		brick.y = (row*brick.height);

		bricks[i] = brick;

		fillRect((int)brick.x, (int)brick.y, (int)brick.width, (int)brick.height, 0xffff);
	}

	unsigned int microseconds = 2000;

	for (;;){
		usleep(microseconds);
		ball = move_ball(ball, breakout, pad);
		pad = read_input(pad, breakout, (pad.x < ball.x) ? 'd':'a');
		ball = hit_something(ball, ball.x, ball.y, bricks, breakout, no_of_bricks, pad);
		breakout = hit_floor(breakout, ball);
		//draw_board(breakout, pad, ball, bricks, no_of_bricks);
		if (breakout.gameover){
			printf("Game over\n");
			break;
		}
	}
}
