#ifndef __graphic_engine_h__
#define __graphic_engine_h__

int backgroundColor;

typedef enum {black, white} COLOR;

struct game_object {
	int x;
	int y;
	int width;
	int height;
	int color;
}

void addObject(struct game_object *obj);
void moveObject(struct game_object *obj);
void removeObject(struct game_object *obj);

void setBackgroundColor(int color);
void setBackgroundColor(COLOR color);

#endif