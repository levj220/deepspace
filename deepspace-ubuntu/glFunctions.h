/*
 * glFunctions.h
 *
 *  Created on: Jun 20, 2013
 *      Author: smolin
 */

#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tr1/unordered_map>
#include <SOIL/SOIL.h>

using namespace std::tr1;

extern GLuint getTexture(const char* Filename);


class Shader
{
public:
    enum ShaderType {SHADER_TYPE_VERTEX, SHADER_TYPE_FRAGMENT};
    Shader(const char* path, ShaderType shaderType);
    GLuint getShaderHandle();
    ShaderType getShaderType();
    const GLchar* getShaderString();
	bool isSuccessful();
	~Shader();

private:
    GLuint shaderHandle;
	ShaderType shaderType;
    const GLchar* shaderString;
	bool allOk;

	GLuint compileShader();
};


class Program
{
public:
	Program();
	Program(Shader* vertexShaderHandle, Shader* fragmentShaderHandle);
	GLuint getProgramHandle();
	bool isSuccessful();
	~Program();

	GLint getAttribute(const char* key);
	GLint getUniform(const char* key);

private:
	GLuint programHandle;
	GLuint vertexShader;
	GLuint fragmentShader;
	bool allOk;

	//Attributes
	unordered_map <const char *, GLuint> attribs;
	unordered_map <const char *, GLuint> uniforms;
};

class VBO
{
public:
	VBO();
	void setVertices(GLfloat *vertices);
	void setColors(GLfloat *colors);
	void setTexCoords(GLfloat *texCoords);
	void setIndices(GLushort *indices);
	GLuint create();
	~VBO();

private:
	GLfloat* vertices;
	GLfloat* colors;
	GLushort* indices;
	GLfloat* texCoords;
	GLuint vao;
	GLuint vbo;
};


#endif /* GLFUNCTIONS_H_ */
