#define GLEW_STATIC
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "libglew32.lib")
#pragma comment(lib, "freeglut_static.lib")

#include <glew.h>
#include <freeglut.h>

#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Camera.h"

using namespace std;



glm::mat4 LookAt(const glm::vec3& Eye, const glm::vec3& Center, const glm::vec3& Up)
{
	glm::mat4 Matrix;
	glm::vec3 X, Y, Z;
	Z = Eye - Center;
	glm::normalize(Z); //Z.glm::Normalize();
	Y = Up;
	
	X = glm::cross(Y, Z); //X = Y.Cross( Z );
	Y = glm::cross(Z, X); //Y = Z.Cross( X );

	glm::normalize(X); //X.Normalize();
	glm::normalize(Y); //Y.Normalize();
	Matrix[0][0] = X.x;
	Matrix[1][0] = X.y;
	Matrix[2][0] = X.z;

	Matrix[3][0] = glm::dot(-X, Eye); //Matrix[3][0] = -X.Dot(Eye);

	Matrix[0][1] = Y.x;
	Matrix[1][1] = Y.y;
	Matrix[2][1] = Y.z;
	
	Matrix[3][1] = glm::dot(-Y, Eye); //Matrix[3][1] = -Y.Dot(Eye);
	
	Matrix[0][2] = Z.x;
	Matrix[1][2] = Z.y;
	Matrix[2][2] = Z.z;
	Matrix[3][2] = glm::dot(-Z, Eye); //Matrix[3][2] = -Z.Dot(Eye);
	Matrix[0][3] = 0;
	Matrix[1][3] = 0;
	Matrix[2][3] = 0;
	Matrix[3][3] = 1.0f;
	return Matrix;
}



