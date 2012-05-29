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
#include "Vector3d.h"

class Quad {
public:
	Quad();
	Quad(Vector3d* vertex1, Vector3d* vertex2, Vector3d* vertex3, Vector3d* vertex4);
	virtual ~Quad();
	void init();

	Vector3d* get_surface_normal();

	void calculate_vertex_normals(
			Quad* NWQuad,		Quad* NQuad,		Quad* NEQuad,
			Quad* WQuad,		/* this QUAD */		Quad* EQuad,
			Quad* SWQuad,		Quad* SQuad,		Quad* SEQuad
	);

private:
	Vector3d* vertex_data[4];
	Vector3d* vertex_normals[4];
	Vector3d* surface_normal;
	void calculate_surface_normals();
};

#endif /* QUAD_H_ */
