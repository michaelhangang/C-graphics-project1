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
using namespace glm;

//auto adjust window when window is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//input control
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
glm::vec3 cameraPos = glm::vec3(0.0f,0.0f, 4.5f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
vector<Mesh> MeshToDraw;

int main(void)
{	
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

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize  glad 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSwapInterval(1);
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
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	
	//
	
	VAOManager VAOManager;
	//load model 1
	Model dragon("dragon","xyzrgb_dragon_simple.ply");
	VAOManager.loadModelToVAO(dragon);
	
	//load model 2
	Model bunny("bunny","bun.ply");
	VAOManager.loadModelToVAO(bunny);
	
	//load model 3

	//load model 4


	
	//create mesh
	Mesh dragon1(dragon);
	dragon1.pos = glm::vec3(1.4f,0.f,0.f);
	dragon1.scale = 0.007f;
	dragon1.orientation = glm::vec3(0.f, glm::radians(-40.f),0.f);
	MeshToDraw.push_back(dragon1);
	//
	Mesh rabbit1(bunny);
	rabbit1.pos = glm::vec3(1.7f, 0.f, 0.f);
	rabbit1.scale = 2.f;
	rabbit1.orientation = glm::vec3(0.f, glm::radians(0.f), 0.f);
	MeshToDraw.push_back(rabbit1);
	
//draw graphics
	while (!glfwWindowShouldClose(window))
	{
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
	
		//perspective 
		glm::mat4 projection =
			glm::perspective(glm::radians(45.0f), ratio, 0.1f, 1000.0f);

		//camera
		glm::mat4 view = glm::mat4(1.0f);	
		view = glm::lookAt(cameraPos,
			cameraTarget,
			cameraUp);

		//set up view and projection value in shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (auto mesh : MeshToDraw) {
			// create transformations
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			
			glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
				mesh.orientation.z,
				glm::vec3(0.0f, 0.0, 1.0f));
		
			glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
				mesh.orientation.y,
				glm::vec3(0.0f, 1.0, 0.0f));
			
			glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
				mesh.orientation.x,
				glm::vec3(1.0f, 0.0, 0.0f));
			
			glm::mat4 move = glm::translate(glm::mat4(1.0f), mesh.pos);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(mesh.scale, mesh.scale, mesh.scale));
			model = rotateZ* rotateY *rotateX* move * scale;

			// pass them to the shaders (3 different ways)
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			Model mod = VAOManager.dateVAO.find(mesh.modelType)->second;
			glBindVertexArray(mod.VAOId);
			glDrawElements(GL_TRIANGLES, mod.numberOfIndices, GL_UNSIGNED_INT, 0);

		}
		
	
		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//input 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	const float cameraSpeed = 0.05f;

	if (key == GLFW_KEY_W )
		cameraPos.z -= cameraSpeed;
	if (key == GLFW_KEY_S )
		cameraPos.z += cameraSpeed;
	if (key == GLFW_KEY_A)
		cameraPos.x -= cameraSpeed;
	if (key == GLFW_KEY_D )
		cameraPos.x += cameraSpeed;
	if (key == GLFW_KEY_Q )
		cameraPos.y += cameraSpeed;
	if (key == GLFW_KEY_E )
		cameraPos.y -= cameraSpeed;

	if (key == GLFW_KEY_R ) {
		MeshToDraw[0].pos.x += 0.1f;
		cout << MeshToDraw[0].pos.x << endl;
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		MeshToDraw[0].pos.x -= 0.1f;
		cout << MeshToDraw[0].pos.x << endl;
	}
}