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

class Quad {
public:
	Quad();
	Quad(Vector3D* vertex1, Vector3D* vertex2, Vector3D* vertex3, Vector3D* vertex4);
	virtual ~Quad();
	void init();

	Vector3D* get_surface_normal();

	void calculate_vertex_normals(
			Quad* NWQuad,		Quad* NQuad,		Quad* NEQuad,
			Quad* WQuad,		/* this QUAD */		Quad* EQuad,
			Quad* SWQuad,		Quad* SQuad,		Quad* SEQuad
	);

private:
	bool using_vertex_normals;
	Vector3D* vertex_data[4];
	Vector3D* vertex_normals[4];
	Vector3D* surface_normal;
	void calculate_surface_normals();
	Vector3D* calculate_vertex_normal(Quad* corner, Quad* direct1, Quad* direct2);
};

#endif /* QUAD_H_ */
