#include <iostream>
#include <glad/glad.h> 
#include <SDL3/SDL.h>
#include "./debug/debug.h"
#include "./pipeline/triangle.h"
#include "./pipeline/OBJLoader.h"

#define glDebug(x) glClearAllErros(); x; glCatchErrors(__FILE__, __LINE__, #x); 

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

int main()
{
	std::cout << "\x1B[2J\x1B[H";
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

	bool isRunning = true;
	//SDL_Event event;

	// HEXÁGONO — vértice central + 6 na borda, leque de 6 triângulos
	std::vector<Vertex> data = {
		{  0.00f,  0.000f, 0.0f,   1.0f, 1.0f, 1.0f,   0.50f, 0.500f },  // 0: centro
		{  0.50f,  0.000f, 0.0f,   1.0f, 0.0f, 0.0f,   1.00f, 0.500f },  // 1:   0°
		{  0.25f,  0.433f, 0.0f,   1.0f, 1.0f, 0.0f,   0.75f, 0.933f },  // 2:  60°
		{ -0.25f,  0.433f, 0.0f,   0.0f, 1.0f, 0.0f,   0.25f, 0.933f },  // 3: 120°
		{ -0.50f,  0.000f, 0.0f,   0.0f, 1.0f, 1.0f,   0.00f, 0.500f },  // 4: 180°
		{ -0.25f, -0.433f, 0.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.067f },  // 5: 240°
		{  0.25f, -0.433f, 0.0f,   1.0f, 0.0f, 1.0f,   0.75f, 0.067f },  // 6: 300°
	};

	std::vector<GLuint> ibo = {
		0, 1, 2,   0, 2, 3,   0, 3, 4,
		0, 4, 5,   0, 5, 6,   0, 6, 1,
	};

	Triangle helloWorld;
	helloWorld.glCreateVao();
	helloWorld.glBindVao();
	helloWorld.glGenBuffer();
	helloWorld.glDefineVertex(data);
	helloWorld.glHandleAttrib();
	helloWorld.glCreateIBO(ibo);
	helloWorld.loadShaderIntoRam("./src/shader/vertex.glsl", helloWorld.s_TempShader);
	helloWorld.shaderCompiler(GL_VERTEX_SHADER, helloWorld.v_shader_c);
	helloWorld.loadShaderIntoRam("./src/shader/fragment.glsl", helloWorld.s_TempShader);
	helloWorld.shaderCompiler(GL_FRAGMENT_SHADER, helloWorld.f_shader_c);
	helloWorld.shaderLinker();
	helloWorld.useProgram();

	// QUADRADO — 4 vértices, 2 triângulos
	std::vector<Vertex> data2 = {
		{ -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f },  // 0: sup-esq
		{ -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f },  // 1: inf-esq
		{  0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f },  // 2: inf-dir
		{  0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f },  // 3: sup-dir
	};

	std::vector<GLuint> ibo2 = {
		0, 1, 2,   // triângulo inferior-esquerdo
		0, 2, 3,   // triângulo superior-direito
	};

	Triangle hello;
	hello.glCreateVao();
	hello.glBindVao();
	hello.glGenBuffer();
	hello.glDefineVertex(data2);
	hello.glHandleAttrib();
	helloWorld.glCreateIBO(ibo2);
	hello.loadShaderIntoRam("./src/shader/vertex.glsl", hello.s_TempShader);
	hello.shaderCompiler(GL_VERTEX_SHADER, hello.v_shader_c);
	hello.loadShaderIntoRam("./src/shader/fragment.glsl", hello.s_TempShader);
	hello.shaderCompiler(GL_FRAGMENT_SHADER, hello.f_shader_c);
	hello.shaderLinker();
	hello.useProgram();

	std::vector<Vertex> data3;
	std::vector<GLuint> ibo3;
	if (!loadOBJ("./src/model/SmallKey.obj", data3, ibo3)) {
		std::cerr << "Não consegui abrir o .obj — confira o caminho\n";
		return -1;
	}

	Triangle key;
	key.glCreateVao();
	key.glBindVao();
	key.glGenBuffer();
	key.glDefineVertex(data3);
	key.glHandleAttrib();
	key.glCreateIBO(ibo3);
	key.loadShaderIntoRam("./src/shader/vertex.glsl", key.s_TempShader);
	key.shaderCompiler(GL_VERTEX_SHADER, key.v_shader_c);
	key.loadShaderIntoRam("./src/shader/fragment.glsl", key.s_TempShader);
	key.shaderCompiler(GL_FRAGMENT_SHADER, key.f_shader_c);
	key.shaderLinker();
	key.useProgram();

	int sum = 0;

	while (isRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) isRunning = false;
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		key.renderProgram();
		SDL_GL_SwapWindow(window);
	}
	
	return 0;
}