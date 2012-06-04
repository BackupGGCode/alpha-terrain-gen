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
	Vector3D();
	Vector3D(const Vector3D& param);
	virtual ~Vector3D();

	void normalise();

	// X Y and Z coordinates
	GLfloat x;
	GLfloat y;
	GLfloat z;

	// Overloaded operators
	Vector3D operator + (Vector3D);
	Vector3D operator - (Vector3D);
	Vector3D operator / (float);
	Vector3D operator / (int);
	Vector3D operator * (float);
	Vector3D operator * (int);
};

float distance(Vector3D, Vector3D);
float distance2d(Vector3D, Vector3D);

#endif /* VECTOR3D_H_ */
