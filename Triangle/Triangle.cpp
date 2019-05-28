#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include"Shader.h"


//static void error_callback(int error, const char* description)
//{
//	fprintf(stderr, "Error: %s\n", description);
//}
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//}


//auto adjust window when window is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//
void processInput(GLFWwindow *window);
void checkShaderCompile(Shader shader);

int main(void)
{
	//GLFWwindow* window;
	//GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	//GLint mvp_location, vpos_location, vcol_location;
	//glfwSetErrorCallback(error_callback);
	//if (!glfwInit())
	//	exit(EXIT_FAILURE);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	//if (!window)
	//{
	//	glfwTerminate();
	//	exit(EXIT_FAILURE);
	//}
	//glfwSetKeyCallback(window, key_callback);
	//glfwMakeContextCurrent(window);
	////gladLoadGL(glfwGetProcAddress);
	//gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//glfwSwapInterval(1);
	//// NOTE: OpenGL error checks have been omitted for brevity
	//glGenBuffers(1, &vertex_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	//glCompileShader(vertex_shader);
	//fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	//glCompileShader(fragment_shader);
	//program = glCreateProgram();
	//glAttachShader(program, vertex_shader);
	//glAttachShader(program, fragment_shader);
	//glLinkProgram(program);
	//mvp_location = glGetUniformLocation(program, "MVP");
	//vpos_location = glGetAttribLocation(program, "vPos");
	//vcol_location = glGetAttribLocation(program, "vCol");
	//glEnableVertexAttribArray(vpos_location);
	//glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
	//	sizeof(vertices[0]), (void*)0);
	//glEnableVertexAttribArray(vcol_location);
	//glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
	//	sizeof(vertices[0]), (void*)(sizeof(float) * 2));
	//while (!glfwWindowShouldClose(window))
	//{
	//	float ratio;
	//	int width, height;
	//	//mat4x4 m, p, mvp;
	//	glm::mat4 m, p, v, mvp;
	//	glfwGetFramebufferSize(window, &width, &height);
	//	ratio = width / (float)height;
	//	glViewport(0, 0, width, height);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	//mat4x4_identity(m);
	//	m = glm::mat4(1.0f);
	//	//mat4x4_rotate_Z(m, m, (float)glfwGetTime());
	//	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
	//		(float)glfwGetTime(),
	//		glm::vec3(0.0f, 0.0, 1.0f));

	//	m = m * rotateZ;
	//	//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	//	p = glm::perspective(0.6f,
	//		ratio,
	//		0.1f,
	//		1000.0f);

	//	v = glm::mat4(1.0f);

	//	glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
	//	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	//	v = glm::lookAt(cameraEye,
	//		cameraTarget,
	//		upVector);
	//	//mat4x4_mul(mvp, p, m);
	//	mvp = p * v * m;
	//	glUseProgram(program);
	//	//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
	//	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
	//	glDrawArrays(GL_TRIANGLES, 0, 3);
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}
	//glfwDestroyWindow(window);
	//glfwTerminate();
	//exit(EXIT_SUCCESS);
	
	//initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize  glad 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//view
	//glViewport(0, 0, 800, 600);

	//create shaders
	Shader vertexShader(Shader::shaderType::VERTEX,"vertex.glsl");
	vertexShader.createShader();
	checkShaderCompile(vertexShader);
	
	Shader fragmentShader(Shader::shaderType::FRAGMENT, "fragment.glsl");
	fragmentShader.createShader();
	checkShaderCompile(fragmentShader);

	//create shader program
	int shaderProgram = glCreateProgram();
	//link 
	glAttachShader(shaderProgram, vertexShader.Id);
	glAttachShader(shaderProgram, fragmentShader.Id);
	glLinkProgram(shaderProgram);

	// check for linking errors
	/*glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
*/
	
	//delete shader object
	glDeleteShader(vertexShader.Id);
	glDeleteShader(fragmentShader.Id);

	//set up vertex data 
	float vertices[] =
	{
		 -0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	//create vertex buffer object 
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//bind vertex Array Object first 
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//setup vertex attributes pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//enable vertext attribte , it is disable by default
	glEnableVertexAttribArray(0);
	//unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//unbind VAO
	glBindVertexArray(0);

	//draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);
		
		//clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	//stop 
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void checkShaderCompile(Shader shader) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader.Id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader.Id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

//input 
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}
