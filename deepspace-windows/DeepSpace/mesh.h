#ifndef MESH_H
#define MESH_H

#include "glFunctions.h"

class Mesh
{
public:
	Mesh(int nPoints);
	void draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj);
	~Mesh();

private:
	Shader *vertexShader;
	Shader *fragmentShader;
	Program *program;
	GLuint vao;
	GLuint vbo;
	GLuint tbo;
	GLuint ibo;
	GLuint nIndices;

	GLuint baseImage;
	GLuint heightMap;
};


#endif