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
#include"Light.h"
using namespace glm;
#pragma region method
//auto adjust window when window is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//input control
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool isShiftDownAlone(int mods);
bool isCtrlDownAlone(int mods);
void SaveEverything(void);
bool isAltDownAlone(int mods);
void drawMesh(Mesh mesh,int shaderId);
void drawLightSphere(Mesh lightSphere, int shaderId);
void drawSpotLightSphere(Mesh lightSphere, int shaderId);
#pragma endregion 

#pragma region global variable
glm::vec3 cameraPos = glm::vec3(3, 3.1, 100.5);
glm::vec3 cameraPos2 = glm::vec3(-15.8, 54.2003, 96.299);
glm::vec3 cameraPos3 = glm::vec3(-9.8, -24.9998, 26.2994);
glm::vec3 cameraTarget = glm::vec3(0, 1, 0);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

vector<Mesh> MeshToDraw;
VAOManager VaoManager;
LightManager lights;
int indexSelectedMesh = 0;
int indexSelectedLight = 0;
int indexSelectedSpotLight = 0;
int view = 0;
//screen 
int screenWidth = 1680;
int creenHigh = 900;

#pragma endregion

int main(void)
{	
#pragma region initialize and configure
	//initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(screenWidth, creenHigh, "OpenGL", NULL, NULL);
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
#pragma endregion 
#pragma region  Get location of view, projection  camera 

	// retrieve the matrix uniform locations
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
#pragma endregion

#pragma region light mesh

	Model sphere("sphere", "Sphere.ply");
	VaoManager.loadModelToVAO(sphere);
	Mesh lightSphere("light", sphere);
	lightSphere.pos = glm::vec3(-7.99999f, 0.f, 0);
	lightSphere.scale = 0.2f;
	lightSphere.isWireframe = true;

#pragma endregion light mesh

	
#pragma region  load Model
	Model dragon("dragon","Falcon_Body_054217_verts.ply");
	VaoManager.loadModelToVAO(dragon);
	//Model flydragon("flydragon2", "Dragon 2.5_ply.ply");
	//VaoManager.loadModelToVAO(flydragon);
	////load model 2
	//Model warrior("warrior","BM_3d_Low.ply");
	//VaoManager.loadModelToVAO(warrior);
	//
	////load model 3
	
	////load model 4
	//Model Thenatarius("Thenatarius", "Thenatarius.ply");
	//VaoManager.loadModelToVAO(Thenatarius);

	////load model 4
	//Model owlmonk("owlmonk", "owl-monk-fixed-mesh.ply");
	//VaoManager.loadModelToVAO(owlmonk);
#pragma endregion 
	
#pragma region mesh

	//create dragon
	Mesh dragon1("dragon1",dragon);
	dragon1.pos = glm::vec3(0.2, 0, 0);
	dragon1.scale = 0.133f;
	dragon1.orientation = glm::vec3(3.09, -2.36, 1.68);
	dragon1.colour = vec3(1.0f, 0.5f, 0.31f);
	MeshToDraw.push_back(dragon1);
	
#pragma endregion 

#pragma region point light

	//light 
	Light lightone;
	lightone.position = vec3(4.50004, 1.89995, 13.9);
	lightone.atten = vec3(1, 0.038, 2.74103e-21);
	lightone.diffuse = vec3(1, -0.0999993, 0.68);
	lights.Lights.push_back(lightone);

	//light2
	Light light2;
	light2.position = vec3(-8.29996, -6.50009, 7.90001);
	light2.atten = vec3(1, 0.119, 2.10543e-29);
	light2.diffuse = vec3(1, 1, 1);
	lights.Lights.push_back(light2);
	//
	Light light3;
	light3.position = vec3(0.199998, 11.4, 5.39998);
	light3.atten = vec3(1, 0.039, 0.0340892);
	light3.diffuse = vec3(-0.629999, 1.97, 1);
	lights.Lights.push_back(light3);
	//
	Light light4;
	light4.position = vec3(-28.3, -17.5, 18.5);
	light4.atten = vec3(1, 0.0320009, 4.22104e-12);
	light4.diffuse = vec3(3.33, 0.5, 0.31);
	light4.turnOff();
	lights.Lights.push_back(light4);
	//
	Light light5;
	light5.position = vec3(-15.1999, 2, 25.1001);
	light5.atten = vec3(1, 0.015, 0.00501816);
	light5.turnOff();
	light5.diffuse = vec3(1, 1.82, 1);
	lights.Lights.push_back(light5);
	lights.GetPointLightsUniformLocations(shaderProgram);
#pragma endregion 

#pragma region spot light

	//Spot
	SpotLight spotLight1;
	spotLight1.position = vec3(-15.1999, 2, 25.1001);
	spotLight1.turnOff();
	spotLight1.atten = vec3(1, 0.015, 0.00501816);
	spotLight1.direction = vec3(-28.8, -2.4, -3.1);
	lights.SpotLights.push_back(spotLight1);
	lights.GetSpotLightsUniformLocations(shaderProgram);
#pragma endregion

//draw graphics
	while (!glfwWindowShouldClose(window))
	{
#pragma	region basic setting

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
		glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
#pragma endregion 	

		//draw every mesh
		for (vector<Mesh>::iterator iter = MeshToDraw.begin(); iter != MeshToDraw.end();iter++) {
			Mesh mesh = *iter;
			drawMesh(mesh,shaderProgram);
		}

		//draw light sphere
		drawLightSphere(lightSphere,shaderProgram);
		drawSpotLightSphere(lightSphere, shaderProgram);
		
		//light up 
		lights.CopyPointLightsValuesToShader();
		lights.CopySpotLightsValuesToShader();

#pragma region swap butters

		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
#pragma endregion 
#pragma region stop 

	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
#pragma endregion 

}



#pragma region input setting 

//input 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const float cameraSpeed = 0.2f;
  #pragma region change mesh
	//change mesh
	if (isShiftDownAlone(mods)) {
		//change postion
		
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

		//change scale
		if (key == GLFW_KEY_Z ) {
			MeshToDraw[indexSelectedMesh].scale += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}
		if (key == GLFW_KEY_X ) {
			MeshToDraw[indexSelectedMesh].scale -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}

		//change orientation
		if (key == GLFW_KEY_T ) {
			MeshToDraw[indexSelectedMesh].orientation.x +=0.01f ;
			cout << MeshToDraw[indexSelectedMesh].orientation.x <<" "<< endl;
		}
		if (key == GLFW_KEY_G) {
			MeshToDraw[indexSelectedMesh].orientation.x -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.x << " " << endl;
		}
		if (key == GLFW_KEY_Y ) {
			MeshToDraw[indexSelectedMesh].orientation.y += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.y << " "  << endl;
		}
		if (key == GLFW_KEY_H) {
			MeshToDraw[indexSelectedMesh].orientation.y -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.y << " " << endl;
		}
		if (key == GLFW_KEY_U ) {
			MeshToDraw[indexSelectedMesh].orientation.z += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.z << " "  << endl;
		}
		if (key == GLFW_KEY_J) {
			MeshToDraw[indexSelectedMesh].orientation.z -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.z << " " << endl;
		}
		//change color
		if (key == GLFW_KEY_1) {
			MeshToDraw[indexSelectedMesh].colour.x +=0.01f ;
			cout << MeshToDraw[indexSelectedMesh].colour.x << endl;
		}
		if (key == GLFW_KEY_2) {
			MeshToDraw[indexSelectedMesh].colour.x -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.x << endl;
		}
		if (key == GLFW_KEY_3) {
			MeshToDraw[indexSelectedMesh].colour.y += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}
		if (key == GLFW_KEY_4) {
			MeshToDraw[indexSelectedMesh].colour.y -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}

		if (key == GLFW_KEY_5) {
			MeshToDraw[indexSelectedMesh].colour.y += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}
		if (key == GLFW_KEY_6) {
			MeshToDraw[indexSelectedMesh].colour.y -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			indexSelectedMesh++;
			if (indexSelectedMesh == MeshToDraw.size())
				indexSelectedMesh = 0;
			cout << indexSelectedMesh << endl;
		}

		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			indexSelectedMesh--;
			if (indexSelectedMesh <0)
				indexSelectedMesh = 0;
			cout << indexSelectedMesh << endl;
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
			MeshToDraw[indexSelectedMesh].isWireframe = true;
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
			MeshToDraw[indexSelectedMesh].isWireframe = false;
		return;
	}
#pragma endregion 

  #pragma region point lights

	//adjust lights
	if (isAltDownAlone(mods)) {
		//change postion
		if (key == GLFW_KEY_A) {
			lights.Lights[indexSelectedLight].position.x -= 0.1f;
			cout << lights.Lights[indexSelectedLight].position.x << endl;
		}
		if (key == GLFW_KEY_D) {
			lights.Lights[indexSelectedLight].position.x += 0.1f;
			cout << lights.Lights[indexSelectedLight].position.x << endl;
		}
		if (key == GLFW_KEY_W) {
			lights.Lights[indexSelectedLight].position.z += 0.1f;
			cout << lights.Lights[indexSelectedLight].position.z << endl;
		}
		if (key == GLFW_KEY_S) {
			lights.Lights[indexSelectedLight].position.z -= 0.1f;
			cout << lights.Lights[indexSelectedLight].position.z << endl;
		}

		if (key == GLFW_KEY_Q) {
			lights.Lights[indexSelectedLight].position.y += 0.1f;
			cout << lights.Lights[indexSelectedLight].position.y << endl;
		}
		if (key == GLFW_KEY_E) {
			lights.Lights[indexSelectedLight].position.y -= 0.1f;
			cout << lights.Lights[indexSelectedLight].position.y << endl;
		}

		//change atten
		if (key == GLFW_KEY_1 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.y -= 0.001f;
			cout << lights.Lights[indexSelectedLight].atten.y << endl;
		}
		if (key == GLFW_KEY_2 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.y += 0.001f;
			cout << lights.Lights[indexSelectedLight].atten.y << endl;
		}

		if (key == GLFW_KEY_3 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.z *= 0.9f;
			cout << lights.Lights[indexSelectedLight].atten.z << endl;
		}
		if (key == GLFW_KEY_4 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.z *= 1.1f;
			cout << lights.Lights[indexSelectedLight].atten.z << endl;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			indexSelectedLight++;
			if (indexSelectedLight == lights.Lights.size())
				indexSelectedLight = 0;
			cout << indexSelectedLight << endl;
		}

		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			indexSelectedLight--;
			if (indexSelectedLight <0)
				indexSelectedLight = 0;
			cout << indexSelectedLight << endl;
		}
		//light is on or off
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
			lights.Lights[indexSelectedLight].turnOff();
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
			lights.Lights[indexSelectedLight].turnOn();

		//change color
		if (key == GLFW_KEY_5) {
			lights.Lights[indexSelectedLight].diffuse.x += 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_6) {
			lights.Lights[indexSelectedLight].diffuse.x -= 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_7) {
			lights.Lights[indexSelectedLight].diffuse.y += 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_8) {
			lights.Lights[indexSelectedLight].diffuse.y -= 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_9) {
			lights.Lights[indexSelectedLight].diffuse.z += 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.z << endl;
		}
		if (key == GLFW_KEY_0) {
			lights.Lights[indexSelectedLight].diffuse.z -= 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.z << endl;
		}
		return;
	}
