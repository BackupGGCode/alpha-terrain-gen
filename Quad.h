/*
 * Quad.h
 *
 * Class for a terrain quad.
 *
 *  Created on: May 28, 2012
 *      Author: Simon Davies
 */

#ifndef QUAD_H_
#define QUAD_H_

#include <GL/gl.h>
#include "Vector3D.h"
#include <vector>
#include "Colour.h"

class Quad {
public:
	Quad();
	Quad(Vector3D* vertex1, Vector3D* vertex2, Vector3D* vertex3, Vector3D* vertex4);
	virtual ~Quad();

	// GL init
	void init(GLuint texture, float tex_x, float tex_y, float tex_size);

	// Normals
	Vector3D* get_surface_normal();
	void calculate_vertex_normals(
			Quad* NWQuad,		Quad* NQuad,		Quad* NEQuad,
			Quad* WQuad,		/* this QUAD */		Quad* EQuad,
			Quad* SWQuad,		Quad* SQuad,		Quad* SEQuad
	);

	// Materials
	void calculate_materials();

	// Grass
	void generate_grass_sprites();
	void init_grass();


private:
	bool using_vertex_normals;
	Vector3D* vertex_data[4];
	Vector3D* vertex_normals[4];
	Vector3D* surface_normal;

	Colour* vertex_materials[4];

	Colour* get_material(Vector3D* vertex, Vector3D* normal);
    void calculate_surface_normals();
	Vector3D* calculate_vertex_normal(Quad* corner, Quad* direct1, Quad* direct2);
};

#endif /* QUAD_H_ */
