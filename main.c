#include "mainheader.h"
#define INIT_RECTS 5

SDL_Window* window; SDL_Renderer* renderer;
cpSpace* global_space = NULL;
SDL_Texture* grey_color = NULL;
int current = INIT_RECTS - 1;

//function prototypes
extern physics_rect* Create_PHY_Rect(cpVect position, cpVect size, float mass);
extern void Update_PHY_Rect(physics_rect* phy_rect);

int main(void) {//main is a macro that should init SDL
	window = SDL_CreateWindow("Lutemo", 0, 100, 800, 800, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//init chipmunk space
	global_space = cpSpaceNew();
	cpSpaceSetGravity(global_space, cpv(0, 20));
	//init ground
	cpShape* ground = cpSpaceAddShape(global_space, cpSegmentShapeNew(cpSpaceGetStaticBody(global_space),cpv(0,800),cpv(800,800),0));
	//init grey color texture
	grey_color = SDL_CreateTextureFromSurface(renderer, SDL_CreateRGBSurfaceWithFormatFrom(
		&((char[3]) { 125, 125, 125 }), 1, 1, 24, 3, SDL_PIXELFORMAT_RGB24));
	//add inital rects
	for (int i = 0; i < 512; ++i)
		rects[i] = NULL;
    for (int i = 0; i < INIT_RECTS; ++i) {
		rects[i] = Create_PHY_Rect(cpv(i * 75, 0), cpv(50, 50), 5);
	}
	SDL_Event events;
	Uint8* keyboard = SDL_GetKeyboardState(NULL);
	while (1) {
		while (SDL_PollEvent(&events))
			if (events.type == SDL_WINDOWEVENT)
				if (events.window.event == SDL_WINDOWEVENT_CLOSE)
					return 0;
		int tmp_x = 0, tmp_y = 0;
		if (SDL_GetMouseState(&tmp_x, &tmp_y) & SDL_BUTTON_LMASK != 0) {
			if (current < 511)
				current += 1;
			else
				current = -1;
			rects[current] = Create_PHY_Rect(cpv(tmp_x, tmp_y), cpv(50, 50), 5);
		}
		if (keyboard[SDL_SCANCODE_C]) {
			for (int i = 0; i < 512; ++i) {
				free(rects[i]);
				rects[i] = NULL;
			}
			current = 0;
		}
		cpSpaceStep(global_space, 1.0 / 60.0);
		SDL_RenderClear(renderer);
		{
			for(int i = 0; i < 512 && rects[i] != NULL; ++i){
				physics_rect* elt = rects[i];
				Update_PHY_Rect(elt); 
				//printf("{%d,%d}\n", elt->loc_sz.x, elt->loc_sz.y);
			}
		}
		SDL_RenderPresent(renderer);
	}
	
	return 0;
}

physics_rect* Create_PHY_Rect(cpVect position, cpVect size, float mass) {
	cpBody* tmp_body = cpSpaceAddBody(global_space, cpBodyNew(mass, cpMomentForBox(mass, size.x, size.y)));
	cpBodySetPosition(tmp_body, cpv(position.x + size.x / 2, position.y - size.y / 2)); //dont forget to translate from left handed coordinates to center coords for a rect 
	cpShape* tmp_shape = cpSpaceAddShape(global_space, cpBoxShapeNew(tmp_body, size.x, size.y, 0));
	physics_rect* tmp_phy_rect = calloc(1, sizeof(*tmp_phy_rect));
	tmp_phy_rect->loc_sz = (SDL_Rect){ position.x,position.y,size.x,size.y };
	tmp_phy_rect->body = tmp_body;
	return tmp_phy_rect;
}

void Update_PHY_Rect(physics_rect* phy_rect) {
	phy_rect->loc_sz.x = cpBodyGetPosition(phy_rect->body).x;
	phy_rect->loc_sz.y = cpBodyGetPosition(phy_rect->body).y;
	SDL_RenderCopyEx(renderer, grey_color, NULL, &phy_rect->loc_sz, cpBodyGetAngle(phy_rect->body) * (180.0 / M_PI), NULL, SDL_FLIP_NONE);
}