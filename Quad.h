/*
 * Quad.h
 *
 *  Created on: May 28, 2012
 *      Author: Simon
 */

#ifndef QUAD_H_
#define QUAD_H_

#include <GL/gl.h>

class Quad {
public:
	Quad();
	Quad(GLfloat vertex1[], GLfloat vertex2[], GLfloat vertex3[], GLfloat vertex4[]);
	virtual ~Quad();
	void init();

private:
	GLfloat vertex_data[4][3];
	GLfloat surface_normal[3];
	void calculate_surface_normals();
};

#endif /* QUAD_H_ */
