/*
 * ControllableCamera.h
 *
 * Controllable first person camera.
 * Uses a mouse and keyboard like a first person shooter.
 *
 *  Created on: May 29, 2012
 *      Author: Simon Davies
 */

#ifndef CONTROLLABLECAMERA_H_
#define CONTROLLABLECAMERA_H_

#include <GL/gl.h>
#include "Inputs.h"
#include "Vector3D.h"

struct Positions{
	// Translation
	Vector3D cam_pos;

	// Rotation
	GLfloat cam_x_rot;
	GLfloat cam_y_rot;
	GLfloat cam_z_rot;
};

class ControllableCamera {
private:
	int window_centre_x;
	int window_centre_y;

	GLfloat movement_speed;

	GLfloat cam_x_speed;
	GLfloat cam_y_speed;
	GLfloat cam_z_speed;

	Positions* cam;

public:
	ControllableCamera(int window_width, int window_height, GLfloat movement_speed);
	virtual ~ControllableCamera();
	// Keyboard camera movement
	void camera_translation_keyboard(Inputs* input);

	// Mouse rotation camera
	void camera_rotation_mouse(int mouse_x, int mouse_y);

	// Update the camera position
	void move_camera();

	// Return the position information of the camera
	Positions* getPositions();

	// Screen Resize - should be called if the screen changes size
	void screen_resize(int new_width, int new_height);
};

#endif /* CONTROLLABLECAMERA_H_ */
