#include "Light.h"
#include <sstream>
Light::Light() {
	position = vec3(0.0,0.0,0.0);
	diffuse = vec3(1.f,1.f,1.f);
	specular = 0.5f;
	atten = vec3(1.0f, 0.09f, 0.0032f);
	isOn = 1.0f;

	lightPosLoc =-1;
	lightSpeLoc =-1;
    lightAttenLoc =-1;
	lightDiffLoc =-1;
	lightIsONLoc = -1;;

	return;
}
void Light::turnOff() {
	isOn = 0.0f;
}

void Light::turnOn() {
	isOn = 1;
}

LightManager::LightManager() {
	

}

void LightManager::GetUniformLocations(int shaderID) {
	for (size_t i = 0; i < Lights.size(); i++) {
		ostringstream iss;
		iss << "theLights["<< i << "].position";
		Lights[i].lightPosLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss <<"theLights["<< i << "].specular";
		Lights[i].lightSpeLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theLights[" << i << "].atten";
		Lights[i].lightAttenLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theLights[" << i << "].diffuse";
		Lights[i].lightDiffLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		
		iss.str("");
		iss << "theLights[" << i << "].isOn";

		Lights[i].lightIsONLoc = glGetUniformLocation(shaderID, iss.str().c_str());

	}
}

void LightManager::CopyLightValuesToShader(void){
	for (size_t i = 0; i < Lights.size(); i++) {

		glUniform3f(Lights[i].lightPosLoc, Lights[i].position.x, Lights[i].position.y, Lights[i].position.z);

		glUniform3f(Lights[i].lightAttenLoc,Lights[i].atten.x, Lights[i].atten.y,Lights[i].atten.z);

		glUniform1f(Lights[i].lightSpeLoc, Lights[i].specular);

		glUniform1f(Lights[i].lightIsONLoc, Lights[i].isOn);

		glUniform3f(Lights[i].lightDiffLoc, Lights[i].diffuse.x, Lights[i].diffuse.y, Lights[i].diffuse.z);
	}
}