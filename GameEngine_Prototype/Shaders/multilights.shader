#VERTEX_SHADER
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoord = aTexCoord;
	gl_Position = projection * view * model * vec4(aPos, 1.0); // added model back into equation, no discernable difference--removed
	//gl_Position = projection * view * model * vec4(FragPos, 1.0); // added model back into equation, no discernable difference--removed
}

#FRAGMENT_SHADER
#version 330 core
out vec4 FragColor;

//struct Material {
//    sampler2D diffuse;
//    vec3 specular;    
//    float shininess;
//}; 

struct GlobalLight { // AKA DirLight
	vec3 color;
	float intensity;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 color;
	float intensity;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform float constant;
uniform float linear;
uniform float quadratic;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform vec3 viewPos;
uniform vec3 lightColor;
//uniform vec3 objectColor

uniform vec4 color;
uniform sampler2D Texture;
uniform sampler2D texture_diffuse1;

const int NUM_LIGHTS = 10;
uniform GlobalLight globalLights[NUM_LIGHTS];
uniform PointLight pointLights[NUM_LIGHTS];

uniform int numLights; // inside simple model component to limit for loop iterations
uniform int numGlobalLights; // each num variable is determined within SimpleModelComponent's Draw
uniform int numPointLights;
uniform int numSpotLights;

vec3 calculateAmbientLighting(const vec4 texel); // NOTE: cannot pass by reference / use inline, 
vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir, const vec4 texel);
vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir, const vec4 texel);

void main()
{
	vec4 texel;// = texture(Texture, TexCoord);
	if (float(textureSize(texture_diffuse1, 0).x) > 1) {
		texel = texture(texture_diffuse1, TexCoord);
	}
	else if (float(textureSize(Texture, 0).x) > 1) {
		texel = texture(Texture, TexCoord);
	}
	else {
		texel = vec4(1.0, 1.0, 1.0, 1.0);
	}

	// diffuse 
	vec3 norm = normalize(Normal);

	//vec3 viewPos = vec3(
	vec3 viewDir = normalize(viewPos - FragPos); // TODO: assign viewPos. Currently nothing inputted

	vec3 result = vec3(0.0f);


	for (int i = 0; i < numGlobalLights; i++) {
		result += calculateGlobalLighting(globalLights[i], norm, viewDir, texel);
	}

	for (int i = 0; i < numPointLights; i++) {
		result += calculatePointLight(pointLights[i], norm, viewDir, texel);
	}

	for (int i = 0; i < numSpotLights; i++) { // TODO: spotLight
											  //result += calculateSpotLight();
	}

	FragColor = vec4(result, 1.0f) * color;
}

vec3 calculateAmbientLighting(const vec4 texel) {
	float ambientStrength = 0.11f;
	return ambientStrength * texel.rgb;
}

vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir, const vec4 texel) {
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance
		+ light.quadratic * (distance * distance));

	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * light.color * texel.rgb;

	// specular
	float specularStrength = 0.3f; // hardcoded specular strength for now
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16); //32 = shininess property of Material
	vec3 specular = specularStrength * spec * light.color;
	return (calculateAmbientLighting(texel) + diffuse + specular) * attenuation * light.intensity;
}

vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir, const vec4 texel) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16); //32 = shininess property of Material
	float specularStrength = 0.3f;

	vec3 diffuse = diff * light.color * texel.rgb;
	vec3 specular = specularStrength * spec * light.color;
	return (calculateAmbientLighting(texel) + diffuse + specular) * light.intensity;
}