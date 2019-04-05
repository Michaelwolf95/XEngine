#MATERIAL_PROPERTIES
vec4 color 1.0f
float shininess 32
float specularity 0.75f
#!MATERIAL_PROPERTIES

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
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_height1;
	vec4 color;
	float shininess;
	float specularity;
};

struct PointLight {
	vec4 color;
	float intensity;
	vec3 position;
	float ambience; // strength
	//vec3 diffuse;
	//vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	// shares attributes with PointLight
	PointLight source;

	// unique attributes
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

struct GlobalLight { // AKA DirLight
	vec4 color;
	float intensity;
	vec3 direction; // function of transform
	float ambience; // strength
	//vec3 diffuse;
	//vec3 specular;
};

const int NUM_LIGHTS = 10;

uniform sampler2D Texture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform vec4 sceneAmbience; // get from camera
uniform float sceneAmbienceStrength; // currrently hardcoded in material::draw


uniform GlobalLight globalLights[NUM_LIGHTS];
uniform PointLight pointLights[NUM_LIGHTS];
uniform SpotLight spotLights[NUM_LIGHTS];

uniform int numLights; // inside simple model component to limit for loop iterations
uniform int numGlobalLights; // each num variable is determined within SimpleModelComponent's Draw
uniform int numPointLights;
uniform int numSpotLights;
uniform Material material;

//vec3 calculateAmbientLighting(); // NOTE: cannot pass by reference / use inline, 
vec3 getDiffuseTexel();
vec3 getSpecularTexel();
vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir);
vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir);
vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 viewDir);

vec3 calculateAmbience(const float ambience);
vec3 calculateDiffuse(const vec3 lightDir, const vec3 lightColor, const vec3 norm);
vec3 calculateSpecular(const vec3 lightDir, const vec3 norm, const vec3 viewDir, const vec4 lightColor);
float calculateAttenuation(const PointLight light);
float calculateDistance(const vec3 position);

void main()
{
	// general ambience
	vec4 ambience = sceneAmbience == vec4(0.0f) ? sceneAmbience : vec4(0.5f);
	ambience *= sceneAmbienceStrength > 0 ? sceneAmbienceStrength : 0.5f;

	vec3 norm = normalize(Normal);

	vec3 viewDir = normalize(viewPos - FragPos); // TODO: assign viewPos. Currently nothing inputted

	vec3 result = vec3(0.0f);


	for (int i = 0; i < numGlobalLights; i++) {
		result += calculateGlobalLighting(globalLights[i], norm, viewDir);

	}

	for (int i = 0; i < numPointLights; i++) {
		result += calculatePointLight(pointLights[i], norm, viewDir);
	}

	for (int i = 0; i < numSpotLights; i++) { // TODO: Test spotlight
		result += calculateSpotLight(spotLights[i], norm, viewDir);
	}

	FragColor = vec4(result, 1.0f) * material.color 
		+ ambience * vec4(getDiffuseTexel(), 1.0f);
}

vec3 getDiffuseTexel() 
{
	return (float(textureSize(texture_diffuse1, 0).x) > 1)
		? texture(texture_diffuse1, TexCoord).rgb : material.color.rgb;
}

vec3 getSpecularTexel() 
{
	return (float(textureSize(texture_specular1, 0).x) > 1) 
		? texture(texture_specular1, TexCoord).rgb : material.color.rgb;
}

vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir) 
{
	vec3 lightDir = normalize(light.position - FragPos);

	// ambient
	vec3 ambience = calculateAmbience(light.ambience);

	// diffuse
	vec3 diffuse = calculateDiffuse(lightDir, light.color.rgb, norm);

	// specular
	vec3 specular = calculateSpecular(lightDir, norm, viewDir, light.color);

	// attenuation
	float attenuation = calculateAttenuation(light);

	return (ambience + diffuse + specular) * light.intensity * attenuation;
}

vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir) 
{
	vec3 lightDir = normalize(-light.direction);
	
	// ambient
	vec3 ambience = calculateAmbience(light.ambience);
	
	// diffuse
	vec3 diffuse = calculateDiffuse(lightDir, light.color.rgb, norm);

	// specular
	vec3 specular = calculateSpecular(lightDir, norm, viewDir, light.color);

	return (ambience + diffuse + specular) * light.intensity;
}

// TODO: specularity for spotlight?
vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 viewDir) 
{
	vec3 lightDir = normalize(light.source.position - FragPos);
	
	// ambient
	vec3 ambience = calculateAmbience(light.source.ambience);

	// attenuation
	float attenuation = calculateAttenuation(light.source);

	// diffuse
	vec3 diffuse = calculateDiffuse(lightDir, light.source.color.rgb, norm);
	
	// specular
	vec3 specular = calculateSpecular(lightDir, norm, viewDir, light.source.color);

	// spotlight (soft edges)
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	return (ambience + (diffuse + specular) * intensity) *  attenuation * light.source.intensity;
}

vec3 calculateAmbience(const float ambience)
{
	return ambience * getDiffuseTexel();
}

vec3 calculateDiffuse(const vec3 lightDir, const vec3 lightColor, const vec3 norm)
{
	float diff = max(dot(norm, lightDir), 0.0f);
	return diff * lightColor * getDiffuseTexel();
}

vec3 calculateSpecular(const vec3 lightDir, const vec3 norm, const vec3 viewDir, const vec4 lightColor)
{
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //32 = shininess property of Material
	return material.specularity * spec * lightColor.rgb * getSpecularTexel();
}

// light intensity determined by distance
float calculateAttenuation(const PointLight source)
{
	float distance = calculateDistance(source.position);
	return 1.0 / (source.constant + source.linear * distance
		+ source.quadratic * (distance * distance));
}

float calculateDistance(const vec3 position)
{
	return length(position - FragPos);
}