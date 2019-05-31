#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <vector>

using namespace glm;
using namespace std;
class Light {
public:
	vec3 position;
	vec3 diffuse;
	float specular;
	vec3 atten;
	
	Light();
};


class LightManager
{
public:
	static const unsigned int NUMBEROFLIGHTS = 5;

	vector<Light> Lights;
	LightManager();

	void LoadUniformLocations(int shaderID);
	
	void CopyLightValuesToShader(void);

};