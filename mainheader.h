/* standard library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* SDL */
#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

/* Physics engine */
#include "include/chipmunk/chipmunk.h"

//Custom struct
typedef struct physics_rect physics_rect;
struct physics_rect {
	SDL_Rect loc_sz;
	cpBody* body;
	struct physics_rect* next; //for linked list
};

physics_rect* rects[512];