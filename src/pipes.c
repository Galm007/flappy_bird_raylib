#include "pipes.h"
#include "constants.h"
#include "sprite.h"

Vector2 pipes[PIPE_COUNT];

void initialize_pipes() {
	for (int i = 0; i < PIPE_COUNT; i++)
		pipes[i] = (Vector2) {
			SCREEN_WIDTH + 50.0f + PIPE_SPREAD * i,
			GetRandomValue(PIPE_GAP + 40.0f, SCREEN_HEIGHT - GROUND_HEIGHT - PIPE_GAP - 40.0f),
		};
}

void update_pipes(float ft, Bird* bird, Texture* pipetex) {
	Rectangle bird_col = get_collider(bird->pos, &birdtex[0]);

	for (int i = 0; i < PIPE_COUNT; i++) {
		// move pipes
		pipes[i].x -= SCROLL_SPEED * ft;
		if (pipes[i].x < -pipetex->width / 2.0f)
			pipes[i].x += PIPE_SPREAD * PIPE_COUNT;

		// calculate pipe hitboxes
		Rectangle pipe_col1 = get_collider(
			(Vector2) {
				pipes[i].x,
				pipes[i].y + PIPE_GAP + pipetex->height / 2.0f,
			},
			pipetex);
		Rectangle pipe_col2 = get_collider(
			(Vector2) {
				pipes[i].x,
				pipes[i].y - PIPE_GAP - pipetex->height / 2.0f,
			},
			pipetex);

		// collision detection
		if (bird->alive
			&& (is_colliding(bird_col, pipe_col1) || is_colliding(bird_col, pipe_col2)))
			bird_kill(bird);
	}
}

void draw_pipes(Texture* pipetex) {
	// NOTE: the total gap between two pipes is PIPE_GAP * 2
	for (int i = 0; i < PIPE_COUNT; i++) {
		draw_sprite(
			pipetex,
			(Vector2) {
				pipes[i].x,
				pipes[i].y + PIPE_GAP + pipetex->height / 2.0f,
			},
			0.0f,
			false);
		draw_sprite(
			pipetex,
			(Vector2) {
				pipes[i].x,
				pipes[i].y - PIPE_GAP - pipetex->height / 2.0f,
			},
			180.0f,
			true);
	}
}
