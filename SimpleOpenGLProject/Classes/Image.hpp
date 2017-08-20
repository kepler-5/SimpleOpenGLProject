#ifndef Image_hpp
#define Image_hpp

#include <string>

/** Usage example:
 
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the image texture
	Image img{"filename.png"};
	glTexImage2D(GL_TEXTURE_2D, 0, img.format(), img.width(), img.height(), 0, img.format(), GL_UNSIGNED_BYTE, img.data());
	// img can now be safely destroyed
 
	glGenerateMipmap(GL_TEXTURE_2D); // etc
 
 
 ** Note: if your image is in a grayscale format, it may turn out unexpectedly RED if you try to use it in an RGB context. Use such textures' red channels as scalar values within shaders, make it look gray with vec4(tex.r), or convert the image file to an RGB format.
 
 */

struct Image {
	Image(const std::string& filename);
	~Image();
	
	const unsigned char* data() const { return _data; }
	int width() const { return _width; }
	int height() const { return _height; }
	GLenum format() const;
	
private:
	unsigned char* _data;
	int _width, _height, _channels;
};


#endif /* Image_hpp */

