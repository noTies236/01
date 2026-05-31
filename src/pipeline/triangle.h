#pragma once
#include "glad/glad.h"
#include "../type/type.h"

// DEFAULT LIB
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class Triangle {
private:
	GLuint p_shader;

public:
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint IBO = 0;
	GLuint data = 0;
	std::string s_TempShader = "";
	GLuint v_shader_c = 0;
	GLuint f_shader_c = 0;

	void glCreateVao();
	void glGenBuffer();
	void glDefineVertex(std::vector<Vertex>& data);
	void glBindVao();
	void glCreateIBO(std::vector<GLuint>& data);

	void glHandleAttrib();
	void glUnBindVao(); 
	void loadShaderIntoRam(const std::string& file, std::string& x);
	void shaderCompiler(GLenum shadeType, GLuint& c_shader) const;
	void shaderLinker(); 
	void useProgram() const;

	// here
	void renderProgram();
};