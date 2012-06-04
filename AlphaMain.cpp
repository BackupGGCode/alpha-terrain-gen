/*
 * AlphaMain.cpp
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

#include "AlphaMain.h"

// C includes
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

// Static declarations
// These are set as static so they are accessible by the secondary
// terrain generation thread
bool AlphaMain::quit_flag;
TerrainManager* AlphaMain::terrain_manager;

/** Constructor / Initialialization of the AlphaMain class.
 * Sets up the SDL screen, and the SDL environment, as well as configuring
 * OpenGL for use.
 *
 * Sets up the ControllableCamera and Input structs, and most importantly creates
 * a new terrain manager that handles all of the terrain generation.
 */
AlphaMain::AlphaMain() {
	// Thread
	terrain_gen_thread = NULL;

	SDL_Init(SDL_INIT_VIDEO);
	// Init screen
	screen = SDL_SetVideoMode(800, 600, 16, SDL_OPENGL | SDL_RESIZABLE);
	if (!screen) {
		fprintf(stderr, "Couldn't set 800x600 GL video mode: %s\n",
				SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	SDL_WM_SetCaption("Alpha", "alpha");

	GLuint terrain_texture;
	if(!load_texture("dirt.bmp", &terrain_texture)){
		texture_mapping_toggle = false;
	}

	T0 = 0;
	frames = 0;

	// Keyboard input struct
	input = new Inputs();
	input->backward = false;
	input->forward = false;
	input->strafe_left = false;
	input->strafe_right = false;

	/* Open GL Setup begins here */

	// Lighting
	GLfloat pos[4] = { 5.0, 5.0, 10.0, 0.0 };
	GLfloat ambient[]  = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat diffuse[]  = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat specular[]  = {1.0f, 1.0f, 1.0f, 1.0f};

	// Lighting setup
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Enable culling of rear faces
	glEnable(GL_CULL_FACE);
	// Enable Textures
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// Enable occlusion of objects
	glEnable(GL_DEPTH_TEST);

	// Fog settings
	GLfloat fogColor[4]= {0.4f,0.7f,1.0f, 1.0f};      // Fog / Sky Color

	// Set class fog variables
	fog_distance_start = 200.0f;
	fog_distance_end = 400.0f;

	glClearColor(fogColor[0],fogColor[1],fogColor[2],fogColor[3]);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.003f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, fog_distance_start);
	glFogf(GL_FOG_END, fog_distance_end);
	glEnable(GL_FOG);

	// Init camera
	camera = new ControllableCamera(screen->w, screen->h, 1.0f);

	// Init terrain manager
	AlphaMain::terrain_manager = new TerrainManager(terrain_texture, 120.0f, camera, 512);

	glEnable(GL_NORMALIZE);

	// set toggle parameters
	fog_toggle = true;
	texture_mapping_toggle = true;

	wireframe_toggle = false;
	is_window_active = true;
}

/** AlphaMain destructor */
AlphaMain::~AlphaMain() {
	delete(AlphaMain::terrain_manager);
	delete(input);
}

/** Handles key events and records any keys which are related to
 * camera control.
 *
 * Updates the Input struct which is later used by the CameraController to determine
 * where to move the camera.
 * Arguments:
 * ----------------------------------------------------------------------------
 * SDL_Event event reported by SDL
 */
void AlphaMain::check_for_movement_inputs(SDL_Event event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_w:
			input->forward = true;
			break;
		case SDLK_s:
			input->backward = true;
			break;
		case SDLK_a:
			input->strafe_left = true;
			break;
		case SDLK_d:
			input->strafe_right = true;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_w:
			input->forward = false;
			break;
		case SDLK_s:
			input->backward = false;
			break;
		case SDLK_a:
			input->strafe_left = false;
			break;
		case SDLK_d:
			input->strafe_right = false;
			break;
		default:
			break;
		}
		break;
	}
}

/** Reshapes the window based on the screen height and width.
 * Generates a new view model frustrum based on the new screen height and width,
 * and culls the screen based on the fog distance.
 * Arguments:
 * ----------------------------------------------------------------------------
 * int new screen width
 * int new screen height
 */
