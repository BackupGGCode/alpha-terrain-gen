/*
 * ControllableCamera.cpp
 *
 * Controllable first person camera.
 * Uses a mouse and keyboard like a first person shooter.
 *
 * Based on code from:
 * http://r3dux.org/2011/05/simple-opengl-keyboard-and-mouse-fps-controls/
 *
 *  Created on: May 29, 2012
 *      Author: Simon Davies
 */

#include "ControllableCamera.h"

// SDL includes
#include "SDL.h"
#include <Math.h>

#define PI 3.1415926

/** Creates a new controllable camera and sets starting values
 * Arguments
 * -----------------------------------------------------------------
 * int window_width - width of the screen
 * int window_height - height of the screen
 *
 * Size of the screen is required to reset the mouse to the middle of the screen.
 *
 * GLfloat movement_speed - set the movement speed of the camera
 */
ControllableCamera::ControllableCamera(int window_width, int window_height,
		GLfloat movement_speed) {

	this->movement_speed = movement_speed;

	window_centre_x = window_width / 2;
	window_centre_y = window_height / 2;

	cam_x_speed = 0;
	cam_y_speed = 0;
	cam_z_speed = 0;

	cam = new Positions();
	cam->cam_pos.x = 0;
	cam->cam_x_rot = 0;
	cam->cam_pos.y = 7.5;
	cam->cam_y_rot = 0;
	cam->cam_pos.z = 50;
	cam->cam_z_rot = 0;
}

ControllableCamera::~ControllableCamera() {
	delete (cam);
}

/* Moves the camera based on the current movement speed set by the
 * keyboard inputs received
 * Should be called after camera_translation_keyboard() */
void ControllableCamera::move_camera() {
	cam->cam_pos.x += cam_x_speed;
	cam->cam_pos.y += cam_y_speed;
	cam->cam_pos.z += cam_z_speed;
}

Positions* ControllableCamera::getPositions() {
	return cam;
}

/** Updates the screen size
 * Should be called if the screen changes size.
 */
void ControllableCamera::screen_resize(int new_width, int new_height) {
	window_centre_x = new_width / 2;
	window_centre_y = new_height / 2;
}

// Function to convert degrees to radians
float radians(const float degrees) {
	return degrees * PI / 180.0f;
}

// Function to calculate which direction we need to move the camera and by what amount
void ControllableCamera::camera_translation_keyboard(Inputs* input) {

	// Break up our movement into components along the X, Y and Z axis
	float cam_movement_x_component = 0.0f;
	float cam_movement_y_component = 0.0f;
	float cam_movement_z_component = 0.0f;

	if(input->forward || input->backward){

		float pitch_factor = float(cos(radians(cam->cam_x_rot)));
		float yaw_factor = float(cos(radians(cam->cam_x_rot)));

		if (input->forward == true) {
			// Control X-Axis movement
			cam_movement_x_component += (movement_speed
					* float(sin(radians(cam->cam_y_rot)))) * pitch_factor;

			// Control Y-Axis movement
			cam_movement_y_component += movement_speed
					* float(sin(radians(cam->cam_x_rot))) * -1.0f;

			// Control Z-Axis movement
			cam_movement_z_component += (movement_speed
					* float(cos(radians(cam->cam_y_rot))) * -1.0f) * yaw_factor;
		}

		if (input->backward == true) {
			// Control X-Axis movement
			cam_movement_x_component += (movement_speed
					* float(sin(radians(cam->cam_y_rot))) * -1.0f) * pitch_factor;

			// Control Y-Axis movement
			cam_movement_y_component += movement_speed
					* float(sin(radians(cam->cam_x_rot)));

			// Control Z-Axis movement
			cam_movement_z_component += (movement_speed
					* float(cos(radians(cam->cam_y_rot)))) * yaw_factor;
		}

	}

	if(input->strafe_left || input->strafe_right){
		float y_rotation_radians = radians(cam->cam_y_rot);

		if (input->strafe_left == true) {
			// Calculate our Y-Axis rotation in radians once here because we use it twice
			cam_movement_x_component += -movement_speed * float(cos(y_rotation_radians));
			cam_movement_z_component += -movement_speed * float(sin(y_rotation_radians));
		}

		if (input->strafe_right == true) {
			// Calculate our Y-Axis rotation in radians once here because we use it twice
			cam_movement_x_component += movement_speed * float(cos(y_rotation_radians));
			cam_movement_z_component += movement_speed * float(sin(y_rotation_radians));
		}
	}

	// After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis
	cam_x_speed = cam_movement_x_component;
	cam_y_speed = cam_movement_y_component;
	cam_z_speed = cam_movement_z_component;

	// Cap the speeds to our movementSpeedFactor (otherwise going
	// forward and strafing at an angle is twice as fast as just going forward!)
	// X Speed cap
	if (cam_x_speed > movement_speed) {
		cam_x_speed = movement_speed;
	}
	if (cam_x_speed < -movement_speed) {
		cam_x_speed = -movement_speed;
	}

	// Y Speed cap
	if (cam_y_speed > movement_speed) {
		cam_y_speed = movement_speed;
	}
	if (cam_y_speed < -movement_speed) {
		cam_y_speed = -movement_speed;
	}

	// Z Speed cap
	if (cam_z_speed > movement_speed) {
		cam_z_speed = movement_speed;
	}
	if (cam_z_speed < -movement_speed) {
		cam_z_speed = -movement_speed;
	}
}

/** Camera movement based on mouse movement.
 * Arguments
 * ----------------------------------------------------------------------------
 * int mouse_x movement of the mouse in the x direction
 * int mouse_y movement of the mosue in the y direction
 *
 * This is usually the mouse movement from the center of the screen,
 * as after every call th mouse is reset to the center.
 */
void ControllableCamera::camera_rotation_mouse(int mouse_x, int mouse_y) {
	GLfloat vertMouseSensitivity = 10.0f;
	GLfloat horizMouseSensitivity = 10.0f;

	int horizantal_movement = mouse_x - window_centre_x;
	int vertical_movement = mouse_y - window_centre_y;

	/* If this is the first time, don't move the camera.
	 * This is so the camera is pointed at the same place when the app is
	 * started, no matter where the cursor was when the user started the
	 * program.
	 */
	bool static first_time = true;
	if(first_time){
		first_time = false;
		SDL_WarpMouse(window_centre_x, window_centre_y);
		return;
	}

	cam->cam_x_rot += vertical_movement / vertMouseSensitivity;
	cam->cam_y_rot += horizantal_movement / horizMouseSensitivity;

	// Control looking up and down with the mouse forward/back movement
	// Limit loking up to vertically up
	if (cam->cam_x_rot < -90.0f) {
		cam->cam_x_rot = -90.0f;
	}
	// Limit looking down to vertically down
	else if (cam->cam_x_rot > 90.0f) {
		cam->cam_x_rot = 90.0f;
	}

	// Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
	if (cam->cam_y_rot < -180.0f) {
		cam->cam_y_rot += 360.0f;
	} else if (cam->cam_y_rot > 180.0f) {
		cam->cam_y_rot -= 360.0f;
	}

	// Reset the mouse position to the centre of the window each frame
	SDL_WarpMouse(window_centre_x, window_centre_y);
}
