/*
 * Texture.h
 *
 * Texture loading function
 *
 *  Created on: May 31, 2012
 *      Author: Simon Davies
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

// GL includes
#include <GL/gl.h>
#include <GL/glut.h>

// SDL includes
#include "SDL.h"

bool load_texture(const char* file_name, GLuint* handle);

#endif /* TEXTURE_H_ */
