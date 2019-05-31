#include "Mesh.h"

Mesh::Mesh(){}

Mesh::Mesh(Model model):modelType(model.name)
{
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);				// 0,0,0 (origin)
	this->colour = glm::vec3(0.0f, 0.0f, 0.0f);		// 0,0,0,1 (black)
	this->orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = 1.0f;
	this->isWireframe = false;
}