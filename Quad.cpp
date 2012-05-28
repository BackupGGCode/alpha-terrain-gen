/*
 * Quad.cpp
 *
 * Class for a terrain quad.
 *
 *  Created on: May 28, 2012
 *      Author: Simon Davies
 */

#include "Quad.h"
#include <string.h>
#include <stdlib.h>

Quad::Quad(){

}

Quad::Quad(GLfloat vertex1[], GLfloat vertex2[], GLfloat vertex3[], GLfloat vertex4[]) {
	memcpy(vertex_data[0], vertex1, sizeof(GLfloat) * 3);
	memcpy(vertex_data[1], vertex2, sizeof(GLfloat) * 3);
	memcpy(vertex_data[2], vertex3, sizeof(GLfloat) * 3);
	memcpy(vertex_data[3], vertex4, sizeof(GLfloat) * 3);
	calculate_surface_normals();
}

Quad::~Quad() {
	free(vertex_data);
	free(surface_normal);
}

/** Initializes the quad for rendering by OpenGL
 * Should be used as part of the GL_QUADS initialization loop */

void Quad::init(){

	glNormal3f(surface_normal[0], surface_normal[1], surface_normal[2]);

	glVertex3f(vertex_data[0][0], vertex_data[0][1], vertex_data[0][2]);
	glVertex3f(vertex_data[1][0], vertex_data[1][1], vertex_data[1][2]);
	glVertex3f(vertex_data[2][0], vertex_data[2][1], vertex_data[2][2]);
	glVertex3f(vertex_data[3][0], vertex_data[3][1], vertex_data[3][2]);
}

/** Calculates the surface normal of a quad */
void Quad::calculate_surface_normals(){
	float u[] = {
			vertex_data[1][0] - vertex_data[0][0],
			vertex_data[1][1] - vertex_data[0][1],
			vertex_data[1][2] - vertex_data[0][2],
	};
	float v[] = {
			vertex_data[2][0] - vertex_data[0][0],
			vertex_data[2][1] - vertex_data[0][1],
			vertex_data[2][2] - vertex_data[0][2],
	};

	surface_normal[0] = (u[1] * v[2])- (u[2] * v[1]);
	surface_normal[1] = (u[2] * v[0])- (u[0] * v[2]);
	surface_normal[2] = (u[0] * v[1])- (u[1] * v[0]);
}
