#pragma once
#include<string>
#include<vector>
using namespace std;


class Shader 
{
public:
	enum shaderType {
		VERTEX,
		FRAGMENT,
        };
	shaderType type;
	unsigned int Id;
	string glslFile;
	vector<string> vecSource;
	
	string m_lastError;

	string getShaderTypeString() { 
		if (this->type == VERTEX) {
			return "VERTEX_SHADER";

		}

		else if (this->type == FRAGMENT) {
			return "FRAGMENT_SHADER";
		}

		else
			return "";
	}
	
	Shader(shaderType type,string filename);

	void  createShader();
	bool wasThereACompileError(unsigned int shaderID, string &errorText);
	bool loadSourceFromFile();
	bool compileShaderFromSource(Shader &shader, string &error);

};