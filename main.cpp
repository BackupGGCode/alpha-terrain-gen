// C includes
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// GL includes
#include <GL/gl.h>
#include <GL/freeglut.h>

// SDL includes
#include "SDL.h"

// Terrain gen
#include "heightgen.h"
#include "TerrainSegment.h"

// Keyboard input struct
#include "Inputs.h"

// Camera
#include "ControllableCamera.h"

static GLint T0 = 0;
static GLint Frames = 0;

TerrainSegment* terrainSegment;
ControllableCamera* camera;

GLint terrain_obj;

// TODO: Rewrite this entire file to be OOP

static void draw(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glRotatef(camera->getPositions()->cam_x_rot, 1.0, 0.0, 0.0);
	glRotatef(camera->getPositions()->cam_y_rot, 0.0, 1.0, 0.0);
	glRotatef(camera->getPositions()->cam_z_rot, 0.0, 0.0, 1.0);
	glTranslatef(	-camera->getPositions()->cam_x_pos,
					-camera->getPositions()->cam_y_pos,
					-camera->getPositions()->cam_z_pos);

	// Draw stuff
	glPushMatrix();
	glEnable(GL_CULL_FACE);

	glCallList(terrain_obj);

	glPopMatrix();

	glPopMatrix();

	Frames++;
	{
		GLint t = SDL_GetTicks();
		if (t - T0 >= 5000) {
			GLfloat seconds = (t - T0) / 1000.0;
			GLfloat fps = Frames / seconds;
			printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
			T0 = t;
			Frames = 0;
		}
	}
}

/* new window size or exposure */
static void reshape(int width, int height) {
	GLfloat h = (GLfloat) height / (GLfloat) width;

	glViewport(0, 0, (GLint) width, (GLint) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -h, h, 5.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -40.0);
}

static void init(int argc, char *argv[]) {

	static GLfloat pos[4] = { 5.0, 5.0, 10.0, 0.0 };
	static GLfloat red[4] = { 0.8, 0.1, 0.0, 1.0 };
	static GLfloat green[4] = { 0.0, 0.8, 0.2, 1.0 };
	static GLfloat blue[4] = { 0.2, 0.2, 1.0, 1.0 };

	//TODO:
	terrainSegment = new TerrainSegment(-40, -40, 160, 160, 0.5);

	// TODO: Change me back

	// TODO: Better lighting
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	terrain_obj = glGenLists(1);
	glNewList(terrain_obj, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	terrainSegment->init_quads();
	glEndList();

	glEnable(GL_NORMALIZE);

	if (argc > 1 && strcmp(argv[1], "-info") == 0) {
		printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
		printf("GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
		printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
		printf("GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));
	}
}

void check_for_movement_inputs(SDL_Event event, Inputs* input) {
	switch (event.type) {
	case SDL_KEYDOWN:
		switch(event.key.keysym.sym){
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
		switch(event.key.keysym.sym){
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

int main(int argc, char *argv[]) {
	SDL_Surface *screen;
	int done;

	// Wireframe mode toggle variable
	int wireframe = 0;

	// When was the last time the scene was drawn?
	double lastDrawTime = 0;

	// Desired Frames Per Second when drawing the scene
	float desiredFPS = 60.0f;

	// One second in milliseconds / desired FPS
	float timePerFrame = 1000 / desiredFPS;

	// Setup input struct
	Inputs* input = new Inputs();
	input->backward = false;
	input->forward = false;
	input->strafe_left = false;
	input->strafe_right = false;

	SDL_Init(SDL_INIT_VIDEO);

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

	// Init screen
	screen = SDL_SetVideoMode(800, 600, 16, SDL_OPENGL | SDL_RESIZABLE);
	if (!screen) {
		fprintf(stderr, "Couldn't set 800x600 GL video mode: %s\n",
				SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	SDL_WM_SetCaption("Alpha", "alpha");

	init(argc, argv);
	reshape(screen->w, screen->h);
	camera = new ControllableCamera(screen->w, screen->h, 0.25f);
	done = 0;

	// Hide cursor for mouse movement
	SDL_ShowCursor(false);

	// Keep mouse within confines of the window
	SDL_WM_GrabInput(SDL_GRAB_ON);

	while (!done) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			check_for_movement_inputs(event, input);
			switch (event.type) {
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
				done = 1;
				break;

			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					done = 1;
					break;
				case SDLK_z:
					if (wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					} else {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}
					wireframe = !wireframe;
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				camera->camera_rotation_mouse(event.motion.x,
												event.motion.y);
				break;

			}
		}

		// Check redraw rate
		if (SDL_GetTicks() > lastDrawTime + timePerFrame) {
			draw();
			camera->camera_translation_keyboard(input);
			camera->move_camera();
			SDL_GL_SwapBuffers();
			lastDrawTime = SDL_GetTicks();
		} else {
			Sleep(0);
		}
	}
	delete(input);
	SDL_Quit();
	return 0; /* ANSI C requires main to return int. */
}
