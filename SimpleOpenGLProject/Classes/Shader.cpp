#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {
	std::string sourceFromPath(std::string path) {
		std::ifstream shaderFile;
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		
		shaderStream << shaderFile.rdbuf();
		
		shaderFile.close();
		
		return shaderStream.str();
	}
	
	void checkCompileErrors(unsigned int shader, const std::string& type) {
		int success;
		char infoLog[1024];
		if ( type != "PROGRAM" ) {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if ( !success ) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
			}
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if ( !success ) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
			}
		}
	}
}

Shader::Shader(std::string vertex_,
			   std::string fragment_,
			   InputTypeTag vertType,
			   InputTypeTag fragType) {
	std::string vShaderCode = vertType == InputTypeTag::Path
								? sourceFromPath(std::move(vertex_))
								: std::move(vertex_);
	const char* vShaderChar = vShaderCode.c_str();
	std::string fShaderCode = fragType == InputTypeTag::Path
								? sourceFromPath(std::move(fragment_))
								: std::move(fragment_);
	const char* fShaderChar = fShaderCode.c_str();
	
	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderChar, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderChar, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}
