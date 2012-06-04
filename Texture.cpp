/*
 * Texture.cpp
 *
 * Texture loading function.
 *
 *  Created on: May 31, 2012
 *      Author: Simon Davies
 */

#include "Texture.h"

/** Loads the texture given by file_name,
 * and stores the OpenGL handle to that texture in the handle variable.
 *
 * Uses SDL and only works with 24-bit or 32-bit (alpha) .bmp files
 *
 * Arguments
 * ------------------------------------------------------------------
 * char* file_name string for the file to load
 * GLuint* handle in which the final texture will be referenced by when
 * 			drawing with the texture
 */
bool load_texture(const char* file_name, GLuint* handle) {
	SDL_Surface *surface; // This surface will tell us the details of the image

	// RGB or RGBA
	GLenum texture_format;

	GLint num_colours;

	// Load a BMP file with SDL
	if ((surface = SDL_LoadBMP(file_name))) {

		// Check that the image's width is a power of 2
		if ((surface->w & (surface->w - 1)) != 0 ||
				((surface->h & (surface->h - 1)) != 0)) {
			printf("Texture loading warning : Image size not power of 2\n");
		}

		// get the number of channels in the SDL surface
		num_colours = surface->format->BytesPerPixel;
		printf("Num Colours : %i\n", num_colours);
		if (num_colours == 4) // contains an alpha channel
				{
			texture_format = GL_RGBA;
		} else if (num_colours == 3) // no alpha channel
				{
			texture_format = GL_RGB;
		} else {
			printf("Texture loading error : Not true colour image.\n");
			return false;
		}


		// Get texture handle
		glGenTextures(1, handle);

		// Bind the texture object to the handle created
		glBindTexture(GL_TEXTURE_2D, *handle);

		// Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Set texture object data based on the information provided by the SDL
		// Image loading
		gluBuild2DMipmaps(GL_TEXTURE_2D, num_colours, surface->w, surface->h,
				texture_format, GL_UNSIGNED_BYTE, surface->pixels);
	} else {
		printf("SDL could not load image.bmp: %s\n", SDL_GetError());
		return false;
	}

	// Free the SDL_Surface only if it was successfully created
	if (surface) {
		SDL_FreeSurface(surface);
		return true;
	}
	return false;
}

