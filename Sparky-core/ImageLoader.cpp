#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"

GLTexture ImageLoader::loadPNG(std::string filePath) {
	GLTexture texture = {};

	std::vector<unsigned char> in;
	std::vector<unsigned char> out;

	unsigned long width, height;

	if (IOManager::readFileToBuffer(filePath, in) == false) {
		fatalError("Failed to load PNG file to buffer ");
	}

	//Decode the .png format into an array of pixels
	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
	if (errorCode != 0) {
		fatalError("decodePNG failed with error : " + std::to_string(errorCode));
	}

	//Generate opengl texture object
	glGenTextures(1, &(texture.id)); // 1 texture and it fills the id and return

	//bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture.id);
	//upload the pixels to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	//Set some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);     //unbind the current texture

	texture.width = width;
	texture.height = height;

	return texture;
}


