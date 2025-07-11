#ifndef __PIPES_H__
#define __PIPES_H__

#include <raylib.h>
#include "bird.h"

#define PIPE_COUNT 8
extern Vector2 pipes[PIPE_COUNT];

void initialize_pipes();
void update_pipes(float ft, Bird* bird, Texture* pipetex);
void draw_pipes(Texture* pipetex);

#endif
