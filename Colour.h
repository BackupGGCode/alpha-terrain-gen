/*
 * Colour.h
 *
 * Basic colour holding class.
 *
 *  Created on: Jun 1, 2012
 *      Author: Simon Davies
 */

#ifndef COLOUR_H_
#define COLOUR_H_

#include <GL/gl.h>

class Colour {
public:
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;

	Colour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void getArray(GLfloat* buffer);
	virtual ~Colour();
};

#endif /* COLOUR_H_ */
