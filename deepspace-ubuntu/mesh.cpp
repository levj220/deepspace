/*
 * mesh.cpp
 *
 *  Created on: Jun 20, 2013
 *      Author: smolin
 */
#include "mesh.h"

Mesh::Mesh(int gridSize)
{
    const int COORDS_PER_VERTEX = 4;
    const int TEXCOORDS_PER_VERTEX = 2;

    vertexShader = new Shader("Shaders/vertex.glsl", Shader::SHADER_TYPE_VERTEX);
    fragmentShader = new Shader("Shaders/fragment.glsl", Shader::SHADER_TYPE_FRAGMENT);
    program = new Program(vertexShader, fragmentShader);
	glUseProgram(program->getProgramHandle());

    int nPoints = gridSize * gridSize * COORDS_PER_VERTEX;
    int nTexPoints = gridSize * gridSize * TEXCOORDS_PER_VERTEX;

    GLfloat *vertices = new GLfloat[nPoints];
    GLfloat *texs = new GLfloat[nTexPoints];
    GLuint *indices;

    // Used to center the mesh
    float offset = ((float) (gridSize - 1)) / 2.0f;

    // For computing texture coordinates
    float texCoordDelta = 1.0f / (float) (gridSize);

    int v = 0;
    int t = 0;
    for (int x = 0; x < (int) (gridSize); x++)
    {
        for (int y = 0; y < (int) (gridSize); y++)
        {
            vertices[v++] = (float)(x) - offset;
            vertices[v++] = 0.0f;
			vertices[v++] = (float)(y) - offset;
            vertices[v++] = 1.0f;

            texs[t++] = (float)(x) * texCoordDelta;
            texs[t++] = (float)(y) * texCoordDelta;
        }
    }

    nIndices = 2 * ((gridSize - 1) * (gridSize - 1) + (gridSize - 1));
	indices = new GLuint[nIndices];

	GLuint rowSteps = (gridSize - 1);
    GLuint colSteps =  (2 * gridSize);

	//Compute the indices to build a triangle strip out of
	v = 0;
	GLuint n = 0;
    for (GLuint row = 0; row < rowSteps; row++)
    {
        for (GLuint col = 0; col < colSteps; col++)
        {
                GLuint i = col + row * colSteps;

                if (col == colSteps - 1)
                {
                    indices[v++] = n;
                }
                else
                {
                    indices[v++] = n;

                    if (i % 2 == 0)
                    {
                        n += gridSize;
                    }
                    else
                    {
                        if (row % 2 == 0)
                        {
                            n -= (gridSize - 1);
                        }
                        else
                        {
                            n -= (gridSize + 1);
                        }
                    }
                }
        }
    }

	heightMap = getTexture("Textures/yoshi_low.png");
	baseImage = getTexture("Textures/yoshi_low.png");

	glUniform1i(program->getUniform("displacementMap"), 0);
	glUniform1i(program->getUniform("baseImage"), 1);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nPoints, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nTexPoints, texs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * nIndices, indices, GL_STATIC_DRAW);


    GLuint errorCheckValue = glGetError();
    if (errorCheckValue != GL_NO_ERROR)
    {
        fprintf(stderr, "ERROR: Could not create a VBO: %s \n", gluErrorString(errorCheckValue));
        return;
    }
}



void Mesh::draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
	//Sent the matrices to the shader
    glUniformMatrix4fv(program->getUniform("projMatrix"), 1, false, glm::value_ptr(proj));
    glUniformMatrix4fv(program->getUniform("viewMatrix"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(program->getUniform("modelMatrix"), 1, false, glm::value_ptr(model));

	//Bind the textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, baseImage);

	glDrawElements(GL_TRIANGLE_STRIP, nIndices, GL_UNSIGNED_INT, NULL);
}

Mesh::~Mesh()
{
    if(program != NULL)
    {
        delete program;
    }

    if(vertexShader != NULL)
    {
        delete vertexShader;
    }

    if(fragmentShader != NULL)
    {
        delete fragmentShader;
    }
}


