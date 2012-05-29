/*
 * Vector3d.cpp
 *
 * Vector class for 3D vectors
 *
 *  Created on: May 29, 2012
 *      Author: Simon Davies
 */

#include "Vector3d.h"

Vector3d::Vector3d(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3d::~Vector3d() {
}

Vector3d Vector3d::operator+(Vector3d param) {
	return Vector3d(this->x + param.x, this->y + param.y,this->z + param.z);
}

Vector3d Vector3d::operator-(Vector3d param) {
	return Vector3d(this->x - param.x, this->y - param.y,this->z - param.z);
}
