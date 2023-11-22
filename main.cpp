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
#include <glm/gtc/type_ptr.hpp>
#include "./packages/stb/stb_image.h"

#include"Shader.h"
#include"Texture.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"





//GLOBAL VARIABLES

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

GLfloat SquareVertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint SquareIndices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};



GLfloat PyramidVertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

GLuint PyramidIndices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

int modelLocation, viewLocation, projectionLocation;

void CombineTransformations(GLuint shaderID) {
	view = glm::translate(view, glm::vec3(0.0, -0.5f, -2.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);

	modelLocation = glGetUniformLocation(shaderID, "model");
	viewLocation = glGetUniformLocation(shaderID, "view");
	projectionLocation = glGetUniformLocation(shaderID, "projection");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

int main() {

    if (!glfwInit()) {
        return -1;
    }
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "YoutubeOpenGL", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK) {
        return -1;
    }


	glEnable(GL_DEPTH_TEST);


	Shader shaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	Shader lightShader("./shaders/light-vertex.glsl", "./shaders/light-fragment.glsl");


	// Pyramid Model
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(PyramidVertices, sizeof(PyramidVertices));
	EBO EBO1(PyramidIndices, sizeof(PyramidIndices));

	VAO1.LinkAttributes(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), 0);
	VAO1.LinkAttributes(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttributes(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttributes(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Light Cube
	VAO lightVAO;
	lightVAO.Bind();
	
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttributes(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	//------------------

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(0.0f,1.5f,0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPosition);

	glm::vec3 pyramidPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPosition);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID,"model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
	//Texture
	Texture popCat("./pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	GLuint uniID_tex0 = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(uniID_tex0, 0);

	//
	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		
		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "CameraPosition"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "CameraMatrix");
		popCat.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(PyramidIndices)/sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "CameraMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

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

