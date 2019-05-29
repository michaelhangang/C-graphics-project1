#include "Mesh.h"


Mesh::Mesh()
{

	this->modelType = "";
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);				// 0,0,0 (origin)
	this->colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);		// 0,0,0,1 (black)
	this->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = 1.0f;
	this->isWireframe = false;
}