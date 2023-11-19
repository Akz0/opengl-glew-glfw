#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <process.h>
#include <fstream>
#include <string>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"





//GLOBAL VARIABLES

GLuint buffer;
GLuint IndexBuffer;
GLint TransformationMatrixLocation;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;

const char* VertexShaderFileName = "./shaders/vertex.glsl";
const char* FragmentShaderFileName = "./shaders/fragment.glsl";

GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};




void CombineTransformations() {
	static float Scale = 0.4f;
	static float Angle = 0.0f;
	static float Shift = 0.0f;

	static float Scale_Delta = 0.01f;
	static float Shift_Delta = 0.01f;
	static float Angle_Delta = 0.03f;


	Scale += Scale_Delta;
	Shift += Shift_Delta;
	Angle += Angle_Delta;

	if (Scale >= 1.0f || Scale <= 0.1) Scale_Delta *= -1.0f;
	if (Shift >= 1.5f || Shift <= -1.5) Shift_Delta *= -1.0f;

	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, Shift));
	glm::mat4 RotationMatrixZ = glm::rotate(glm::mat4(1.0f), Angle, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 TransformMatrix = ScalingMatrix;

	glUniformMatrix4fv(TransformationMatrixLocation, 1, GL_TRUE, &TransformMatrix[0][0]);

}

int main() {

    if (!glfwInit()) {
        return -1;
    }
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK) {
        return -1;
    }


	Shader shaderProgram(VertexShaderFileName, FragmentShaderFileName);



	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();




    while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
    }


	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

