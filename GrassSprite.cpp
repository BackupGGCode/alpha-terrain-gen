/*
 * GrassSprite.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: Simon
 */

#include "GrassSprite.h"

GrassSprite::GrassSprite(){

}

GrassSprite::GrassSprite(Vector3D* vertex1, Vector3D* vertex2,
		Vector3D* vertex3, Vector3D* vertex4, Vector3D* normal) {

	vertex_data[0] = vertex1;
	vertex_data[1] = vertex2;
	vertex_data[2] = vertex3;
	vertex_data[3] = vertex4;

	this->normal = normal;
}

GrassSprite::~GrassSprite() {
	delete(vertex_data[0]);
	delete(vertex_data[1]);
	delete(vertex_data[2]);
	delete(vertex_data[3]);

	delete(normal);
}

void GrassSprite::init(){

	glShadeModel(GL_FLAT);
	glBegin(GL_QUADS);

	glBindTexture (GL_TEXTURE_2D, texture_id);
	glNormal3f(normal->x,normal->y, normal->z);

    glTexCoord2f (0.0, 1.0);
	glVertex3f(vertex_data[0]->x, vertex_data[0]->y, vertex_data[0]->z);

    glTexCoord2f (0.0, 0.0);
	glVertex3f(vertex_data[1]->x, vertex_data[1]->y, vertex_data[1]->z);

	glTexCoord2f (1.0, 1.0);
	glVertex3f(vertex_data[2]->x, vertex_data[2]->y, vertex_data[2]->z);

    glTexCoord2f (1.0, 0.0);
	glVertex3f(vertex_data[3]->x, vertex_data[3]->y, vertex_data[3]->z);

	glEnd();
}