#pragma endregion point lights

  #pragma region spot lights

	if( isCtrlDownAlone( mods)) {
		//change postion
		if (key == GLFW_KEY_A) {
			lights.SpotLights[indexSelectedSpotLight].position.x -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.x << endl;
		}
		if (key == GLFW_KEY_D) {
			lights.SpotLights[indexSelectedSpotLight].position.x += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.x << endl;
		}
		if (key == GLFW_KEY_W) {
			lights.SpotLights[indexSelectedSpotLight].position.z += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.z << endl;
		}
		if (key == GLFW_KEY_S) {
			lights.SpotLights[indexSelectedSpotLight].position.z -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.z << endl;
		}

		if (key == GLFW_KEY_Q) {
			lights.SpotLights[indexSelectedSpotLight].position.y += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.y << endl;
		}
		if (key == GLFW_KEY_E) {
			lights.SpotLights[indexSelectedSpotLight].position.y -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.y << endl;
		}

		//change atten
		if (key == GLFW_KEY_1 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.y -= 0.001f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.y << endl;
		}
		if (key == GLFW_KEY_2 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.y += 0.001f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.y << endl;
		}

		if (key == GLFW_KEY_3 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.z *= 0.9f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.z << endl;
		}
		if (key == GLFW_KEY_4 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.z *= 1.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.z << endl;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			indexSelectedSpotLight++;
			if (indexSelectedSpotLight == lights.SpotLights.size())
				indexSelectedSpotLight = 0;
			cout << indexSelectedSpotLight << endl;
		}

		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			indexSelectedSpotLight--;
			if (indexSelectedSpotLight < 0)
				indexSelectedSpotLight = 0;
			cout << indexSelectedSpotLight << endl;
		}
		//light is on or off
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
			lights.SpotLights[indexSelectedSpotLight].turnOff();
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
			lights.SpotLights[indexSelectedSpotLight].turnOn();

		//change color
		if (key == GLFW_KEY_5) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.x += 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_6) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.x -= 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_7) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.y += 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_8) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.y -= 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_9) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.z += 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.z << endl;
		}
		if (key == GLFW_KEY_0) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.z -= 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.z << endl;
		}
		if (key == GLFW_KEY_Z) {
			lights.SpotLights[indexSelectedSpotLight].direction.x += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.x << endl;
		}
		if (key == GLFW_KEY_X) {
			lights.SpotLights[indexSelectedSpotLight].direction.x -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.x << endl;
		}
		if (key == GLFW_KEY_C) {
			lights.SpotLights[indexSelectedSpotLight].direction.y += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.y << endl;
		}
		if (key == GLFW_KEY_V) {
			lights.SpotLights[indexSelectedSpotLight].direction.y -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.y << endl;
		}
		if (key == GLFW_KEY_B) {
			lights.SpotLights[indexSelectedSpotLight].direction.z += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.z<< endl;
		}
		if (key == GLFW_KEY_N) {
			lights.SpotLights[indexSelectedSpotLight].direction.z -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.z << endl;
		}
		return;
	}
