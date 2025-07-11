#include "bird.h"
#include "constants.h"
#include "game_state.h"
#include "sprite.h"

#include <raylib.h>

#define ANIMATION_FRAME_TIME 0.1

int animation_frame = 0;
Texture2D birdtex[9];
Sound wingfx, hitfx, diefx;

// TODO: 
// - Turn bird.tex into a non-member of Bird

Bird bird_new() {
	Bird bird = {
		.pos = (Vector2) { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
		.vely = 0.0f,
		.rot = 0.0f,
		.alive = true,
		.skin = GetRandomValue(0, 2),
	};

	return bird;
}

void bird_free(Bird* bird) {
	bird->pos = Vector2Zero();
	bird->vely = bird->rot = 0.0f;
	bird->alive = false;
}

void bird_kill(Bird* bird) {
	bird->alive = false;
	bird->vely = -JUMP_FORCE * 0.8f;
	flash_alpha = 1.0f;
	state = TRANSITION;
	transition_time = TRANSITION_TIME;
	PlaySound(hitfx);
	PlaySound(diefx);
}

void bird_update(float ft, Bird* bird) {
	// flap
	if (bird->alive && flap_key_presssed) {
		bird->vely = -JUMP_FORCE;

		if (state == PLAYING)
			PlaySound(wingfx);
	}

	// gravity
	if (state == PLAYING || state == TRANSITION) {
		bird->vely += GRAVITY * ft;
		bird->pos.y += bird->vely * ft;
		bird->rot = bird->vely * 0.05f;
	}

	// keep the bird on the ground when dead
	if (!bird->alive && bird->pos.y > SCREEN_HEIGHT - GROUND_HEIGHT) {
		bird->pos.y = SCREEN_HEIGHT - GROUND_HEIGHT;
		bird->rot = 45.0f;
	}

	// animation
	if (state == PLAYING) {
		static float animation_timer = 0.0f;
		animation_timer -= ft;
		while (animation_timer <= 0.0f) {
			animation_timer += ANIMATION_FRAME_TIME;
			animation_frame = (animation_frame + 1) % 3;
		}
	}
}

void bird_draw(Bird* bird) {
	draw_sprite(&birdtex[animation_frame + 3 * bird->skin], bird->pos, bird->rot, false);
}
