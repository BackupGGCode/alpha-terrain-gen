/*
 * Vector3d.cpp
 *
 * Vector class for 3D vectors
 *
 *  Created on: May 29, 2012
 *      Author: Simon Davies
 */

#include "Vector3d.h"
#include "Math.h"

Vector3D::Vector3D() {
}

Vector3D::Vector3D(const Vector3D& param) {
	this->x = param.x;
	this->y = param.y;
	this->z = param.z;
}


Vector3D::Vector3D(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D::~Vector3D() {
}

void Vector3D::normalise(){
	float norm = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
	x = x / norm;
	y = y / norm;
	z = z / norm;
}

Vector3D Vector3D::operator + (Vector3D param) {
	return Vector3D(this->x + param.x, this->y + param.y,this->z + param.z);
}

Vector3D Vector3D::operator - (Vector3D param) {
	return Vector3D(this->x - param.x, this->y - param.y,this->z - param.z);
}

/** Scalar division */
Vector3D Vector3D::operator / (float param) {
	return Vector3D(this->x / param, this->y / param,this->z / param);
}
/** Scalar division */
Vector3D Vector3D::operator / (int param) {
	return Vector3D(this->x / param, this->y / param,this->z / param);
}

/** Scalar division */
Vector3D Vector3D::operator * (float param) {
	return Vector3D(this->x * param, this->y * param,this->z * param);
}
/** Scalar division */
Vector3D Vector3D::operator * (int param) {
	return Vector3D(this->x * param, this->y * param,this->z * param);
}

/** Returns the distance between two vectors */
float distance(Vector3D p, Vector3D q){
	return sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2) + pow(p.z - q.z, 2));
}

/** Returns the distance between two vectors */
float distance2d(Vector3D p, Vector3D q){
	return sqrt(pow(p.x - q.x, 2) + pow(p.z - q.z, 2));
}
