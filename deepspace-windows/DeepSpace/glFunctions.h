#ifndef GL_FUNC_H
#define GL_FUNC_H

#include <glew.h>
#include <glfw3.h>
#include <freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <hash_map>
#include <SOIL.h>

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "SOIL.lib")

using namespace std;

extern GLuint getTexture(string Filename);

class Shader
{
public:
    enum ShaderType {SHADER_TYPE_VERTEX, SHADER_TYPE_FRAGMENT};

    Shader(const GLchar* shaderString, ShaderType shaderType);
    Shader(int shaderResourceId, ShaderType shaderType);
    GLuint getShaderHandle();
    ShaderType getShaderType();
    GLchar* getShaderString();
	bool isSuccessful();
	~Shader();

private:
    GLuint shaderHandle;
	ShaderType shaderType;
    GLchar* shaderString;   
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

	GLint getAttribute(std::string key);
	GLint getUniform(string key);

private:
	GLuint programHandle;
	GLuint vertexShader;
	GLuint fragmentShader;
	bool allOk;

	//Attributes 
	hash_map <string, GLuint> attribs;
	hash_map <string, GLuint> uniforms;	
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

#endif