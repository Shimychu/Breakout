#include "Shader.h"

#include <iostream>

Shader& Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {

	unsigned int sVertex, sFragment, gShader;

	/*Vertex Shaders*/
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	//for debugging purposes we can check for comiple errors here.
	//checkCompileError(sVertex, "VERTEX")

	/*Fragment Shaders*/
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sVertex);
	//for debugging purposes we can check for comiple errors here.
	//checkCompileError(sFragment, "FRAGMENT")

	/*Geometry Source Shader*/
	if (geometrySource != nullptr) 
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		//for debugging purposes we can check for comiple errors here.
		//checkCompileError(gShader, "GEOMETRY")
	}

	/*Shader Program*/
	this->ID = glCreateProgram();
	glAttachShader(this->ID, sVertex);
	glAttachShader(this->ID, sFragment);
	if (geometrySource != nullptr) 
	{
		glAttachShader(this->ID, gShader);
	}
	glLinkProgram(this->ID);
	//for debugging purposes we can check for comiple errors here.
	//checkCompileErrors(this->ID, "PROGRAM");


	/*Clean up*/
	//Delete Shader because they're linked to program now and no longer needed.
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr) 
	{
		glDeleteShader(gShader);
	}

}

void Shader::SetFloat(const char* name, float value, bool useShader) {
	if (useShader)
	{
		this->Use();
	}
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger(const char* name, int value, bool useShader) {
	if (useShader)
	{
		this->Use();
	}
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned int object, std::string type)
{
	//TODO::fill this out 
}