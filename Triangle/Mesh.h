#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include<string>
#include"Model.h"
using namespace std;

class Mesh {
public:	
	string modelType;
	
	glm::vec3 pos;			// 0,0,0 (origin)
	glm::vec4 colour;		// 0,0,0,1 (black)
	glm::vec3 orientation;	// aka rotation 
	float scale;		// 1.0f  
	bool isWireframe;		// false
	Mesh();
	Mesh(Model model);
};