void AlphaMain::reshape(int width, int height) {
	GLfloat h = (GLfloat) height / (GLfloat) width;

	// reset viewport
	glViewport(0, 0, (GLint) width, (GLint) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// distant object culling is set here, by the 6th argument
	glFrustum(-1.0, 1.0, -h, h, 5.0, fog_distance_end);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/** Draws the scene to the screen.
 * First translates to the (opposite of) camera position then renders the scene.
 * Arguments:
 * ----------------------------------------------------------------------------
 */
void AlphaMain::draw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	// Camera movements
	// Rotation
	glRotatef(camera->getPositions()->cam_x_rot, 1.0, 0.0, 0.0);
	glRotatef(camera->getPositions()->cam_y_rot, 0.0, 1.0, 0.0);
	glRotatef(camera->getPositions()->cam_z_rot, 0.0, 0.0, 1.0);

	// Translation
	glTranslatef(-camera->getPositions()->cam_pos.x,
			-camera->getPositions()->cam_pos.y,
			-camera->getPositions()->cam_pos.z);

	// Draw world objects
	glPushMatrix();

	// Draw terrain
	AlphaMain::terrain_manager->draw();

	glPopMatrix();

	glPopMatrix();

	// Measure framerate and report
	frames++;
	{
		GLint t = SDL_GetTicks();
		if (t - T0 >= 5000) {
			GLfloat seconds = (t - T0) / 1000.0;
			GLfloat fps = frames / seconds;
			printf("%d frames in %g seconds = %g FPS\n", frames, seconds, fps);
			T0 = t;
			frames = 0;
		}
	}
}

/** Handles SDL_events, such as when the screen is resized,
 * mouse movements and keyboard inputs.
 * Arguments:
 * ----------------------------------------------------------------------------
 * SDL_Event event - The event to be handled
 */
void AlphaMain::handle_event(SDL_Event event)
{
    check_for_movement_inputs(event);
    switch (event.type) {
				// Screen resized
				case SDL_VIDEORESIZE:
					screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16,
							SDL_OPENGL | SDL_RESIZABLE);
					if (screen) {
						reshape(screen->w, screen->h);
						// Camera needs to be informed of new screen size
						camera->screen_resize(screen->w, screen->h);
					} else {
						// Error setting screen
					}
					break;

				case SDL_QUIT:
					quit_flag = 1;
					break;

				case SDL_ACTIVEEVENT:
					if (event.active.state & SDL_APPINPUTFOCUS) {
						//If the application is no longer active
						if (event.active.gain == 0) {
							is_window_active = false;
							// Turn on cursor and let it escape the screen space
							SDL_ShowCursor(true);
							SDL_WM_GrabInput(SDL_GRAB_OFF);
						} else {
							is_window_active = true;
							// Hide cursor for mouse movement
							SDL_ShowCursor(false);
							// Keep mouse within confines of the window
							SDL_WM_GrabInput(SDL_GRAB_ON);
						}
					}
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						// User has hit escape,
						// quits out of the program
						this->quit_flag = 1;
						break;
					case SDLK_c:
						// Test function - prints out camera position
						printf("Camera Position : \n"
								"cam_x_pos: %f\n"
								"cam_x_rot: %f\n"
								"cam_y_pos: %f\n"
								"cam_y_rot: %f\n"
								"cam_z_pos: %f\n"
								"cam_z_rot: %f\n",
						camera->getPositions()->cam_pos.x,
						camera->getPositions()->cam_x_rot,
						camera->getPositions()->cam_pos.y,
						camera->getPositions()->cam_y_rot,
						camera->getPositions()->cam_pos.z,
						camera->getPositions()->cam_z_rot
						);
						break;
					case SDLK_f:
						// F key toggles fog
						if(!fog_toggle){
							glEnable(GL_FOG);
							fog_toggle = true;
						}
						else{
							glDisable(GL_FOG);
							fog_toggle = false;
						}
						break;
					case SDLK_z:
						// Z key toggles Wireframe mode
						if (wireframe_toggle) {
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							wireframe_toggle = false;
						} else {
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							wireframe_toggle = true;
						}
						break;
					case SDLK_t:
						// T key toggles texture mapping
						if (!texture_mapping_toggle) {
							glEnable(GL_TEXTURE_2D);
							texture_mapping_toggle = true;
						} else {
							glDisable(GL_TEXTURE_2D);
							texture_mapping_toggle = false;
						}
						break;
					default:
						break;
					}
					break;
				case SDL_MOUSEMOTION:
					// Mouse movement is tracked and sent to the the camera
					// class, and then rotates the camera appropriately.
					if(is_window_active){
						camera->camera_rotation_mouse(event.motion.x, event.motion.y);
					}
					break;

				}
}

/** Main Run loop */
void AlphaMain::run(){
		AlphaMain::quit_flag = false;

		// When was the last time the scene was drawn?
		double lastDrawTime = 0;

		// Desired Frames Per Second when drawing the scene
		float desiredFPS = 60.0f;

		// One second in milliseconds / desired FPS
		float timePerFrame = 1000 / desiredFPS;

		reshape(screen->w, screen->h);

		// Hide cursor for mouse movement
		SDL_ShowCursor(false);

		// Keep mouse within confines of the window
		SDL_WM_GrabInput(SDL_GRAB_ON);

		// Create the a new thread that will handle the terrain generation process
		// so that the main drawing thread is not interrupted by terrain calculation
		terrain_gen_thread = SDL_CreateThread(terrain_regeneration, NULL);

		// Run loop
		while (!AlphaMain::quit_flag) {
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				handle_event(event);
			}

			// Check redraw rate
			// Screen is not drawn unless enough time has passed since the
			// last pass to maintain 60 FPS.
			if (SDL_GetTicks() > lastDrawTime + timePerFrame) {
				// Initializes any uninitialized terrain segments
				AlphaMain::terrain_manager->initialize_new_terrain_segments();
				draw();
				// Camera movement is not activated unless the screen is in OS focus
				if(is_window_active){
					// Move camera based on the current state of inputs
					camera->camera_translation_keyboard(input);
					camera->move_camera();
				}
				SDL_GL_SwapBuffers();
				lastDrawTime = SDL_GetTicks();
			} else {
				SDL_Delay(0);
			}
		}
}

/* Static method for doing new terrain regeneration.
 * Used by a seperate second thread, hence the need for this to be Static.
 * Arguments:
 * ----------------------------------------------------------------------------
 */
int AlphaMain::terrain_regeneration(void* data){
	while(!AlphaMain::quit_flag){
		AlphaMain::terrain_manager->repopulate_terrain();
	}
	return 0;
}

