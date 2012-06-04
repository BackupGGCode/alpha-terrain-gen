/*
 * AlphaMain.h
 *
 *  Created on: May 30, 2012
 *      Author: Simon Davies
 */

#ifndef ALPHAMAIN_H_
#define ALPHAMAIN_H_

// GL includes
#include <GL/gl.h>
#include <GL/freeglut.h>

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
	SDL_Surface *screen;
	SDL_Thread *terrain_gen_thread;

	GLuint T0;
	GLuint frames;

	// Wireframe mode toggle variable
	bool wireframe;

	// Is the window currently OS active?
	bool window_active;

	bool fog_enabled;

	bool texture_mapping;

	GLfloat fog_distance_end;
	GLfloat fog_distance_start;

	// Keyboard input struct for camera control
	Inputs* input;

	ControllableCamera* camera;

	void draw();
	void reshape(int width, int height);
	void check_for_movement_inputs(SDL_Event event);
    void handle_event(SDL_Event event);

public:
	static bool quit_flag;
	static TerrainManager* terrain_manager;

	AlphaMain();
	virtual ~AlphaMain();

	void run();

	// Thread
	static int terrain_regeneration(void* data);

};

#endif /* ALPHAMAIN_H_ */
