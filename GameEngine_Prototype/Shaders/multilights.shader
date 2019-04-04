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
	float ambience;
};

struct GlobalLight { // AKA DirLight
	vec4 color;
	float intensity;
	vec3 direction; // function of transform
	//vec3 ambience; // ambience should depend on the material not light source
	//vec3 diffuse;
	//vec3 specular;
};

struct PointLight {
	vec4 color;
	float intensity;
	vec3 position;
	//float ambience; // ambience should be restricted to global light source for now
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
};



//uniform float constant; // found in PointLight struct
//uniform float linear;
//uniform float quadratic;
const int NUM_LIGHTS = 10;

//#MATERIAL_PROPERTIES

uniform vec3 viewPos;

//uniform vec4 color;
uniform sampler2D Texture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform GlobalLight globalLights[NUM_LIGHTS];
uniform PointLight pointLights[NUM_LIGHTS];
uniform SpotLight spotLights[NUM_LIGHTS];

uniform int numLights; // inside simple model component to limit for loop iterations
uniform int numGlobalLights; // each num variable is determined within SimpleModelComponent's Draw
uniform int numPointLights;
uniform int numSpotLights;
uniform Material material;

//#!MATERIAL_PROPERTIES
//vec3 calculateAmbientLighting(); // NOTE: cannot pass by reference / use inline, 
vec3 getDiffuseTexel();
vec3 getSpecularTexel();
vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir);
vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir);
vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 viewDir);

void main()
{
	// diffuse 
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

	FragColor = vec4(result, 1.0f) * material.color;
}

//vec3 calculateAmbientLighting() {
//	float ambientStrength = 0.11f;
//	return ambientStrength * getDiffuseTexel();
//}

vec3 getDiffuseTexel() {
	return (float(textureSize(texture_diffuse1, 0).x) > 1)
		? texture(texture_diffuse1, TexCoord).rgb : material.color.rgb;
}

vec3 getSpecularTexel() {
	return (float(textureSize(texture_specular1, 0).x) > 1) 
		? texture(texture_specular1, TexCoord).rgb : material.color.rgb;
}

vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir) {
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance
		+ light.quadratic * (distance * distance));

	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * light.color.rgb * getDiffuseTexel();

	// specular
	float specularStrength = 0.3f; // hardcoded specular strength for now
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //32 = shininess property of Material
	vec3 specular = specularStrength * spec * light.color.rgb * getSpecularTexel();
	//return (calculateAmbientLighting() + diffuse + specular) * attenuation * light.intensity;
	return (material.ambience * getDiffuseTexel() + diffuse + specular) * attenuation * light.intensity;
}

vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess); //32 = shininess property of Material
	float specularStrength = 0.3f;

	vec3 diffuse = diff * light.color.rgb * getDiffuseTexel();
	vec3 specular = specularStrength * spec * light.color.rgb * getSpecularTexel();

	return (material.ambience * getDiffuseTexel() + diffuse + specular) * light.intensity;
}

// TODO: specularity for spotlight?
vec3 calculateSpotLight(const SpotLight light, const vec3 norm, const vec3 viewDir) {
	float distance = length(light.source.position - FragPos);
	vec3 lightDir = normalize(light.source.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	if (theta > light.cutOff)
	{
		// do lighting calculations
		return calculatePointLight(light.source, norm, viewDir);
	}
	// else, use ambient light so scene isn't completely dark outside the spotlight.
	float attenuation = 1.0 / (light.source.constant + light.source.linear * distance
		+ light.source.quadratic * (distance * distance));
	attenuation = attenuation > 1.0f ? 0.0f : attenuation;
	return material.ambience * getDiffuseTexel() * attenuation * light.source.intensity;
}