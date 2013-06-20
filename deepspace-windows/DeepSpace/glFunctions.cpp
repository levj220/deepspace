#include <Windows.h>
#include <stdio.h>
#include "glFunctions.h"
#include "resource.h"

GLchar* loadTextResource(int resourceId);

//////////////////////////////////////////////////////////////////
/////////////////////PROGRAM CLASS FUNCTIONS//////////////////////
//////////////////////////////////////////////////////////////////

Program::Program()
{

}
Program::Program(Shader* vertexShader, Shader* fragmentShader)
{
    programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader->getShaderHandle());
    glAttachShader(programHandle, fragmentShader->getShaderHandle());
    glLinkProgram(programHandle);

    int status;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &status);

    if(status != GL_TRUE)
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
GLint Program::getAttribute(std::string name)
{
    //First check to see if we have the value stored in our hashmap
    std::hash_map<string, GLuint>::iterator a = attribs.find(name);

    if(a != attribs.end())
    {
        //if it exists, return it
        return a->second;
    }
    else
    {
        //Otherwise, look for it in the program and store it
        GLuint attrib = glGetAttribLocation(programHandle, name.c_str());
        attribs[name] = attrib;
        return attrib;
    }
}

/**
Serach the program for a given attribue.
*/
GLint Program::getUniform(std::string name)
{
    //First check to see if we have the value stored in our hashmap
    std::hash_map<string, GLuint>::iterator u = uniforms.find(name);

    if(u != uniforms.end())
    {
        return u->second;
    }
    else
    {
        GLuint unif = glGetUniformLocation(programHandle, name.c_str());
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
*/
Shader::Shader(const GLchar* string, ShaderType shaderType)
{
    if(shaderString == NULL)
    {
        allOk = false;
        return;
    }

    int textSize = sizeof(string);

    this->shaderString = new GLchar[textSize + 1];
    memcpy_s(shaderString, textSize, string, textSize);
    shaderString[textSize] = '\0';

    printf("Testing: %s", shaderString);
    this->shaderType = shaderType;
    compileShader();
}

/*
Load a shader from a resource. The resource must be in the current module and be stored of type "TEXT"
*/
Shader::Shader(int shaderResourceId, ShaderType shaderType)
{
    this->shaderString = loadTextResource(shaderResourceId);

    if(shaderString == NULL)
    {
        allOk = false;
        return;
    }

    this->shaderType = shaderType;
    compileShader();
}


GLuint Shader::compileShader()
{
    if(shaderType == SHADER_TYPE_VERTEX)
    {
        this->shaderHandle = glCreateShader(GL_VERTEX_SHADER);
    }
    else
    {
        this->shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    }

    glShaderSource(this->shaderHandle, 1, (const GLchar**) &this->shaderString, NULL);
    glCompileShader(this->shaderHandle);

    int compileStatus;
    glGetShaderiv(this->shaderHandle, GL_COMPILE_STATUS, &compileStatus);

    if(compileStatus != GL_TRUE)
    {
        if(shaderType == SHADER_TYPE_VERTEX)
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
        if(shaderType == SHADER_TYPE_VERTEX)
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

GLchar* Shader::getShaderString()
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



//////////////////////////////////////////////////////////////////
///////////////////////VBO CLASS FUNCTIONS////////////////////////
//////////////////////////////////////////////////////////////////
VBO::VBO()
{
    vertices = NULL;
    indices = NULL;
    colors = NULL;
}

void VBO::setVertices(GLfloat *vData)
{
}

void VBO::setColors(GLfloat *vData)
{
}

void VBO::setIndices(GLushort *vData)
{
}

void VBO::setTexCoords(GLfloat *texCoords)
{
}

GLuint VBO::create(void)
{
    if(vertices == NULL)
    {
        return -1;
    }

    const size_t bufferSize = sizeof(vertices);
    const size_t vertexSize = sizeof(vertices);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
    glEnableVertexAttribArray(0);

    return 1;

}




VBO::~VBO()
{
    if(vertices != NULL)
    {
        delete[] vertices;
    }

    if(indices != NULL)
    {
        delete[] indices;
    }

    if(colors != NULL)
    {
        delete[] colors;
    }

    if(texCoords != NULL)
    {
        delete[] texCoords;
    }
}
//////////////////////////////////////////////////////////////////
/////////////////////END///END///END///END////////////////////////
//////////////////////////////////////////////////////////////////


/**
Loads a shader from a resource TEXT file and returns its NULL terminated string.
*/
GLchar* loadTextResource(int resourceId)
{
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceId), "TEXT");

    if (hResource)
    {
        HGLOBAL hLoadedResource = LoadResource(NULL, hResource);
        DWORD dwResourceSize = SizeofResource(NULL, hResource);


        if (hLoadedResource)
        {
            const char* pLockedResource = (const char*)LockResource(hLoadedResource);

            if (pLockedResource)
            {
                if (dwResourceSize != 0)
                {
                    GLchar* shader = new GLchar[dwResourceSize + 1];
                    memcpy_s(shader, dwResourceSize, pLockedResource, dwResourceSize);
                    shader[dwResourceSize] = '\0';
                    return shader;
                }
            }
        }
    }

    return NULL;
}


GLuint getTexture(std::string Filename)
{
    GLuint tex_ID;

	tex_ID = SOIL_load_OGL_texture(
                 Filename.c_str(),
				 SOIL_LOAD_RGBA,
                 SOIL_CREATE_NEW_ID,
                 SOIL_FLAG_POWER_OF_TWO
                 | SOIL_FLAG_MIPMAPS
                 | SOIL_FLAG_MULTIPLY_ALPHA
                 | SOIL_FLAG_COMPRESS_TO_DXT
                 | SOIL_FLAG_DDS_LOAD_DIRECT
                 | SOIL_FLAG_INVERT_Y
             );

    if( tex_ID > 0 )
    {
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, tex_ID );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        return tex_ID;
    }
    else
        return -1;
}