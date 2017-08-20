#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

Image::Image(const std::string& filename) {
	_data = stbi_load(filename.c_str(), &_width, &_height, &_channels, 0);
	if ( !_data )
		std::cout << "IMAGE LOAD ERROR: <" << filename << ">. Image data will be NULL.";
}

Image::~Image() {
	stbi_image_free(_data);
}

GLenum Image::format() const {
	switch ( _channels ) {
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
		case 1:
			return GL_RED;
		default:
			return GL_RGBA;
	}
}
