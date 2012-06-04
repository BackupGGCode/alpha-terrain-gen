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
#include <math.h>

#define PI 3.1415926

Quad::Quad(){

}

/** Creates a new Quad based on four input vertices */
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

	// Only delete vertex_normals if they have actually been calculated...
	if(using_vertex_normals){
		delete(vertex_normals[0]);
		delete(vertex_normals[1]);
		delete(vertex_normals[2]);
		delete(vertex_normals[3]);

		delete(vertex_materials[0]);
		delete(vertex_materials[1]);
		delete(vertex_materials[2]);
		delete(vertex_materials[3]);
	}
}

/** Calculate all vertex normals.
 * This requires an input of all of the neighbouring quads, and all of these
 * quads must have calculated Surface Normals.
 *
 * Vertex normals are calculated by taking the average of all of the surfaces that
 * share a single vertex.
 *
 * Arguments:
 * -----------------------------------------------------------------
 * 8 neighbouring quads:
 * 		NWQuad,		NQuad,		NEQuad,
		WQuad,		*****		EQuad,
		SWQuad,		SQuad,		SEQuad
 */
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

	Vector3D* vertex_normal = new Vector3D(result.x, result.y, result.z);
	vertex_normal->normalise();

	return vertex_normal;
}

#define SAND_MAX 4.0
#define SAND_MIN 6.0

#define ROCK_MIN 0.175
#define ROCK_MAX 0.375

/** Performs a cosine interpolation between two values */
static double interpolate(double a, double b, double x){
	double ft = x * PI;
	double f = (1 - cos(ft)) * 0.5;

	return a * (1 - f) + b * f;
}

/** Calculates the material colour for each vertex. */
void Quad::calculate_materials(){
	vertex_materials[0] = get_material(vertex_data[0], vertex_normals[0]);
	vertex_materials[1] = get_material(vertex_data[1], vertex_normals[1]);
	vertex_materials[2] = get_material(vertex_data[2], vertex_normals[2]);
	vertex_materials[3] = get_material(vertex_data[3], vertex_normals[3]);
}

/** Sets the material colour, based on the height of the vertex
 * Arguments
 * ----------------------------------------------------------
 * Vector3D the vertex that the material is to be based on
 * Vector3D normal - the vertex normal for the material
 * */
Colour* Quad::get_material(Vector3D* vertex, Vector3D* normal)
{
    // using vertex normals
    Colour* material_colour = new Colour(0.0, 0.8, 0.2, 1.0);

    // Set sand colour
    if(vertex->y < SAND_MAX){
		material_colour->r = 0.7f;
	}
	else if (vertex->y < SAND_MIN){
		material_colour->r = interpolate(0.7f, 0.0f, (vertex->y - SAND_MAX) / (SAND_MIN - SAND_MAX)) ;
	}

    // Set rock surfaces if something that looks like a cliff face
    // Use the normal to determine which direction a vertex is pointing
    if(fabs(normal->x) > ROCK_MAX){
    	material_colour->r = 0.35f;
    	material_colour->g = 0.65f;
    	material_colour->b = 0.4f;
    }
    else if(fabs(normal->x) > ROCK_MIN){
    	material_colour->r = interpolate(0.35f, material_colour->r, (fabs(normal->x) - ROCK_MIN) / (ROCK_MAX - ROCK_MIN));
    	material_colour->g = interpolate(0.65f, material_colour->g, (fabs(normal->x) - ROCK_MIN) / (ROCK_MAX - ROCK_MIN));
    	material_colour->b = interpolate(0.4f, material_colour->b, (fabs(normal->x) - ROCK_MIN) / (ROCK_MAX - ROCK_MIN));
    }
    if(fabs(normal->z) > ROCK_MAX){
    	material_colour->r = 0.35f;
    	material_colour->g = 0.65f;
    	material_colour->b = 0.4f;
    }
    else if(fabs(normal->z) > ROCK_MIN){
    	material_colour->r = interpolate(0.35f, material_colour->r, (fabs(normal->z) - ROCK_MIN) / (ROCK_MAX - ROCK_MIN));
    	material_colour->g = interpolate(0.65f, material_colour->g, (fabs(normal->z) - ROCK_MIN) / (ROCK_MAX - ROCK_MIN));
    	material_colour->b = interpolate(0.4f, material_colour->b, (fabs(normal->z) - ROCK_MIN) / (ROCK_MAX - ROCK_MIN));
    }

    return material_colour;
}

/** Initializes the quad for rendering by OpenGL
 * Should be used as part of the GL_QUADS initialization loop
 * Arguments
 * ------------------------------------------------------------------
 * GLuint texture - ID tag for for the texture
 * float tex_x - texture offset in the x axis
 * float tex_y - texture offset in the y axis
 * float tex_size - size of the texture / scale
 * */
void Quad::init(GLuint texture, float tex_x, float tex_y, float tex_size){

	if(using_vertex_normals){
	// using vertex normals
	GLfloat material[4];

	vertex_materials[0]->getArray(material);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
    glTexCoord2f (tex_x, tex_y);
	glNormal3f(vertex_normals[0]->x,vertex_normals[0]->y, vertex_normals[0]->z);
	glVertex3f(vertex_data[0]->x, vertex_data[0]->y, vertex_data[0]->z);

	vertex_materials[1]->getArray(material);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glTexCoord2f (tex_x + tex_size, tex_y);
	glNormal3f(vertex_normals[1]->x,vertex_normals[1]->y, vertex_normals[1]->z);
	glVertex3f(vertex_data[1]->x, vertex_data[1]->y, vertex_data[1]->z);

	vertex_materials[2]->getArray(material);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glTexCoord2f (tex_x + tex_size, tex_y + tex_size);
	glNormal3f(vertex_normals[2]->x,vertex_normals[2]->y, vertex_normals[2]->z);
	glVertex3f(vertex_data[2]->x, vertex_data[2]->y, vertex_data[2]->z);

	vertex_materials[3]->getArray(material);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glTexCoord2f (tex_x, tex_y + tex_size);
	glNormal3f(vertex_normals[3]->x,vertex_normals[3]->y, vertex_normals[3]->z);
	glVertex3f(vertex_data[3]->x, vertex_data[3]->y, vertex_data[3]->z);
	}
	// If there is no vertex normals, don't use any normals.
	// This should not be reached in ordinary circumstances
	else{
		glNormal3f(0,0,1);
		glVertex3f(vertex_data[0]->x, vertex_data[0]->y, vertex_data[0]->z);
		glVertex3f(vertex_data[1]->x, vertex_data[1]->y, vertex_data[1]->z);
		glVertex3f(vertex_data[2]->x, vertex_data[2]->y, vertex_data[2]->z);
		glVertex3f(vertex_data[3]->x, vertex_data[3]->y, vertex_data[3]->z);
	}

}

/** Calculates the surface normal of a quad. */
void Quad::calculate_surface_normals(){
	Vector3D u = *vertex_data[1] - *vertex_data[0];
	Vector3D v = *vertex_data[2] - *vertex_data[0];

	GLfloat x = (u.y * v.z) - (u.z * v.y);
	GLfloat y = (u.z * v.x) - (u.x * v.z);
	GLfloat z = (u.x * v.y) - (u.y * v.x);

	surface_normal = new Vector3D(x,y,z);
	surface_normal->normalise();
}
