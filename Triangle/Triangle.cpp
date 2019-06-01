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

//auto adjust window when window is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//input control
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool isShiftDownAlone(int mods);
void SaveEverything(void);
bool isAltDownAlone(int mods);
void drawMesh(int shaderId);

//global variable
glm::vec3 cameraPos = glm::vec3(4.1f, -4.3f, 11.75);
glm::vec3 cameraPos2 = glm::vec3(0.0f, 5.0f, 14.5f);
glm::vec3 cameraPos3 = glm::vec3(-5.0f, 0.0f, 14.5f);

glm::vec3 cameraTarget = glm::vec3(-0.5, 1.49012, -4);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

vector<Mesh> MeshToDraw;
VAOManager VaoManager;
LightManager lights;
int indexSelectedMesh = 0;
int indexSelectedLight = 0;

float rotationDegree = 0.f;

int screenWidth = 1680;
int creenHigh = 900;
int main(void)
{	
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

	//draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// retrieve the matrix uniform locations
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "viewPos");



	
	//load model 1
	Model dragon("dragon","xyzrgb_dragon_simple.ply");
	VaoManager.loadModelToVAO(dragon);
	//load model 2
	Model bunny("bunny","bun.ply");
	VaoManager.loadModelToVAO(bunny);
	
	//load model 3
	Model sphere("sphere", "Sphere.ply");
	VaoManager.loadModelToVAO(sphere);
	//load model 4
	Model ship("ship", "MilleniumFalcon_xyz_n_rgba_uv(simplified).ply");
	VaoManager.loadModelToVAO(ship);


	
	//create dragon
	Mesh dragon1("dragon1",dragon);
	dragon1.pos = glm::vec3(0.9, -0.2, -2);
	dragon1.scale = 0.009f;
	dragon1.orientation = glm::vec3(0.f, glm::radians(124.f),0.f);
	dragon1.colour = vec3(1.0f, 0.5f, 0.31f);
	MeshToDraw.push_back(dragon1);
	//
	Mesh rabbit1("rabbit1",bunny);
	rabbit1.pos = glm::vec3(-0.5, 1.49012e-08, -4);
	rabbit1.scale = 9.7f;
	rabbit1.orientation = glm::vec3(0.f, radians(0.f), 0.f);
	rabbit1.colour = vec3(0.7f, 0.6f, 0.2f);
	MeshToDraw.push_back(rabbit1);
	//
	Mesh lightSphere("light",sphere);
	lightSphere.pos = glm::vec3(-7.99999f, 0.f, 0);
	lightSphere.scale = 0.2f;
	lightSphere.isWireframe = true;
	lightSphere.colour = vec3(1.f, 0.f, 0.f);
	MeshToDraw.push_back(lightSphere);

	//
	Mesh spaceship("spaceship", ship);
	spaceship.pos = glm::vec3(7.99999f, -1.6f, 3.7);
	spaceship.scale = 2.2f;
	spaceship.orientation = glm::vec3(2.14675, glm::radians(0.f), 0.f);
	spaceship.colour = vec3(1.f, 1.f, 1.f);
	MeshToDraw.push_back(spaceship);

	//light
	//light 
	Light lightone;
	lightone.position = vec3(0.f, 0.f, 0.f);
	lightone.atten = vec3(1.0f, 0.09f, 0.0032f);
	lights.Lights.push_back(lightone);
	lights.GetUniformLocations(shaderProgram);
	
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
			cameraPos+cameraTarget,
			cameraUp);

		//set up view and projection value in shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
		
        //let shpere postion is equal to light pos
		MeshToDraw[2].pos = lights.Lights[0].position;

		drawMesh(shaderProgram);

		lights.CopyLightValuesToShader();
		
		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//input 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const float cameraSpeed = 0.05f;
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
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			MeshToDraw[indexSelectedMesh].scale += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}
		if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			MeshToDraw[indexSelectedMesh].scale -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}

		//change orientation
		if (key == GLFW_KEY_T ) {
			MeshToDraw[indexSelectedMesh].orientation.x = radians(rotationDegree++);
			cout << MeshToDraw[indexSelectedMesh].orientation.x <<" "<< rotationDegree << endl;
		}
		if (key == GLFW_KEY_Y ) {
			MeshToDraw[indexSelectedMesh].orientation.y = radians(rotationDegree++);
			cout << MeshToDraw[indexSelectedMesh].orientation.y << " " << rotationDegree << endl;
		}
		if (key == GLFW_KEY_U ) {
			MeshToDraw[indexSelectedMesh].orientation.z = radians(rotationDegree++);
			cout << MeshToDraw[indexSelectedMesh].orientation.z << " " << rotationDegree << endl;
		}
		//change color
		if (key == GLFW_KEY_1) {
			MeshToDraw[indexSelectedMesh].colour.x +=0.01f ;
			cout << MeshToDraw[indexSelectedMesh].colour.x << endl;
		}
		if (key == GLFW_KEY_2) {
			MeshToDraw[indexSelectedMesh].colour.y += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}
		if (key == GLFW_KEY_3) {
			MeshToDraw[indexSelectedMesh].colour.z += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.z << endl;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			indexSelectedMesh++;
			if (indexSelectedMesh == MeshToDraw.size())
				indexSelectedMesh = 0;
			cout << indexSelectedMesh << endl;
		}

		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			indexSelectedMesh--;
			if (indexSelectedMesh == MeshToDraw.size())
				indexSelectedMesh = 0;
			cout << indexSelectedMesh << endl;
		}
		return;
	}
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
		if (key == GLFW_KEY_1) {
			lights.Lights[indexSelectedLight].atten.y *= 1.1f;
			cout << lights.Lights[indexSelectedLight].atten.y << endl;
		}
		if (key == GLFW_KEY_2) {
			lights.Lights[indexSelectedLight].atten.y *= 0.9f;
			cout << lights.Lights[indexSelectedLight].atten.y << endl;
		}

		if (key == GLFW_KEY_3) {
			lights.Lights[indexSelectedLight].atten.z *= 1.1f;
			cout << lights.Lights[indexSelectedLight].atten.z << endl;
		}
		if (key == GLFW_KEY_4) {
			lights.Lights[indexSelectedLight].atten.z *= 0.9f;
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
			if (indexSelectedLight == lights.Lights.size())
				indexSelectedLight = 0;
			cout << indexSelectedLight << endl;
		}
		return;
	}
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
	if (key == GLFW_KEY_C)
		cameraPos = cameraPos2;
	if (key == GLFW_KEY_V)
		cameraPos = cameraPos3;

	
}

