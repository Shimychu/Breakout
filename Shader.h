#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	void SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
	void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
	void SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
	void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

private:
	void checkCompileErrors(unsigned int object, std::string type);

};

#endif

