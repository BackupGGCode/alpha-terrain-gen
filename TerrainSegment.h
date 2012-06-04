/* TerrainSegment.h
 *
 * Class for a section of terrain.
 * Can be specified to an abritrary number of quads, with an arbitrary quad size.
 *
 * Generates the terrain based on the height map generation algorithm.
 *
 *  Created on: May 28, 2012
 *      Author: Simon Davies
 */

#ifndef TERRAINSEGMENT_H_
#define TERRAINSEGMENT_H_

#include <GL/gl.h>
#include "Quad.h"
#include <vector>

class TerrainSegment {
private:
	// Two dimensional array of Quad objects which make
	// up the terrain
	std::vector <std::vector<Quad*> > quad_arrays;

	// Texture ID for the terrain texture
	GLuint terrain_texture_id;

	// Number of quads in width and height directions
	int quad_count_width;
	int quad_count_height;

	// Vector that represents the centre of the terrain segment
	Vector3D centre;

	// Whather this terrain segment has been initialized and
	// is ready to be drawn using OpenGL yet
	bool initialized;

public:
	TerrainSegment();
	TerrainSegment(float x, float z, float segment_size, float quad_size,
			GLuint terrain_texture);
	virtual ~TerrainSegment();
	void init_quads();

	Vector3D get_centre();

	bool get_initialized();

	GLuint terrain_GL_obj;

private:
	Quad* calculate_new_quad(float x, float z, float size);
};

#endif /* TERRAINSEGMENT_H_ */
