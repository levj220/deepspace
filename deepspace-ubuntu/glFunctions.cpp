/*
 * glFunctions.cpp
 *
 *  Created on: Jun 20, 2013
 *      Author: smolin
 */

/*
 * glFunctions.cpp
 *
 *  Created on: Jun 20, 2013
 *      Author: smolin
 */

#include <cstdio>
#include <fstream>
#include <string>
#include "glFunctions.h"

using namespace std::tr1;

const GLchar* loadTextFile(const char* path);

//////////////////////////////////////////////////////////////////
/////////////////////PROGRAM CLASS FUNCTIONS//////////////////////
//////////////////////////////////////////////////////////////////
Program::Program(Shader* vertexShader, Shader* fragmentShader)
{
	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertexShader->getShaderHandle());
	glAttachShader(programHandle, fragmentShader->getShaderHandle());
	glLinkProgram(programHandle);

	int status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		fprintf(stderr, "Could not link program.");
		allOk = false;
		glDeleteProgram(programHandle);
		return;
	}
	else
	{
		printf("Program linked successfully.\n");
	}

	this->vertexShader = vertexShader->getShaderHandle();
	this->fragmentShader = fragmentShader->getShaderHandle();
	allOk = true;
}

GLuint Program::getProgramHandle()
{
	return programHandle;
}

bool Program::isSuccessful()
{
	return allOk;
}

/**
 Serach the program for a given attribue.
 */
GLint Program::getAttribute(const char* name)
{
	//First check to see if we have the value stored in our hashmap
	unordered_map<const char*, GLuint>::iterator a = uniforms.find(name);

	if (a != attribs.end())
	{
		//if it exists, return it
		return a->second;
	}
	else
	{
		//Otherwise, look for it in the program and store it
		GLuint attrib = glGetAttribLocation(programHandle, name);
		attribs[name] = attrib;
		return attrib;
	}
}

/**
 Serach the program for a given attribue.
 */
GLint Program::getUniform(const char* name)
{
	//First check to see if we have the value stored in our hashmap
	unordered_map<const char*, GLuint>::iterator u = uniforms.find(name);

	if (u != uniforms.end())
	{
		return u->second;
	}
	else
	{
		GLuint unif = glGetUniformLocation(programHandle, name);
		uniforms[name] = unif;
		return unif;
	}
}

Program::~Program()
{
	glDetachShader(programHandle, vertexShader);
	glDetachShader(programHandle, fragmentShader);
	glDeleteProgram(programHandle);
}

//////////////////////////////////////////////////////////////////
/////////////////////END///END///END///END////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
/////////////////////SHADER CLASS FUNCTIONS///////////////////////
//////////////////////////////////////////////////////////////////

/**
 Load a shader from a string.  This class will own the memory of the string and delete it when necessary.

 Shader::Shader(const GLchar* string, ShaderType shaderType)
 {
 if (shaderString == NULL)
 {
 allOk = false;
 return;
 }

 int textSize = sizeof(string);

 this->shaderString = new GLchar[textSize + 1];
 memcpy(shaderString, string, textSize);
 shaderString[textSize] = '\0';

 printf("Testing: %s", shaderString);
 this->shaderType = shaderType;
 compileShader();
 }*/

/*
 Load a shader from a resource. The resource must be in the current module and be stored of type "TEXT"
 */
Shader::Shader(const char* path, ShaderType shaderType)
{
	this->shaderString = loadTextFile(path);

	if (shaderString == NULL)
	{
		allOk = false;
		return;
	}

	this->shaderType = shaderType;
	compileShader();
}

GLuint Shader::compileShader()
{
	if (shaderType == SHADER_TYPE_VERTEX)
	{
		this->shaderHandle = glCreateShader(GL_VERTEX_SHADER);
	}
	else
	{
		this->shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glShaderSource(this->shaderHandle, 1, (const GLchar**) &this->shaderString,
			NULL);
	glCompileShader(this->shaderHandle);

	int compileStatus;
	glGetShaderiv(this->shaderHandle, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE)
	{
		if (shaderType == SHADER_TYPE_VERTEX)
		{
			fprintf(stderr, "Could not compile vertex shader!\n");
			allOk = false;
		}
		else
		{
			fprintf(stderr, "Could not compile fragment shader!\n");
			this->shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		}
		glDeleteShader(shaderHandle);
	}
	else
	{
		if (shaderType == SHADER_TYPE_VERTEX)
		{
			printf("Vertex shader compiled successfully.\n");
		}
		else
		{
			printf("Fragment shader compiled successfully.\n");
		}
		allOk = true;
	}
	return 0;
}

bool Shader::isSuccessful()
{
	return allOk;
}

GLuint Shader::getShaderHandle()
{
	return shaderHandle;
}

const GLchar* Shader::getShaderString()
{
	return shaderString;
}

Shader::ShaderType Shader::getShaderType()
{
	return shaderType;
}

Shader::~Shader()
{
	//glDeleteShader(shaderHandle);
	delete[] shaderString;
}

//////////////////////////////////////////////////////////////////
/////////////////////END///END///END///END////////////////////////
//////////////////////////////////////////////////////////////////

/**
 Loads a shader from a resource TEXT file and returns its NULL terminated string.
 */
const GLchar* loadTextFile(const char* path)
{
	std::ifstream file;

	file.open(path, std::ifstream::in);

	std::string str;
	std::string file_contents;
	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}

	return file_contents.c_str();
}

GLuint getTexture(const char* filename)
{
	GLuint tex_ID;

	tex_ID = SOIL_load_OGL_texture(filename, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS);

	if (tex_ID > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);
		return tex_ID;
	}
	else
	{
		fprintf(stderr, "Could not load texture.");
		return -1;
	}
}

