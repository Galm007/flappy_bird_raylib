#ifndef __BIRD_H__
#define __BIRD_H__

#include <raylib.h>
#include <raymath.h>

typedef struct {
	Vector2 pos;
	float vely, rot;
	bool alive;
	int skin;
} Bird;

extern Texture2D birdtex[9];
extern Sound wingfx, hitfx, diefx;

Bird bird_new();
void bird_free(Bird* bird);

void bird_kill(Bird* bird);
void bird_update(float ft, Bird* bird);
void bird_draw(Bird* bird);

#endif
