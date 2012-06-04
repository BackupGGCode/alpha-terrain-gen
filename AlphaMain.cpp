/*
 * AlphaMain.cpp
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
bool AlphaMain::quit_flag;
TerrainManager* AlphaMain::terrain_manager;

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
		texture_mapping = false;
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
	fog_enabled = true;
	texture_mapping = true;

	wireframe = false;
	window_active = true;
}

AlphaMain::~AlphaMain() {
	delete(AlphaMain::terrain_manager);
	delete(input);
}

/** Handles key events and records any keys which are related to
 * camera control.
 *
 * Updates the Input struct which is later used by the CameraController to determine
 * where to move the camera.
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

/** Reshapes the window based on the screen height and width */
void AlphaMain::reshape(int width, int height) {
	GLfloat h = (GLfloat) height / (GLfloat) width;

	glViewport(0, 0, (GLint) width, (GLint) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-1.0, 1.0, -h, h, 5.0, fog_distance_end);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/** Draws the scene */
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
						/* Uh oh, we couldn't set the new video mode?? */;
					}
					break;

				case SDL_QUIT:
					quit_flag = 1;
					break;

				case SDL_ACTIVEEVENT:
					if (event.active.state & SDL_APPINPUTFOCUS) {
						//If the application is no longer active
						if (event.active.gain == 0) {
							window_active = false;
							// Turn on cursor and let it escape the screen space
							SDL_ShowCursor(true);
							SDL_WM_GrabInput(SDL_GRAB_OFF);
						} else {
							window_active = true;
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
						if(!fog_enabled){
							glEnable(GL_FOG);
							fog_enabled = true;
						}
						else{
							glDisable(GL_FOG);
							fog_enabled = false;
						}
						break;
					case SDLK_z:
						if (wireframe) {
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							wireframe = false;
						} else {
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							wireframe = true;
						}
						break;
					case SDLK_t:
						if (!texture_mapping) {
							glEnable(GL_TEXTURE_2D);
							texture_mapping = true;
						} else {
							glDisable(GL_TEXTURE_2D);
							texture_mapping = false;
						}
						break;
					case SDLK_r:
						AlphaMain::terrain_manager->repopulate_terrain();
						break;
					default:
						break;
					}
					break;
				case SDL_MOUSEMOTION:
					if(window_active){
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

		terrain_gen_thread = SDL_CreateThread(terrain_regeneration, NULL);

		while (!AlphaMain::quit_flag) {
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				handle_event(event);
			}

			// Check redraw rate
			if (SDL_GetTicks() > lastDrawTime + timePerFrame) {
				AlphaMain::terrain_manager->initialize();
				draw();
				// Camera movement is not activated unless the screen is in OS focus
				if(window_active){
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

int AlphaMain::terrain_regeneration(void* data){
	while(!AlphaMain::quit_flag){
		AlphaMain::terrain_manager->repopulate_terrain();
	}
	return 0;
}

