#include "constants.h"
#include "game_state.h"
#include "sprite.h"
#include "bird.h"
#include "pipes.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

enum GameState state = GET_READY;
float flash_alpha = 0.0f;
float transition_time = 0.0f;
float point_timer = 0.0f;
unsigned int score = 0;
bool flap_key_presssed = false;
int bg_skin = 0;

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
	InitAudioDevice();
	SetTargetFPS(60);

	// load textures
	Texture2D bgtex[2] = {
		LoadTexture("../assets/sprites/background-day.png"),
		LoadTexture("../assets/sprites/background-night.png")
	};
	Texture2D pipetex[2] = {
		LoadTexture("../assets/sprites/pipe-green.png"),
		LoadTexture("../assets/sprites/pipe-red.png")
	};
	Texture2D groundtex = LoadTexture("../assets/sprites/base.png");
	Texture2D gameovertex = LoadTexture("../assets/sprites/gameover.png");
	Texture2D getreadytex = LoadTexture("../assets/sprites/message.png");
	Texture2D numbertex[10];
	{
		char filename[64];
		for (int i = 0; i < 10; i++) {
			sprintf(filename, "../assets/sprites/%d.png", i);
			numbertex[i] = LoadTexture(filename);
		}
	}
	birdtex[0] = LoadTexture("../assets/sprites/yellowbird-upflap.png");
	birdtex[1] = LoadTexture("../assets/sprites/yellowbird-midflap.png");
	birdtex[2] = LoadTexture("../assets/sprites/yellowbird-downflap.png");
	birdtex[3] = LoadTexture("../assets/sprites/bluebird-upflap.png");
	birdtex[4] = LoadTexture("../assets/sprites/bluebird-midflap.png");
	birdtex[5] = LoadTexture("../assets/sprites/bluebird-downflap.png");
	birdtex[6] = LoadTexture("../assets/sprites/redbird-upflap.png");
	birdtex[7] = LoadTexture("../assets/sprites/redbird-midflap.png");
	birdtex[8] = LoadTexture("../assets/sprites/redbird-downflap.png");

	// load sound effects
	Sound pointfx = LoadSound("../assets/audio/point.ogg");
	Sound swooshfx = LoadSound("../assets/audio/swoosh.ogg");
	wingfx = LoadSound("../assets/audio/wing.ogg");
	hitfx = LoadSound("../assets/audio/hit.ogg");
	diefx = LoadSound("../assets/audio/die.ogg");

	// generate white flash texture
	Texture2D whitetex;
	{
		Image whiteimg = LoadImageFromTexture(bgtex[0]);
		ImageClearBackground(&whiteimg, WHITE);
		whitetex = LoadTextureFromImage(whiteimg);
		UnloadImage(whiteimg);
	}

	// initialize entities
	initialize_pipes();
	Bird bird = bird_new();

	// the time between each awarded point is constant
	const float point_interval = PIPE_SPREAD / SCROLL_SPEED;

	while (!WindowShouldClose()) {
		float ft = GetFrameTime();

		// input handling
		flap_key_presssed = IsKeyPressed(KEY_SPACE)
			|| IsKeyPressed(KEY_UP)
			|| IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

		// death flash
		flash_alpha -= ft;
		if (flash_alpha < 0.0f)
			flash_alpha = 0.0f;

		// state management
		if (state == TRANSITION) {
			transition_time -= ft;
			if (transition_time <= 0.0f)
				state = GAME_OVER;
			PlaySound(swooshfx);
		}
		if (flap_key_presssed) {
			if (state == GAME_OVER) {
				state = GET_READY;
				bird.pos.y = SCREEN_HEIGHT / 2.0f;
				bird.rot = 0.0f;
				bird.alive = true;
				bird.skin = (bird.skin + 1) % 3;
				bg_skin = !bg_skin;
				initialize_pipes();
			}
			else if (state == GET_READY) {
				state = PLAYING;
				score = 0;
				point_timer = point_interval;
			}
		}

		static float bg_posx = 0.0f;
		static float ground_posx = 0.0f;
		if (state == PLAYING || state == GET_READY) {
			// bg movement
			bg_posx -= SCROLL_SPEED * ft * 0.5f;
			if (bg_posx < -SCREEN_WIDTH)
				bg_posx += SCREEN_WIDTH;

			// ground movement
			ground_posx -= SCROLL_SPEED * ft;
			if (ground_posx < -SCREEN_WIDTH)
				ground_posx += SCREEN_WIDTH;
		}

		// move bird
		bird_update(ft, &bird);

		if (state == PLAYING)
		{
			// point timer
			point_timer -= ft;
			if (point_timer <= 0.0f) {
				point_timer += point_interval;
				score++;
				PlaySound(pointfx);
			}
			
			// get bird hitbox
			Rectangle bird_col = get_collider(bird.pos, &birdtex[0]);

			// move pipes and check for bird-pipe collision
			update_pipes(ft, &bird, &pipetex[bg_skin]);

			// kill bird if it touched ground
			if (bird.alive && bird_col.y + bird_col.height / 2.0f > SCREEN_HEIGHT - GROUND_HEIGHT)
				bird_kill(&bird);
		}

		BeginDrawing();
		ClearBackground(BLACK);

		// draw background
		DrawTexture(bgtex[bg_skin], bg_posx, 0.0f, WHITE);
		DrawTexture(bgtex[bg_skin], bg_posx + bgtex[0].width, 0.0f, WHITE);

		// draw pipes
		draw_pipes(&pipetex[bg_skin]);

		// draw ground
		const float groundy = SCREEN_HEIGHT - GROUND_HEIGHT;
		DrawTexture(groundtex, ground_posx, groundy, WHITE);
		DrawTexture(groundtex, ground_posx + groundtex.width, groundy, WHITE);

		// draw bird
		bird_draw(&bird);

		if (state != GET_READY)
			render_score(numbertex);

		// draw UI
		float y_offset = transition_time * SCREEN_HEIGHT * 4.0f;
		if (state == TRANSITION || state == GAME_OVER)
			DrawTexture(
				gameovertex,
				(SCREEN_WIDTH - gameovertex.width) / 2.0f,
				(SCREEN_HEIGHT - gameovertex.height) / 2.0f + y_offset,
				WHITE);
		else if (state == GET_READY)
			DrawTexture(
				getreadytex,
				(SCREEN_WIDTH - getreadytex.width) / 2.0f,
				(SCREEN_HEIGHT - getreadytex.height) / 2.0f,
				WHITE);

		// death flash
		DrawTexture(whitetex, 0, 0, ColorAlpha(WHITE, flash_alpha));

		EndDrawing();
	}

	bird_free(&bird);

	UnloadSound(pointfx);
	UnloadSound(swooshfx);
	UnloadSound(wingfx);
	UnloadSound(hitfx);
	UnloadSound(diefx);

	UnloadTexture(bgtex[0]);
	UnloadTexture(bgtex[1]);
	UnloadTexture(pipetex[0]);
	UnloadTexture(pipetex[1]);
	UnloadTexture(groundtex);
	UnloadTexture(whitetex);
	UnloadTexture(gameovertex);
	UnloadTexture(getreadytex);
	for (int i = 0; i < 9; i++)
		UnloadTexture(birdtex[i]);

	CloseAudioDevice();
	CloseWindow();
	return 0;
}
