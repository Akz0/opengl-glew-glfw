#include "Mesh.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <tuple>

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


std::pair<std::vector<Vertex>, std::vector<GLuint>> ProcessMesh(const aiMesh* mesh) {
	// Vector to store unique vertices
	std::vector<Vertex> uniqueVertices;
	// Vector to store indices
	std::vector<unsigned int> indices;

	// Iterate over each face in the mesh
	for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
		const aiFace& face = mesh->mFaces[faceIndex];

		// Add indices for each vertex in the face
		for (unsigned int index = 0; index < face.mNumIndices; ++index) {
			unsigned int vertexIndex = face.mIndices[index];
			Vertex currentVertex;
			currentVertex.position = glm::vec3(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z);
			currentVertex.normal = glm::vec3(mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z);
			currentVertex.texUV = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);

			// Check if the vertex with the same position, normal, and texture UV is already in uniqueVertices
			auto key = std::make_tuple(currentVertex.position,currentVertex.normal,currentVertex.texUV);
			auto it = std::find_if(uniqueVertices.begin(), uniqueVertices.end(),
				[&key](const Vertex& v) {
					return std::tie(v.position,v.normal,v.texUV) == key;
				});

			// If not found, add it to uniqueVertices
			if (it == uniqueVertices.end()) {
				uniqueVertices.push_back(currentVertex);
				indices.push_back(static_cast<unsigned int>(uniqueVertices.size() - 1));
			}
			else {
				// If found, use the existing index
				indices.push_back(static_cast<unsigned int>(std::distance(uniqueVertices.begin(), it)));
			}
		}
	}

	// Print unique vertices and indices
	/*
	std::cout << "Unique Vertices:\n";
	for (const auto& vertex : uniqueVertices) {
		std::cout << "Position: (" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << "), ";
		std::cout << "Normal: (" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << "), ";
		std::cout << "Texture Coordinate: (" << vertex.texUV.x << ", " << vertex.texUV.y << ")\n";
	}
	*/

	std::cout << "Unique Vertices: " << uniqueVertices.size() << std::endl;
	std::cout << "Indices: "<<indices.size()<<std::endl;

	return std::make_pair(uniqueVertices, indices);
}


int main() {

	if (!glfwInit()) {
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Project", NULL, NULL);

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
	Shader floorShaderProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	Shader lightShader("./shaders/light-vertex.glsl", "./shaders/light-fragment.glsl");

	Texture textures[]
	{
		Texture("./textures/rubik.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
	};

	Texture House[]
	{
		Texture("./textures/cottage_diffuse.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
	};

	Texture floorTextures[]
	{
		Texture("./textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("./textures/planksSpec.png","specular",1,GL_RED,GL_UNSIGNED_BYTE)
	};

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> floor_tex(floorTextures, floorTextures + sizeof(floorTextures) / sizeof(Texture));

	Mesh floor(verts, ind, floor_tex);
	Mesh floor2(verts, ind, floor_tex);

	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(0.0f, 8.5f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPosition);
	lightModel = glm::scale(lightModel, glm::vec3(0.5, 0.5, 0.5));

	glm::vec3 ObjectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 ObjectModel = glm::mat4(1.0f);
	ObjectModel = glm::translate(ObjectModel, ObjectPosition);
	ObjectModel = glm::scale(ObjectModel, glm::vec3(0.1f, 0.1f, 0.1f));

	glm::vec3 floorPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	floorModel = glm::translate(floorModel, floorPosition);
	floorModel = glm::scale(floorModel, glm::vec3(100.0f, 100.0f, 100.0f));

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(ObjectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);

	floorShaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(floorShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
	glUniform4f(glGetUniformLocation(floorShaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(floorShaderProgram.ID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);

	std::vector<Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Mesh> Objects;
	std::vector <Mesh> LightCube;

	Assimp::Importer importer;

	// Load the model
	const aiScene* scene = importer.ReadFile("./models/rubik.obj",
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
		return -1;
	}

	// Process each mesh in the scene
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		std::cout << "Mesh : " << meshIndex + 1 << std::endl;
		auto result = ProcessMesh(mesh);
		vertices = result.first;
		indices = result.second;
		Objects.push_back(Mesh(vertices, indices, tex));
		std::cout << "\n";

	}



	scene = importer.ReadFile("./models/ball.obj",
		aiProcess_Triangulate | aiProcess_FlipUVs| aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
		return -1;
	}

	// Process each mesh in the scene
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		std::cout << "Mesh : " << meshIndex + 1 << std::endl;
		auto result = ProcessMesh(mesh);
		vertices = result.first;
		indices = result.second;
		LightCube.push_back(Mesh(vertices, indices, tex));
	}



	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 1.0f, 2.0f));
	static float angle = 0;

    while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		angle =0.01;

		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "CameraPosition"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(ObjectModel));
		camera.Matrix(shaderProgram, "CameraMatrix");

		floorShaderProgram.Activate();
		glUniform3f(glGetUniformLocation(floorShaderProgram.ID, "CameraPosition"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniformMatrix4fv(glGetUniformLocation(floorShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
		camera.Matrix(floorShaderProgram, "CameraMatrix");

		lightShader.Activate();
		camera.Matrix(lightShader, "CameraMatrix");

		floor.Draw(floorShaderProgram, camera, false);

		for (auto it : Objects) {
			it.Draw(shaderProgram, camera, false);
		}

		for (auto it : LightCube) {
			it.Draw(lightShader, camera, false);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
    }

	shaderProgram.Delete();
	lightShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

