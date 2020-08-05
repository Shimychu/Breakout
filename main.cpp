#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"
#include "ResourceManager.h"

#include <iostream>


// GLFW functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window Screen dimensions
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Glad load all Open GL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
	}

	// Initalize game
	Breakout.Init();

	// Time variable
	float dTime = 0.0f;
	float lastFrame = 0.0f;

	// Starting game
	Breakout.State = GAME_MENU;

	while (!glfwWindowShouldClose(window))
	{
		// Delta Time
		float currentFrame = glfwGetTime();
		dTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// Manager user Input
		Breakout.ProcessInput(dTime);

		// Update game state
		Breakout.Update(dTime);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using resource manager
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Close on pressing esc
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			Breakout.Keys[key] = false;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Make sure view port matches width/height
	glViewport(0, 0, width, height);
}