#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <stdbool.h>

enum GameState { GET_READY, PLAYING, GAME_OVER, TRANSITION };

extern enum GameState state;
extern float flash_alpha;
extern float transition_time;
extern unsigned int score;
extern bool flap_key_presssed;

#endif
