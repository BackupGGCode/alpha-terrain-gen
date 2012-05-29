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
#include "Vector3D.h"

Quad::Quad(){

}

Quad::Quad(Vector3D* vertex1, Vector3D* vertex2, Vector3D* vertex3, Vector3D* vertex4) {
	vertex_data[0] = vertex1;
	vertex_data[1] = vertex2;
	vertex_data[2] = vertex3;
	vertex_data[3] = vertex4;
	using_vertex_normals = false;
	calculate_surface_normals();
}

Quad::~Quad() {

	delete(vertex_data[0]);
	delete(vertex_data[1]);
	delete(vertex_data[2]);
	delete(vertex_data[3]);

	delete(vertex_normals[0]);
	delete(vertex_normals[1]);
	delete(vertex_normals[2]);
	delete(vertex_normals[3]);
}

void Quad::calculate_vertex_normals(
		Quad* NWQuad,		Quad* NQuad,		Quad* NEQuad,
		Quad* WQuad,		/* this Quad */		Quad* EQuad,
		Quad* SWQuad,		Quad* SQuad,		Quad* SEQuad
){
	vertex_normals[0] = calculate_vertex_normal(NWQuad,NQuad,WQuad);
	vertex_normals[1] = calculate_vertex_normal(SWQuad,SQuad,WQuad);
	vertex_normals[2] = calculate_vertex_normal(SEQuad,EQuad,SQuad);
	vertex_normals[3] = calculate_vertex_normal(NEQuad,NQuad,EQuad);
	// Toggle flag
	using_vertex_normals = true;
}

Vector3D* Quad::calculate_vertex_normal(Quad* corner, Quad* direct1, Quad* direct2){
	Vector3D result = *this->surface_normal;
	result = result + *corner->surface_normal + *direct1->surface_normal + *direct2->surface_normal;
	result = result / 4;
	return new Vector3D(result.x, result.y, result.z);
}


/** Initializes the quad for rendering by OpenGL
 * Should be used as part of the GL_QUADS initialization loop */
void Quad::init(){

	// TODO: Currently not drawing edge surfaces, when not been able to calculate surface normals
	if(using_vertex_normals){

	// using vertex normals
	glNormal3f(vertex_normals[0]->x,vertex_normals[0]->y, vertex_normals[0]->z);
	glVertex3f(vertex_data[0]->x, vertex_data[0]->y, vertex_data[0]->z);

	glNormal3f(vertex_normals[1]->x,vertex_normals[1]->y, vertex_normals[1]->z);
	glVertex3f(vertex_data[1]->x, vertex_data[1]->y, vertex_data[1]->z);

	glNormal3f(vertex_normals[2]->x,vertex_normals[2]->y, vertex_normals[2]->z);
	glVertex3f(vertex_data[2]->x, vertex_data[2]->y, vertex_data[2]->z);

	glNormal3f(vertex_normals[3]->x,vertex_normals[3]->y, vertex_normals[3]->z);
	glVertex3f(vertex_data[3]->x, vertex_data[3]->y, vertex_data[3]->z);

	}
}

/** Calculates the surface normal of a quad */
void Quad::calculate_surface_normals(){
	Vector3D u = *vertex_data[1] - *vertex_data[0];
	Vector3D v = *vertex_data[2] - *vertex_data[0];

	GLfloat x = (u.y * v.z)- (u.z * v.y);
	GLfloat y = (u.z * v.x)- (u.x * v.z);
	GLfloat z = (u.x * v.y)- (u.y * v.x);

	surface_normal = new Vector3D(x,y,z);
}
