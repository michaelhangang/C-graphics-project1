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
#include<fstream>
#include"VAOManager.h"
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
	
	Shader fragmentShader(Shader::shaderType::FRAGMENT, "fragment.glsl");
	fragmentShader.createShader();

	//create shader program
	int shaderProgram = glCreateProgram();
	//link 
	glAttachShader(shaderProgram, vertexShader.Id);
	glAttachShader(shaderProgram, fragmentShader.Id);
	glLinkProgram(shaderProgram);
	
	//delete shader object
	glDeleteShader(vertexShader.Id);
	glDeleteShader(fragmentShader.Id);

	
	
	//float vertices[12222];
	//
	//int indices[62222];
	////load ply file 
	//ifstream iff("vers.txt");
	//if (iff.is_open()) {
	//	string temp;
	//	while (iff >> temp) {
	//		if (temp == "vertex")
	//			break;
	//	}
	//	//read the number of vertices
	//	int verNum;
	//	iff >> verNum;
	//	
	//	while (iff >> temp) {
	//		if (temp == "face")
	//			break;
	//	}
	//	//read the number of faces
	//	unsigned int faceNum;
	//	iff >> faceNum;

	//	while (iff >> temp) {
	//		if (temp == "end_header")
	//			break;
	//	}
	//		
	//	//load  vertex attributes
	//	for (int i = 0; i < verNum*6;i++) {
	//	     
	//		iff >>vertices[i];
	//	}
	//	unsigned int tem = 0;
	//	
	//	//load index
	//	for (unsigned int i = 0,j=0; i < faceNum ;i++,j+=3) {
	//		
	//		iff >> tem;
	//		iff>> indices[j+0];
	//		iff >> indices[j+1];
	//		iff>> indices[j+2];
	//	}
	//}
	//else {
	//	cout << "open modle file failed";
	//}
	//
	//
	//	
	////create vertex buffer object 
	//unsigned int VAO,VBO,EBO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	////bind vertex Array Object first 
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	////setup vertex attributes pointer
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	////enable vertext attribte , it is disable by default
	//glEnableVertexAttribArray(0);
	//
	//// ÑÕÉ«ÊôÐÔ
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//
	////unbind VBO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	////unbind VAO
	//glBindVertexArray(0);

	//draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	VAOManager VAOManager;
	Model triangle("vers.txt");
	VAOManager.loadModelToVAO(triangle.fileName, triangle);
	
	Model angle("bun.ply");
	VAOManager.loadModelToVAO(angle.fileName, angle);

	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);
		//
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		
		//clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		
		//activate shader program
		glUseProgram(shaderProgram);
	
		glBindVertexArray(triangle.VAOId);
		glDrawElements(GL_TRIANGLES, triangle.numberOfIndices, GL_UNSIGNED_INT, 0);
		

		glBindVertexArray(angle.VAOId);
		glDrawElements(GL_TRIANGLES, angle.numberOfIndices, GL_UNSIGNED_INT, 0);
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

//input 
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}
