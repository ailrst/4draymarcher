
/**
 * File for common shit used across files that doesnt have something. 
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <stdlib.h>

#include "colours.h"
#include "types.h"
#include "vect.h"
#include "math.h"
#include "queue.h"

#ifndef MIAINH
#define MIAINH

extern struct object white_sphere;

extern SDL_Renderer *ren;  

void sdlb_set_colour(struct colour col); 

void sdlb_draw_col_pixel(struct colour col, int x, int y);

extern struct object scene_object;
extern int keyboardstate[322];  // 322 is the number of SDLK_DOWN events
extern struct camera *camera;
#endif
