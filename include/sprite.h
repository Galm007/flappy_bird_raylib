#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <raylib.h>

void draw_sprite(Texture2D* tex, Vector2 pos, float rot, bool fliph);
Rectangle get_collider(Vector2 pos, Texture2D* tex);
bool is_colliding(Rectangle r1, Rectangle r2);
void render_score(Texture2D numbertex[10]);

#endif
