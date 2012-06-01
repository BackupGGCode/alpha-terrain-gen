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

AlphaMain::AlphaMain() {
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

	Texture terrain_texture = Texture("dirt.bmp");

	T0 = 0;
	frames = 0;

	wireframe = false;
	window_active = true;

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

	// TODO: More terrain segments
	terrain_segments.resize(81);
	float start_x = -10;
	float start_z = -10;

	float quad_size = 1.0f;
	float segment_size = 10.0f;

	float x = start_x;
	float z = start_z;

	// Statically generate terrain segments
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			terrain_segments[(i * 9) + j] = new TerrainSegment(x, z,
					segment_size, quad_size, terrain_texture.get_texture_handle());
			z += segment_size;
		}
		z = start_z;
		x += segment_size;
	}

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
	glClearColor(fogColor[0],fogColor[1],fogColor[2],fogColor[3]);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.003f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 30.0f);
	glFogf(GL_FOG_END, 400.0f);
	glEnable(GL_FOG);

	/* Initialise terrain segments */
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		terrain_segments[i]->terrain_GL_obj = glGenLists(1);

		glNewList(terrain_segments[i]->terrain_GL_obj, GL_COMPILE);

		terrain_segments[i]->init_quads();
		glEndList();
	}

	glEnable(GL_NORMALIZE);
}

AlphaMain::~AlphaMain() {
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		delete(terrain_segments[i]);
	}
	terrain_segments.clear();
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
	glFrustum(-1.0, 1.0, -h, h, 5.0, 400.0);
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
	glTranslatef(-camera->getPositions()->cam_x_pos,
			-camera->getPositions()->cam_y_pos,
			-camera->getPositions()->cam_z_pos);

	// Draw world objects
	glPushMatrix();

	// Draw Terrain
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		glCallList(terrain_segments[i]->terrain_GL_obj);
	}


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
						quit_flag = 1;
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
						camera->getPositions()->cam_x_pos,
						camera->getPositions()->cam_x_rot,
						camera->getPositions()->cam_y_pos,
						camera->getPositions()->cam_y_rot,
						camera->getPositions()->cam_z_pos,
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
		quit_flag = false;

		// When was the last time the scene was drawn?
		double lastDrawTime = 0;

		// Desired Frames Per Second when drawing the scene
		float desiredFPS = 60.0f;

		// One second in milliseconds / desired FPS
		float timePerFrame = 1000 / desiredFPS;

		reshape(screen->w, screen->h);
		camera = new ControllableCamera(screen->w, screen->h, 0.25f);

		// Hide cursor for mouse movement
		SDL_ShowCursor(false);

		// Keep mouse within confines of the window
		SDL_WM_GrabInput(SDL_GRAB_ON);

		while (!quit_flag) {
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				handle_event(event);
			}

			// Check redraw rate
			if (SDL_GetTicks() > lastDrawTime + timePerFrame) {
				draw();
				// Camera movement is not activated unless the screen is in OS focus
				if(window_active){
					camera->camera_translation_keyboard(input);
					camera->move_camera();
				}
				SDL_GL_SwapBuffers();
				lastDrawTime = SDL_GetTicks();
			} else {
				Sleep(0);
			}
		}
}

