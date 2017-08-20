#ifndef Types_h
#define Types_h

#include <glm/glm.hpp>

using Position	= glm::vec3;
using Color		= glm::vec4;
using TexCoord	= glm::vec2;

struct Vertex {
	Position position;
	Color color;
	TexCoord texCoord;
};

#endif /* Types_h */
