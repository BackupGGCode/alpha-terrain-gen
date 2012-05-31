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

// SDL includes
#include "SDL.h"

class Texture {
private:
	GLuint texture; // This is a handle to our texture object
public:
	Texture(char* filename);
	virtual ~Texture();
	GLuint get_texture_handle();
};

#endif /* TEXTURE_H_ */