#pragma endregion  spot lights

  #pragma region basic control

	//save 
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		SaveEverything();

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
	//change camera
	if (key == GLFW_KEY_C&&action== GLFW_PRESS) {
		view++;
		switch (view)
		{
		case 1:cameraPos = cameraPos2; break;
		case 2:cameraPos = cameraPos3; break;
		default:
			cameraPos = vec3(-13, -18.7998, 73.4994);
			view = 0;
			break;
		}
	}
#pragma endregion 

}
#pragma endregion 

#pragma region Save setting

void SaveEverything(void)
{
	std::ofstream saveFile("Save.txt");

	for (unsigned int index = 0;
		index != MeshToDraw.size(); index++)
	{
		saveFile
			<< MeshToDraw[index].name << "\n postion "
			<< MeshToDraw[index].pos.x << ", "
			<< MeshToDraw[index].pos.y << ", "
			<< MeshToDraw[index].pos.z << "\n\n orientation "
			<< MeshToDraw[index].orientation.x << ", "
			<< MeshToDraw[index].orientation.y << ","
			<< MeshToDraw[index].orientation.z << " \n\n scale "
			<< MeshToDraw[index].scale << "\n\n color "
			<< MeshToDraw[index].colour.x << ", "
			<< MeshToDraw[index].colour.y << ", "
			<< MeshToDraw[index].colour.z << "\n\n";

	}

	for (unsigned int index = 0;
		index < lights.Lights.size(); index++)
	{
		saveFile
			<< "Light # " << index + 1 << "\n postion "
			<< lights.Lights[index].position.x << ", "
			<< lights.Lights[index].position.y << ", "
			<< lights.Lights[index].position.z << "\n\n atten "
			<< lights.Lights[index].atten.x << ", "
			<< lights.Lights[index].atten.y << ","
			<< lights.Lights[index].atten.z << " \n\n color "
			<< lights.Lights[index].diffuse.x << ","
			<< lights.Lights[index].diffuse.y << ","
			<< lights.Lights[index].diffuse.z << " \n\n";

			

	}
	for (unsigned int index = 0;
		index < lights.SpotLights.size(); index++)
	{
		saveFile
			<< "SpotLight # " << index + 1 << "\n postion "
			<< lights.SpotLights[index].position.x << "  "
			<< lights.SpotLights[index].position.y << "  "
			<< lights.SpotLights[index].position.z << "\n\n atten "
			<< lights.SpotLights[index].atten.x << " "
			<< lights.SpotLights[index].atten.y << " "
			<< lights.SpotLights[index].atten.z << " \n\n color "
			<< lights.SpotLights[index].diffuse.x << ","
			<< lights.SpotLights[index].diffuse.y << ","
			<< lights.SpotLights[index].diffuse.z << " \n\n direction "
			<< lights.SpotLights[index].direction.x << " "
			<< lights.SpotLights[index].direction.y << " "
			<< lights.SpotLights[index].direction.z << " \n\n";

	}
	saveFile <<"Camera "<< cameraPos.x << "," << cameraPos.y << "," << cameraPos.z;

	saveFile.close();
	cout << "Saved...." << endl;
	return;
}
#pragma endregion 


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
bool isCtrlDownAlone(int mods) {
	if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
	{
		return true;
	}
	return false;

}

