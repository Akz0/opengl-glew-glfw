#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "./packages/stb/stb_image.h"

#include "Shader.h"

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	
	bool firstClick = true;

	int height;
	int width;

	float speed = 1.0f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);

	// Updates the camera matrix to the Vertex Shader
	void UpdateMatrix(float FieldOfView, float NearPlane, float FarPlane);
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);


};
#endif // !CAMERA_CLASS_H
