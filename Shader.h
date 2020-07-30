#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	// State
	unsigned int ID;

	//Constructor
	Shader() { }

	//sets the current shader as active
	Shader& Use();

	//Compiles the shader from given source code
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	//Utility function
	void SetFloat(const char *name, float value, bool useShader = false);
	void SetInteger(const char *name, int value, bool useShader = false);
	void SetVector2f(const char* name, float x, float y, bool useShader = false);

};

