#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"
#include <vector>

GLTexture ImageLoader::loadPNG(std::string filePath) {
	//Generate GLTexture and set its fields to 0.
	GLTexture texture = {};
	
	//This is the input data to decodePNG, which we load from a file.
	std::vector<unsigned char> in;

	//This is the output data from decodePNG, which is the pixel data for out texture.
	std::vector<unsigned char> out;

	unsigned long height, width;

	//Read in the image file contents into a buffer.
	if (IOManager::readFileToBuffer(filePath, in) == false) {
		fatalError("Failed to read PNG to buffer from " + filePath);
	}

	//Decode the .png format into an array of pixels.
	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
	if (errorCode != 0) {
		fatalError("decodePNG failed with error code " + std::to_string(errorCode));
	}

	//Generate openGL texture object.
	glGenTextures(1, &texture.ID);

	//Bind the texture object.
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	//Upload the pixels to the texture.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	//Set some texture parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//generates mipmaps.
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	//return a copy of the texture data.
	return texture;
}