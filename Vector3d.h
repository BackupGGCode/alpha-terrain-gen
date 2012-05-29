/*
 * Vector3d.h
 *
 * Vector class for 3D vectors.
 *
 *  Created on: May 29, 2012
 *      Author: Simon Davies
 */

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <GL/gl.h>

class Vector3d {
public:
	Vector3d(GLfloat x, GLfloat y, GLfloat z);
	virtual ~Vector3d();

	GLfloat x;
	GLfloat y;
	GLfloat z;

	Vector3d operator + (Vector3d);
	Vector3d operator - (Vector3d);
};

#endif /* VECTOR3D_H_ */