/*
Camera::Camera() {
	camera_mode = FREE;
	camera_up = glm::vec3(0, 1, 0);
	field_of_view = 45;
	camera_position_delta = glm::vec3(0, 0, 0);
	camera_scale = .5f;
	max_pitch_rate = 5;
	max_heading_rate = 5;
	move_camera = false;
}
Camera::~Camera() {
}

void Camera::Reset() {
	camera_up = glm::vec3(0, 1, 0);
}

void Camera::Update() {
	camera_direction = glm::normalize(camera_look_at - camera_position);
	//need to set the matrix state. this is only important because lighting doesn't work if this isn't done
	glViewport(viewport_x, viewport_y, window_width, window_height);

	if (camera_mode == ORTHO) {
		//our projection matrix will be an orthogonal one in this case
		//if the values are not floating point, this command does not work properly
		//need to multiply by aspect!!! (otherise will not scale properly)
		projection = glm::ortho(-1.5f * float(aspect), 1.5f * float(aspect), -1.5f, 1.5f, -10.0f, 10.f);
	} else if (camera_mode == FREE) {
		projection = glm::perspective(field_of_view, aspect, near_clip, far_clip);
		//detmine axis for pitch rotation
		glm::vec3 axis = glm::cross(camera_direction, camera_up);
		//compute quaternion for pitch based on the camera pitch angle
		glm::quat pitch_quat = glm::angleAxis(camera_pitch, axis);
		//determine heading quaternion from the camera up vector and the heading angle
		glm::quat heading_quat = glm::angleAxis(camera_heading, camera_up);
		//add the two quaternions
		glm::quat temp = glm::cross(pitch_quat, heading_quat);
		temp = glm::normalize(temp);
		//update the direction from the quaternion
		camera_direction = glm::rotate(temp, camera_direction);
		//add the camera delta
		camera_position += camera_position_delta;
		//set the look at to be infront of the camera
		camera_look_at = camera_position + camera_direction * 1.0f;
		//damping for smooth camera
		camera_heading *= .5;
		camera_pitch *= .5;
		camera_position_delta = camera_position_delta * .8f;
	}
	//compute the MVP
	view = glm::lookAt(camera_position, camera_look_at, camera_up);
	model = glm::mat4(1.0f);
	MVP = projection * view * model;
}

//Setting Functions
void Camera::SetMode(CameraType cam_mode) {
	camera_mode = cam_mode;
	camera_up = glm::vec3(0, 1, 0);
}

void Camera::SetPosition(glm::vec3 pos) {
	camera_position = pos;
}

void Camera::SetLookAt(glm::vec3 pos) {
	camera_look_at = pos;
}
void Camera::SetFOV(double fov) {
	field_of_view = fov;
}
void Camera::SetViewport(int loc_x, int loc_y, int width, int height) {
	viewport_x = loc_x;
	viewport_y = loc_y;
	window_width = width;
	window_height = height;
	//need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
	aspect = double(width) / double(height);
	;
}
void Camera::SetClipping(double near_clip_distance, double far_clip_distance) {
	near_clip = near_clip_distance;
	far_clip = far_clip_distance;
}

void Camera::Move(CameraDirection dir) {
	if (camera_mode == FREE) {
		switch (dir) {
			case UP:
				camera_position_delta += camera_up * camera_scale;
				break;
			case DOWN:
				camera_position_delta -= camera_up * camera_scale;
				break;
			case LEFT:
				camera_position_delta -= glm::cross(camera_direction, camera_up) * camera_scale;
				break;
			case RIGHT:
				camera_position_delta += glm::cross(camera_direction, camera_up) * camera_scale;
				break;
			case FORWARD:
				camera_position_delta += camera_direction * camera_scale;
				break;
			case BACK:
				camera_position_delta -= camera_direction * camera_scale;
				break;
		}
	}
}
void Camera::ChangePitch(float degrees) {
	//Check bounds with the max pitch rate so that we aren't moving too fast
	if (degrees < -max_pitch_rate) {
		degrees = -max_pitch_rate;
	} else if (degrees > max_pitch_rate) {
		degrees = max_pitch_rate;
	}
	camera_pitch += degrees;

	//Check bounds for the camera pitch
	if (camera_pitch > 360.0f) {
		camera_pitch -= 360.0f;
	} else if (camera_pitch < -360.0f) {
		camera_pitch += 360.0f;
	}
}
void Camera::ChangeHeading(float degrees) {
	//Check bounds with the max heading rate so that we aren't moving too fast
	if (degrees < -max_heading_rate) {
		degrees = -max_heading_rate;
	} else if (degrees > max_heading_rate) {
		degrees = max_heading_rate;
	}
	//This controls how the heading is changed if the camera is pointed straight up or down
	//The heading delta direction changes
	if (camera_pitch > 90 && camera_pitch < 270 || (camera_pitch < -90 && camera_pitch > -270)) {
		camera_heading -= degrees;
	} else {
		camera_heading += degrees;
	}
	//Check bounds for the camera heading
	if (camera_heading > 360.0f) {
		camera_heading -= 360.0f;
	} else if (camera_heading < -360.0f) {
		camera_heading += 360.0f;
	}
}
void Camera::Move2D(int x, int y) {
	//compute the mouse delta from the previous mouse position
	glm::vec3 mouse_delta = mouse_position - glm::vec3(x, y, 0);
	//if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
	if (move_camera) {
		ChangeHeading(.08f * mouse_delta.x);
		ChangePitch(.08f * mouse_delta.y);
	}
	mouse_position = glm::vec3(x, y, 0);
}

void Camera::SetPos(int button, int state, int x, int y) {
	if (button == 3 && state == GLUT_DOWN) {
		camera_position_delta += camera_up * .05f;
	} else if (button == 4 && state == GLUT_DOWN) {
		camera_position_delta -= camera_up * .05f;
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		move_camera = true;
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		move_camera = false;
	}
	mouse_position = glm::vec3(x, y, 0);
}

CameraType Camera::GetMode() {
	return camera_mode;
}

void Camera::GetViewport(int &loc_x, int &loc_y, int &width, int &height) {
	loc_x = viewport_x;
	loc_y = viewport_y;
	width = window_width;
	height = window_height;
}

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M) {
	P = projection;
	V = view;
	M = model;
}
*/