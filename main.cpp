/**
 * Main Function for Alpha
 *
 * Terrain Generation Open GL Program
 * Generates a 3D Terrain-Based world using a procedural algorithm based on
 * the Fractional Brownian Motion algorithm.
 *
 * Uses SDL and OpenGL.
 *
 * Author: Simon Davies
 */

#include "AlphaMain.h"

#include "SDL.h"

int main(int argc, char *argv[]) {
	// Reset console output
#ifdef _WIN32

	const char * confile = "CONOUT$";

	FreeConsole();
	if (!AllocConsole()) {
		// handle error somehow.
	} else {
		freopen(confile, "w", stdout);
		freopen(confile, "w", stderr);
	}
#endif

	AlphaMain application = AlphaMain();

	// Will return when program ends
	application.run();

	// Exit code
	SDL_Quit();
	return 0; /* ANSI C requires main to return int. */
}
