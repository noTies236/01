#include "debug.h"
#include <glad/glad.h>

void glClearAllErros() 
{
	while (glGetError() != GL_NO_ERROR);
}

void glCatchErrors(const char* fileName, int line, const char* functionName)
{
	GLenum error = glGetError();

	if (error != GL_NO_ERROR) {
		std::cout 
			<< "Error: " << error <<				"\n"
			<< "function name: " << functionName <<	"\n" 
			<< "file name: " << fileName <<			"\n" 
			<< "line: " << line <<					"\n";
	}
}