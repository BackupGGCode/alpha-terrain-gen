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

Quad::Quad(Vector3d* vertex1, Vector3d* vertex2, Vector3d* vertex3, Vector3d* vertex4) {
	vertex_data[0] = vertex1;
	vertex_data[1] = vertex2;
	vertex_data[2] = vertex3;
	vertex_data[3] = vertex4;
	calculate_surface_normals();
}

Quad::~Quad() {
	free(vertex_data);
}

void Quad::calculate_vertex_normals(
		Quad* NWQuad,		Quad* NQuad,		Quad* NEQuad,
		Quad* WQuad,		/* this Quad */		Quad* EQuad,
		Quad* SWQuad,		Quad* SQuad,		Quad* SEQuad
){
	// NorthWest vertex

}


/** Initializes the quad for rendering by OpenGL
 * Should be used as part of the GL_QUADS initialization loop */

void Quad::init(){

	// TODO: Use vertex normals
	glNormal3f(surface_normal->x,surface_normal->y, surface_normal->z);

	glVertex3f(vertex_data[0]->x, vertex_data[0]->y, vertex_data[0]->z);
	glVertex3f(vertex_data[1]->x, vertex_data[1]->y, vertex_data[1]->z);
	glVertex3f(vertex_data[2]->x, vertex_data[2]->y, vertex_data[2]->z);
	glVertex3f(vertex_data[3]->x, vertex_data[3]->y, vertex_data[3]->z);
}

/** Calculates the surface normal of a quad */
void Quad::calculate_surface_normals(){
	Vector3d u = *vertex_data[1] - *vertex_data[0];
	Vector3d v = *vertex_data[2] - *vertex_data[0];

	GLfloat x = (u.y * v.z)- (u.z * v.y);
	GLfloat y = (u.z * v.x)- (u.x * v.z);
	GLfloat z = (u.x * v.y)- (u.y * v.x);

	surface_normal = new Vector3d(x,y,z);
}
