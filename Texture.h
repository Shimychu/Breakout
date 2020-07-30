#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture
{
public:
	// holds the ID of the texture object, used for all testure operation to reference to this particular texture
	unsigned int ID;

	// Dimensions in pixels of loaded texture
	unsigned int Width, Height;

	// Format
	unsigned int Internal_Format; // format of texture object
	unsigned int Image_Format; // format of loaded image

	// Configurations
	unsigned int Wrap_S; // wraping mode S axis
	unsigned int Wrap_T; // wraping mode T axis
	unsigned int Filter_Min; // filter if texture pixel < screen pixel
	unsigned int Filter_Max; // filter if texture > screen pixel

	// Constructor
	Texture();

	// Generate texture from image
	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	// Binds texture to to texture object
	void Bind() const;

private:


};

#endif

