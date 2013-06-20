/*
 * mesh.h
 *
 *  Created on: Jun 20, 2013
 *      Author: smolin
 */

#ifndef MESH_H_
#define MESH_H_

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

#endif /* MESH_H_ */
