#include "Mesh.h"
	
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

Vertex vertices[] = 
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_FRONT);
	glEnable(GL_CW);


	Shader shaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	Shader floorShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");

	Texture textures[]
	{
		Texture("./textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	Texture floorTextures[]
	{
		Texture("./textures/planks.png", "diffuse", 2, GL_RGBA, GL_UNSIGNED_BYTE),
	};

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> floor_tex(floorTextures, floorTextures + sizeof(floorTextures) / sizeof(Texture));

	Mesh floor(verts, ind, tex);
	Mesh floor2(verts, ind, floor_tex);


	Shader lightShader("./shaders/light-vertex.glsl", "./shaders/light-fragment.glsl");
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(0.0f,0.5f,0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPosition);
	lightModel = glm::scale(lightModel, glm::vec3(0.5, 0.5, 0.5));

	glm::vec3 pyramidPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPosition);

	glm::vec3 floorPosition = glm::vec3(0.0f, 0.2f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	floorModel = glm::translate(floorModel, floorPosition);
	floorModel = glm::scale(floorModel, glm::vec3(0.5, 0.5, 0.5));


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID,"model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);

	floorShaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(floorShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);


	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
	static float angle = 0;

    while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		angle =0.01;

		pyramidModel = glm::rotate(pyramidModel, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "CameraPosition"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
		camera.Matrix(shaderProgram, "CameraMatrix");

		floorShaderProgram.Activate();
		glUniform3f(glGetUniformLocation(floorShaderProgram.ID, "CameraPosition"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(floorShaderProgram, "CameraMatrix");

		lightShader.Activate();
		camera.Matrix(lightShader, "CameraMatrix");

		floor.Draw(shaderProgram, camera, false);
		floor2.Draw(floorShaderProgram, camera, false);
		light.Draw(lightShader, camera,false);

		glfwSwapBuffers(window);
		glfwPollEvents();
    }

	shaderProgram.Delete();
	lightShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

