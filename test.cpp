/*
 * test.cpp
 *
 *  Created on: May 24, 2012
 *      Author: Simon Davies
 */


#include "SDL.h"
#include <stdio.h>

#include "heightgen.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256

int height_array[SCREEN_WIDTH][SCREEN_HEIGHT];

void put_pixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);
int draw_screen(SDL_Surface* screen);
SDL_Surface* init_video();

int main(int argc, char *argv[]) {
	SDL_Surface *screen;
	SDL_Event event;

	int quit = false;

	/* Initialise SDL video */
	screen = init_video();

	/* generate heightmap */
	for(int x = 0; x < screen->w; x++){
		for(int y = 0; y < screen->h; y++){
			height_array[x][y] = (int)(brownianValue(x,y + SCREEN_HEIGHT * SCREEN_WIDTH,3) * 255);
		}
	}

	while (!quit) {
		// Draw scene
		if(!(draw_screen(screen) == 0)){
			quit = true;
			break;
		}
		// Poll for events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
				break;
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
			}
		}

	}

	SDL_Quit();

	return 0;
}

/* Initialise SDL Video
 * returns:
 * 	SDL_Surface* screen initialized
 */
SDL_Surface* init_video(){
	SDL_Surface *surface;
	// Initialize defaults, Video and Audio
	if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) {
		fprintf(stderr, "Could not initialize SDL: %s.\n", SDL_GetError());
		return NULL;
	}

	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 24, SDL_HWSURFACE);
	if (surface == NULL) {
		fprintf(stderr, "Couldn't set 800x600x24 video mode: %s\n",
				SDL_GetError());
		return NULL;
	}
	return surface;
}

/* Draws (something) to the screen
 * args:
 * 	SDL_Surface* screen to draw to
 */
int draw_screen(SDL_Surface* screen){
	// Lock the screen for direct access to the pixels
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return -3;
		}
	}

	// Plot the Pixel
	for(int x = 0; x < screen->w; x++){
		for(int y = 0; y < screen->h; y++){
			int colour = height_array[x][y];
			put_pixel(screen, x, y, colour, colour, colour);
		}
	}

	// Unlock Surface if necessary
	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}

	// Update just the part of the display that we've changed
	SDL_Flip(screen);
	return 0;
}

/* Draws an rgb value to a given pixel in a screen.
 * args:
 * 	SDL_Surface* screen that is to be drawn on
 * 	int x, y - coordinates to be drawn to
 * 	int r,g,b - the colour value. Between 0-255
 */
void put_pixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to set

	// Map the color yellow to this display (R=0xff, G=0xFF, B=0x00)
	Uint32 pixel = SDL_MapRGB(surface->format, r, g, b);

	Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *) p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *) p = pixel;
		break;
	}
}
