#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp> 

#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include"Shader.h"
#include<fstream>
#include"VAOManager.h"
#include"Mesh.h"


//auto adjust window when window is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//input control
void processInput(GLFWwindow *window);

int main(void)
{	
	//initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(1440, 900, "OpenGL", NULL, NULL);
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

	//draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int pLoc = glGetUniformLocation(shaderProgram, "projection");
	
	//
	
	VAOManager VAOManager;
	Model dragon("dragon","xyzrgb_dragon_simple.ply");
	VAOManager.loadModelToVAO(dragon);
	

	Model bunny("bunny","bun.ply");
	VAOManager.loadModelToVAO(bunny);

	vector<Mesh> MeshToDraw;
	//create mesh
	Mesh dragon1(dragon);
	MeshToDraw.push_back(dragon1);
	//dragon1.
	Mesh rabbit1(bunny);
	MeshToDraw.push_back(rabbit1);

//draw graphics
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
	
		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 scale = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
		scale = glm::scale(scale,glm::vec3( 0.007f, 0.007f, 0.007f));
		

		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(scale));


		Model x= VAOManager.dateVAO.find(dragon1.modelType)->second;
		glBindVertexArray(x.VAOId);
		glDrawElements(GL_TRIANGLES, x.numberOfIndices, GL_UNSIGNED_INT, 0);
		

		// create transformations
		glm::mat4 model1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection1 = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, (float)glfwGetTime() * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view1 = glm::translate(view1, glm::vec3(0.3f, 0.0f, 0.0f));
		projection1 = glm::scale(projection1, glm::vec3(2.f, 2.f, 2.0f));
		

		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view1));
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection1));


		Model y = VAOManager.dateVAO.find(rabbit1.modelType)->second;
		glBindVertexArray(y.VAOId);
		glDrawElements(GL_TRIANGLES, y.numberOfIndices, GL_UNSIGNED_INT, 0);
		
		
		
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
