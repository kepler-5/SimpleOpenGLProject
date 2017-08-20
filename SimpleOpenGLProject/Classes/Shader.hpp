#ifndef Shader_hpp
#define Shader_hpp

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
	enum class InputTypeTag { Path, SourceString };
	Shader(std::string vertex,
		   std::string fragment,
		   InputTypeTag vertType = InputTypeTag::Path,
		   InputTypeTag fragType = InputTypeTag::Path);
	
	~Shader();

	void use() const {
		glUseProgram(ID);
	}

	void setUniformBool(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setUniformInt(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setUniformFloat(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setUniformMat4(const std::string& name, const glm::mat4& matrix) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	GLint getAttributeLocation(const std::string& name) const {
		return glGetAttribLocation(ID, name.c_str());
	}
	
	unsigned int getID() const { return ID; }
	
private:
	unsigned int ID;
};

#endif /* Shader_hpp */

