#include <iostream>
#include <stdio.h>
#include "glFunctions.h"
#include "mesh.h"
#include "resource.h"

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

void initialize(GLFWwindow *window);
void draw(void);
bool glewLoad(void);
void update(double millis);
void destroy(void);
void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);

double fpsTimer;
int fps;

/**
The matrices that compose the view, model, and projection as well as a matrix for the combined matrix.
**/
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projMatrix;
glm::mat4 mVPMatrix;

Mesh *m;
float rx=0, ry=0, rz=0;

//To move in accordance with time, not frame rate
float elapsed = 0;

void resize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
	projMatrix = glm::perspective(45.0f, (float)width/(float)height, 1.0f, 2000.0f);
    viewMatrix = glm::lookAt(glm::vec3(0,0,250), glm::vec3(0,0,0), glm::vec3(0,1,0));
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }


	const GLFWvidmode *mode;
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

    /* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(mode->width, mode->height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


	glfwIconifyWindow(window);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, &resize);
    glfwSetKeyCallback(window, onKeyPress);


    /* Initialize glew*/
    if(!glewLoad())
    {
        getchar();
        return -1;
    }

    //Set up our OpenGL world*/
    initialize(window);

    double oldTime = glfwGetTime();
    fpsTimer = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // calculate time elapsed
        double currentTime = glfwGetTime();
        update(currentTime - oldTime);
        oldTime = currentTime;

        /* Render here */
        draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    destroy();
    glfwTerminate();
    return 0;
}

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static int mode = GL_FILL;

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
		if(mode == GL_FILL)
		{
			mode = GL_LINE;
		}
		else
		{
			mode = GL_FILL;
		}
		glPolygonMode(GL_FRONT_AND_BACK, mode);
    }
	else if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if(key == GLFW_KEY_LEFT)
	{
		ry += elapsed * 20;
	}
	else if(key == GLFW_KEY_RIGHT)
	{
		ry -= elapsed * 20;
	}
	else if(key == GLFW_KEY_UP)
	{
		rx += elapsed * 20;
	}
	else if(key == GLFW_KEY_DOWN)
	{
		rx -= elapsed * 20;
	}
}

//All updating should be done here
void update(double millis)
{
	//Store the elapsed time so we can use it for key presses
    elapsed = (float)millis;
}

bool glewLoad(void)
{
    GLenum glewInitResult;

    glewInitResult = glewInit();

    if(glewInitResult != GLEW_OK)
    {
        fprintf(
            stderr,
            "ERROR: %s\n",
            glewGetErrorString(glewInitResult));

        return false;
    }

    //Print the version number to the screen
    fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));
    return true;
}

void destroy()
{
    delete m;
}

void initialize(GLFWwindow *window)
{
    //First try to initialize glew
    glewInit();

	//Now set up our projection and view matrices
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    projMatrix = glm::perspective(45.0f, (float)width / (float)height, 1.0f, 2000.0f);
    viewMatrix = glm::lookAt(glm::vec3(0,0,250), glm::vec3(0,0,0), glm::vec3(0,1,0));

	//Enable stuff
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glEnable(GL_DEPTH_TEST);

	//Create one time objects
    m = new Mesh(350);
}

void draw(void)
{
    if(glfwGetTime() - fpsTimer > 1.0)
    {
        printf("FPS: %d\n", fps);
        fps = 0;
        fpsTimer = glfwGetTime();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    modelMatrix = glm::mat4();
    modelMatrix = glm::rotate(modelMatrix, rx, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, ry, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rz, glm::vec3(0.0f, 0.0f, 1.0f));
    m->draw(modelMatrix, viewMatrix, projMatrix);

    fps++;
}