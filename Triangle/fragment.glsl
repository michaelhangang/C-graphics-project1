#version 420 

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 ourColor;
uniform vec3 sphereColor;
uniform bool isLightSphere;

struct Light {
    vec3 position;  
   
    vec3 diffuse;
    float specular;
	vec3 atten;
	float isOn;
};

const int NUMBEROFLIGHTS = 5;
uniform Light theLights[NUMBEROFLIGHTS];

void main()
{
    vec3 finalColor= vec3(0.0) ;
	float ambientStrength = 0.1;
	
	if(isLightSphere){
	    FragColor = vec4(sphereColor, 1.0);
		return;
	}
	
	for(int index =0;index<NUMBEROFLIGHTS;index++){
	   
	//if light is off 
	   if(theLights[index].isOn ==0.0f)
		{
			continue;
		}
	//set up ambient 
        vec3 ambient = ambientStrength * theLights[index].diffuse;
	 
	//diffuse 
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(theLights[index].position- FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * theLights[index].diffuse;

	//specular 
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = theLights[index].specular * spec * theLights[index].diffuse;
	
	//attenua
	float distance    = length(theLights[index].position - FragPos);
    float attenuation = 1.0 / (theLights[index].atten.x + theLights[index].atten.y * distance +theLights[index].atten.z * (distance * distance));
	ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;


	finalColor += (ambient + diffuse + specular) * ourColor;

	
	}
    FragColor = vec4(finalColor, 1.0);
	
} 