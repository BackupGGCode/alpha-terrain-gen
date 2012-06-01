/*
 * Texture.cpp
 *
 *  Created on: May 31, 2012
 *      Author: Simon Davies
 */

#include "Texture.h"

Texture::Texture(){

}

/* Image loading class */
Texture::Texture(const char* file_name) {
	SDL_Surface *surface; // This surface will tell us the details of the image
	GLenum texture_format;
	GLint num_colours;

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
		}

		// Get texture handle
		glGenTextures(1, &texture);

		// Bind the texture object to the handle created
		glBindTexture(GL_TEXTURE_2D, texture);

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
	}

	// Free the SDL_Surface only if it was successfully created
	if (surface) {
		SDL_FreeSurface(surface);
	}

}

Texture::~Texture() {

}

GLuint Texture::get_texture_handle() {
	return texture;
}

