/*
 * GrassSprite.h
 *
 *  Created on: Jun 1, 2012
 *      Author: Simon
 */

#ifndef GRASSSPRITE_H_
#define GRASSSPRITE_H_

#include "GL\gl.h"
#include "Texture.h"
#include "Vector3D.h"

class GrassSprite {
private:
	GLint texture_id;
	GLfloat scale;
	Vector3D* vertex_data[4];
	Vector3D* normal;
public:
	GrassSprite();
	GrassSprite(Vector3D* vertex1, Vector3D* vertex2,
			Vector3D* vertex3, Vector3D* vertex4, Vector3D* normal);
	virtual ~GrassSprite();

	void init();
};

#endif /* GRASSSPRITE_H_ */
