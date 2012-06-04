/*
 * AlphaMain.h
 *
 * The MainClass that is the first class to be initialized.
 * Contains the setup for SDL and OpenGL and holds the Terrain Generation Manager
 * in the form of the TerrainManager instance.
 *
 * This class is the basis of the program and pretty much everything comes from here.
 *
 *  Created on: May 30, 2012
 *      Author: Simon Davies
 */

#ifndef ALPHAMAIN_H_
#define ALPHAMAIN_H_

// GL includes
#include <GL/gl.h>

// SDL includes
#include "SDL.h"
#include "SDL_thread.h"

// Terrain gen
#include "TerrainManager.h"

// Keyboard input struct
#include "Inputs.h"

// Camera
#include "ControllableCamera.h"

// Texture
#include "Texture.h"

class AlphaMain {
private:

	/** The SDL_Surface for the screen, which is set up
	 * to use OpenGL for 3D rendering */
	SDL_Surface *screen;

	// Second thread for Terrain Generation
	SDL_Thread *terrain_gen_thread;

	GLuint T0;
	GLuint frames;

	// Is the window currently OS active?
	bool is_window_active;

	/* Toggles */
	// Wireframe mode toggle variable
	bool wireframe_toggle;

	// Toggle for OpenGL fog
	bool fog_toggle;

	// Toggle for texture mapping
	bool texture_mapping_toggle;

	// Fog settings
	// The end value distance of the fog
	// By this point, objects are completly occluded/
	GLfloat fog_distance_end;

	// The start distance for the fog occlusion
	GLfloat fog_distance_start;

	// Keyboard input struct for camera control
	Inputs* input;

	// Controllable Camera allows mouse and keyboard input
	// for the user to move around the world.
	ControllableCamera* camera;

	// Draws the scene
	void draw();

	// Reshapes the screen based on new pixel width and pixel height
	void reshape(int width, int height);

	// Check for movement inputs based on users keyboard input
	void check_for_movement_inputs(SDL_Event event);

	// Event handling for the rest of SDL_Events
    void handle_event(SDL_Event event);

public:
    // Static variables,
    // Made static so they are accessible by the secondary thread
	static bool quit_flag;
	static TerrainManager* terrain_manager;

	AlphaMain();
	virtual ~AlphaMain();

	void run();

	// Thread
	static int terrain_regeneration(void* data);

};

#endif /* ALPHAMAIN_H_ */
