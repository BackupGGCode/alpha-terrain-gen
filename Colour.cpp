/*
 * Colour.cpp
 *
 *	Basic colour holding class.
 *
 *  Created on: Jun 1, 2012
 *      Author: Simon Davies
 */

#include "Colour.h"

Colour::Colour(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Colour::~Colour() {
}

void Colour::getArray(GLfloat* buffer){
	buffer[0] = r;
	buffer[1] = g;
	buffer[2] = b;
	buffer[3] = a;
}
