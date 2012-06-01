/*
 * Texture.h
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

// C++ include
#include <string>

class Texture {
private:
	GLuint texture; // This is a handle to our texture object
public:
	Texture();
	Texture(const char* file_name);
	virtual ~Texture();
	GLuint get_texture_handle();
};

#endif /* TEXTURE_H_ */
