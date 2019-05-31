#include "Light.h"

Light::Light() {
	this->position = vec3(0.0,0.0,0.0);
	this->diffuse = vec3(1.f,1.f,1.f);
	this->specular = 1.0f;
	this->atten = vec3(1.0f, 0.09f, 0.0032f);


	return;
}

LightManager::LightManager() {


}