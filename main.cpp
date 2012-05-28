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

// TODO: remove me
#include "detrand.h"

static GLint T0 = 0;
static GLint Frames = 0;

TerrainSegment* terrainSegment;

GLint terrain_obj;

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;

static void draw(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);
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
	glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
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
	terrainSegment = new TerrainSegment(-40,-40, 160, 160, 0.5);

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

	Uint8 *keys;

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

	//TODO: REmove me test / debug
	printf("x: 1 , y : 1, %f\n", getRandFloat(1,1));
	printf("x: -1 , y : -1, %f\n", getRandFloat(-1,-1));
	printf("x: -1 , y : 1, %f\n", getRandFloat(-1,1));

	init(argc, argv);
	reshape(screen->w, screen->h);
	done = 0;
	while (!done) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_VIDEORESIZE:
				screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16,
						SDL_OPENGL | SDL_RESIZABLE);
				if (screen) {
					reshape(screen->w, screen->h);
				} else {
					/* Uh oh, we couldn't set the new video mode?? */;
				}
				break;

			case SDL_QUIT:
				done = 1;
				break;

			case SDL_KEYDOWN:
				keys = SDL_GetKeyState(NULL);
				if (keys[SDLK_ESCAPE]) {
					done = 1;
				}
				if (keys[SDLK_UP]) {
					view_rotx += 5.0;
				}
				if (keys[SDLK_DOWN]) {
					view_rotx -= 5.0;
				}
				if (keys[SDLK_LEFT]) {
					view_roty += 5.0;
				}
				if (keys[SDLK_RIGHT]) {
					view_roty -= 5.0;
				}
				if (keys[SDLK_z]) {
					if (SDL_GetModState() & KMOD_SHIFT) {
						view_rotz -= 5.0;
					} else {
						view_rotz += 5.0;
					}
				}
				if (keys[SDLK_w]) {
					if (wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					} else {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}
					wireframe = !wireframe;
				}
				break;
			}
		}

		// Check redraw rate
		if (SDL_GetTicks() > lastDrawTime + timePerFrame) {
			draw();
			SDL_GL_SwapBuffers();
			lastDrawTime = SDL_GetTicks();
		}
	}
	SDL_Quit();
	return 0; /* ANSI C requires main to return int. */
}
