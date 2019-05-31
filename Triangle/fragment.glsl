#version 420 

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 ourColor;


struct Light {
    vec3 position;  
   
    vec3 diffuse;
    float specular;
	vec3 atten;
};

const int NUMBEROFLIGHTS = 5;
uniform Light theLights[NUMBEROFLIGHTS];

void main()
{
    //float specularStrength = 1.0;
    //vec3 lightColor= vec3(1.0) ;
	float ambientStrength = 0.1;
	
	//set up ambient 
    vec3 ambient = ambientStrength * theLights[0].diffuse;
	 
	//diffuse 
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(theLights[0].position- FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * theLights[0].diffuse;

	//specular 
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = theLights[0].specular * spec * theLights[0].diffuse;
	
	//attenua
	float distance    = length(theLights[0].position - FragPos);
    float attenuation = 1.0 / (theLights[0].atten.x + theLights[0].atten.y * distance +theLights[0].atten.z * (distance * distance));
	ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;


	vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 1.0);
	
} 