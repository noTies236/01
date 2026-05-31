#pragma once
#include <glad/glad.h>
#include "../debug/debug.h"
#include "triangle.h"

#define glDebug(x) glClearAllErros(); x; glCatchErrors(__FILE__, __LINE__, #x); 

void Triangle::glCreateVao()
{
	glDebug(glCreateVertexArrays(1, &this->VAO);)
}

void Triangle::glBindVao()
{
	glDebug(glBindVertexArray(this->VAO);)
}

void Triangle::glCreateIBO(std::vector<GLuint>& data)
{
	glGenBuffers(1, &this->IBO);
	glDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);)

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), GL_STATIC_DRAW);
}

void Triangle::glGenBuffer()
{
	glGenBuffers(1, &this->VBO);
	glDebug(glBindBuffer(GL_ARRAY_BUFFER, this->VBO);)
}

void Triangle::glDefineVertex(std::vector<Vertex>& data)
{
	glDebug(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), data.data(), GL_STATIC_DRAW))
}

void Triangle::glHandleAttrib()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glDebug(glEnableVertexAttribArray(0);)
}

void Triangle::glUnBindVao()
{
	glBindVertexArray(0);
}

void Triangle::loadShaderIntoRam(const std::string& filePath, std::string& x)
{
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "Erro: nao foi possivel abrir o shader: " << filePath << "\n";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	x = buffer.str();
}

void Triangle::shaderCompiler(GLenum shadeType, GLuint& c_shader) const
{
	GLuint shaderID = glCreateShader(shadeType);
	const char* src = this->s_TempShader.c_str();
	glShaderSource(shaderID, 1, &src, nullptr);
	glDebug(glCompileShader(shaderID))

	// Sem isso, erros de GLSL viram tela preta sem explicação
	int ok;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		char log[512];
		glGetShaderInfoLog(shaderID, 512, nullptr, log);
		std::cerr << "Erro de compilacao do shader: " << log << "\n";
	}

	c_shader = shaderID;
}

void Triangle::shaderLinker()
{
	this->p_shader = glCreateProgram();
	glAttachShader(this->p_shader, this->v_shader_c);
	glAttachShader(this->p_shader, this->f_shader_c);
	glLinkProgram(p_shader);

	int ok;
	glGetProgramiv(this->p_shader, GL_LINK_STATUS, &ok);
	if (!ok) {
		char log[512];
		glGetProgramInfoLog(this->p_shader, 512, nullptr, log);
		std::cerr << "Erro de link do shader: " << log << "\n";
	}

	glDeleteShader(this->v_shader_c);
	glDeleteShader(this->f_shader_c);
}

void Triangle::useProgram() const
{
	glUseProgram(this->p_shader);
}

void Triangle::renderProgram()
{
	this->glBindVao();
	glDebug(glDrawElements(GL_TRIANGLES, 10000, GL_UNSIGNED_INT, 0);)
}