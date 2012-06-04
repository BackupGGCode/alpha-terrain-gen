/*
 * Colour.cpp
 *
 *	Basic colour holding class.
 *
 *  Created on: Jun 1, 2012
 *      Author: Simon Davies
 */

#include "Colour.h"

/** Colour Constructor
 * Arguments:
 * ----------------------------------------------------------------------------
 * float Red value - between 0.0 and 1.0
 * float Green value - between 0.0 and 1.0
 * float Blue value - between 0.0 and 1.0
 * float Alpha value - between 0.0 and 1.0
 */
Colour::Colour(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Colour::~Colour() {
}

/** Sets the buffer to have the contents
 * of the colour in a format used by OpenGL.
 */
void Colour::getArray(GLfloat* buffer){
	buffer[0] = r;
	buffer[1] = g;
	buffer[2] = b;
	buffer[3] = a;
}