void SaveEverything(void)
{
	std::ofstream saveFile("Save.txt");

	for (unsigned int index = 0;
		index != MeshToDraw.size(); index++)
	{
		saveFile
			<< MeshToDraw[index].name << "\n postion "
			<< MeshToDraw[index].pos.x << "f, "
			<< MeshToDraw[index].pos.y << "f, "
			<< MeshToDraw[index].pos.z << "f\n\n orientation "
			<< MeshToDraw[index].orientation.x << "f, "
			<< MeshToDraw[index].orientation.y << "f,"
			<< MeshToDraw[index].orientation.z << "f \n\n scale "
			<< MeshToDraw[index].scale << "\n\n color "
			<< MeshToDraw[index].colour.x << "f, "
			<< MeshToDraw[index].colour.y << "f, "
			<< MeshToDraw[index].colour.z << "f\n\n ";

	}

	for (unsigned int index = 0;
		index < lights.Lights.size(); index++)
	{
		saveFile
			<< "Light #"<< lights.Lights.size() << "\n postion "
			<< lights.Lights[index].position.x << "f, "
			<< lights.Lights[index].position.y << "f, "
			<< lights.Lights[index].position.z << "f\n\n atten "
			<< lights.Lights[index].atten.x << "f, "
			<< lights.Lights[index].atten.y << "f,"
			<< lights.Lights[index].atten.z << "f \n\n ";
			/*<< MeshToDraw[index].scale << "\n\n color "
			<< MeshToDraw[index].colour.x << "f, "
			<< MeshToDraw[index].colour.y << "f, "
			<< MeshToDraw[index].colour.z << "\n\n ";*/

	}
	saveFile <<"Camera "<< cameraPos.x << "f," << cameraPos.y << "f," << cameraPos.z;

	saveFile.close();
	cout << "Saved...." << endl;
	return;
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

void drawMesh(int shaderId) {
	for (Mesh mesh : MeshToDraw) {
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




}
