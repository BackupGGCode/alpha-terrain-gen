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

class Vector3D {
public:
	Vector3D(GLfloat x, GLfloat y, GLfloat z);
	virtual ~Vector3D();

	GLfloat x;
	GLfloat y;
	GLfloat z;

	Vector3D operator + (Vector3D);
	Vector3D operator - (Vector3D);
	Vector3D operator / (float);
	Vector3D operator * (float);
};

#endif /* VECTOR3D_H_ */