void drawMesh(Mesh mesh,int shaderId) {
	//for (Mesh mesh : MeshToDraw) {
		// create transformations
		glm::mat4 model = glm::mat4(1.0f);

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
		model = rotateZ * rotateY * rotateX * move * scale;

		// pass them to the shaders 
		unsigned int modelLoc = glGetUniformLocation(shaderId, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//pass color to shaders
		unsigned int colorLoc = glGetUniformLocation(shaderId, "ourColor");
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

		//get VAO Id 
		Model mod = VaoManager.dateVAO.find(mesh.modelType)->second;
		glBindVertexArray(mod.VAOId);
		glDrawElements(GL_TRIANGLES, mod.numberOfIndices, GL_UNSIGNED_INT, 0);
}
void drawLightSphere(Mesh mesh,int shaderId) {
	unsigned int sphereColourLoc = glGetUniformLocation(shaderId, "sphereColor");
	unsigned int  isLightLoc = glGetUniformLocation(shaderId, "isLightSphere");
	glUniform1f(isLightLoc, (float)GL_TRUE);
	glUniform3f(sphereColourLoc, 1.0f, 1.0f, 1.0f);
	mesh.pos = lights.Lights[indexSelectedLight].position;

	//draw lightphere 
	drawMesh(mesh, shaderId);
	glUniform1f(isLightLoc, (float)GL_FALSE);

}
void drawSpotLightSphere(Mesh lightSphere, int shaderId) {
	unsigned int sphereColourLoc = glGetUniformLocation(shaderId, "sphereColor");
	unsigned int  isLightLoc = glGetUniformLocation(shaderId, "isLightSphere");
	glUniform1f(isLightLoc, (float)GL_TRUE);
	glUniform3f(sphereColourLoc, 1.0f, 1.0f, 1.0f);
	lightSphere.pos = lights.SpotLights[indexSelectedSpotLight].position;

	//draw lightphere 
	drawMesh(lightSphere, shaderId);
	glUniform1f(isLightLoc, (float)GL_FALSE);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}