#include <iostream>
#include <glad/glad.h> 
#include <SDL3/SDL.h>

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window);
//


// resoze window? idk 
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}


//void processInput(GLFWwindow *window) 
//{ 
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//}

// Vertex Shader: processa cada vértice e define sua posição na tela (gl_Position)
const char* vertexShaderSource = 
	"#version 330 core\n"  // Define OpenGL 3.3
	"layout (location = 0) in vec3 aPos;\n" // Entrada: posição do vértice (x, y, z)
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"  // Define posição do vértice na tela
	"}\0";

unsigned int vertexShader;

const char* fragmentShadeSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

unsigned int fragmentShader;

unsigned int shaderProgram;

float vertices[] = {
	-0.0f, -0.5f, 0.0f, // X   
	 0.2f,  0.0f, 0.1f, // Y  
	 0.0f,  0.5f, 0.0f  // Z 
};

unsigned int VBO;
unsigned int VAO;

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("01", 800, 600, SDL_WINDOW_OPENGL);
	
	if (window == NULL) {
		std::cout << "creation of windows have falied!!!" << "\n";
		return -1;
	}

	SDL_GL_CreateContext(window);

	if (!gladLoadGL()) {
		std::cout << "Failed to initialize GLAD" << "\n";
		return -1;
	}

	//glViewport(0, 0, 800, 600);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetSamplerParameteriv(vertexShader, 512, NULL);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShadeSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),  vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgram);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (true) {
		/*processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/
	}

	//glfwTerminate();
	return 0;
}