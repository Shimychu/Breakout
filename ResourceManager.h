#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#define STB_IMAGE_IMPLEMENTATION

#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
public:
	// Storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	// Load & retrive Shader
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static Shader &GetShader(std::string name);

	// Load & Retreive Texture
	static Texture LoadTexture(const char* file, bool alpha, std::string name);
	static Texture &GetTexture(std::string name);

	// Deallocate all loaded resources
	static void Clear();

private:
	ResourceManager();

	// Load and generate shadre from file
	static Shader loadShaderFromFile(const char *vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	// Load single texture from file
	static Texture loadTextureFromFile(const char* file, bool alpha);
};

#endif