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
bool isShiftDownAlone(int mods);
glm::vec3 cameraPos = glm::vec3(0.0f,0.0f, 4.5f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
vector<Mesh> MeshToDraw;
int indexSelectedMesh = 0;
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
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");

	
	VAOManager VAOManager;
	//load model 1
	Model dragon("dragon","xyzrgb_dragon_simple.ply");
	VAOManager.loadModelToVAO(dragon);
	
	//load model 2
	Model bunny("bunny","bun.ply");
	VAOManager.loadModelToVAO(bunny);
	
	//load model 3
	Model sphere("sphere", "Sphere.ply");
	VAOManager.loadModelToVAO(sphere);
	//load model 4


	
	//create dragon
	Mesh dragon1(dragon);
	dragon1.pos = glm::vec3(-2.6f,0.f,0.f);
	dragon1.scale = 0.009f;
	dragon1.orientation = glm::vec3(0.f, glm::radians(-40.f),0.f);
	dragon1.colour = vec3(1.0f, 0.5f, 0.31f);
	MeshToDraw.push_back(dragon1);
	//
	Mesh rabbit1(bunny);
	rabbit1.pos = glm::vec3(1.7f, 0.f, 0.f);
	rabbit1.scale = 2.f;
	rabbit1.orientation = glm::vec3(0.f, glm::radians(0.f), 0.f);
	rabbit1.colour = vec3(0.7f, 0.6f, 0.2f);
	MeshToDraw.push_back(rabbit1);
	//
	Mesh light(sphere);
	light.pos = glm::vec3(1.2f, 1.f, 0.8f);
	light.scale = 0.2f;
	//rabbit1.orientation = glm::vec3(0.f, glm::radians(0.f), 0.f);
	light.colour = vec3(1.f, 1.f, 1.f);
	MeshToDraw.push_back(light);

	//light

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

			// pass them to the shaders 
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			//pass color to shaders
			glUniform3f(colorLoc, mesh.colour.x, mesh.colour.y, mesh.colour.z);
			
			// Is the object wireframe
			if (mesh.isWireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

					
			//light
			unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "theLights[0].position");
			vec3 lightPos = vec3(0.f, 0.f, 0.f);
			glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

			unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
			glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
			unsigned int lightAttenLoc = glGetUniformLocation(shaderProgram, "theLights[0].atten");
			vec3 lightAtten = vec3(1.0f, 0.12f, 0.07f);
			glUniform3f(lightAttenLoc, lightAtten.x, lightAtten.y, lightAtten.z);
			unsigned int lightAmbLoc = glGetUniformLocation(shaderProgram, "theLights[0].ambient");
			glUniform1f(lightAmbLoc,0.1f);
			unsigned int lightSpeLoc = glGetUniformLocation(shaderProgram, "theLights[0].specular");
			glUniform1f(lightSpeLoc,1.0f);
			
			unsigned int lightDiffLoc = glGetUniformLocation(shaderProgram, "theLights[0].diffuse");
			glUniform3f(lightDiffLoc, 1.0f, 1.f, 1.f);

			//get VAO Id 
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
	const float cameraSpeed = 0.05f;
	if (isShiftDownAlone(mods)) {
		if (key == GLFW_KEY_A) {
			MeshToDraw[indexSelectedMesh].pos.x += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_D) {
			MeshToDraw[indexSelectedMesh].pos.x -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_W) {
			MeshToDraw[indexSelectedMesh].pos.z += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_S) {
			MeshToDraw[indexSelectedMesh].pos.z -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}

		if (key == GLFW_KEY_Q) {
			MeshToDraw[indexSelectedMesh].pos.y += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_E) {
			MeshToDraw[indexSelectedMesh].pos.y -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}

		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			MeshToDraw[indexSelectedMesh].scale += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}
		if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			MeshToDraw[indexSelectedMesh].scale -= 0.001f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}
		return;

	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		indexSelectedMesh ++;
		cout << indexSelectedMesh << endl;
	}
		
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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

	//
	
}

bool isShiftDownAlone(int mods)
{
	if ((mods & GLFW_MOD_SHIFT)== GLFW_MOD_SHIFT)
	{
		return true;
	}
	return false;
}

bool isAltDownAlone(int mods)
{
	if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
	{
		return true;
	}
	return false;
}
