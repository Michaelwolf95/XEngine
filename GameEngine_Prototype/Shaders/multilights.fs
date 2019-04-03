#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;   
    float shininess;
}; 

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

const int NUM_LIGHTS = 10;

uniform vec3 viewPos; 
uniform vec3 lightColor;
//uniform vec3 objectColor

uniform vec4 color;
uniform sampler2D Texture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform Material material;
uniform GlobalLight globalLights[NUM_LIGHTS];
uniform PointLight pointLights[NUM_LIGHTS];

uniform int numLights; // inside simple model component to limit for loop iterations
uniform int numGlobalLights; // each num variable is determined within SimpleModelComponent's Draw
uniform int numPointLights;
uniform int numSpotLights;

vec3 calculateAmbientLighting(); // NOTE: cannot pass by reference / use inline, 
vec3 getDiffuseTexel();
vec3 getSpecularTexel();
vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir);
vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir);

void main()
{
    //vec4 texel_diff;// = texture(Texture, TexCoord);
    //if (float(textureSize(texture_diffuse1, 0).x) > 1) {
    //    texel_diff = texture(texture_diffuse1, TexCoord);
    //}
    //else if(float(textureSize(Texture,0).x) > 1) {
    //    texel_diff = texture(Texture, TexCoord);
    //}
    //else {
    //    texel_diff = vec4(1.0,1.0,1.0,1.0);
    //}
  	
    // diffuse 
    vec3 norm = normalize(Normal);

    //vec3 viewPos = vec3(
    vec3 viewDir = normalize(viewPos - FragPos); // TODO: assign viewPos. Currently nothing inputted

    vec3 result = vec3(0.0f);


    for (int i = 0; i < numGlobalLights; i++) {
        result += calculateGlobalLighting(globalLights[i], norm, viewDir);
    }

    for (int i = 0; i < numPointLights; i++) {
        result += calculatePointLight(pointLights[i], norm, viewDir);
    }

    for (int i = 0; i < numSpotLights; i++) { // TODO: spotLight
        //result += calculateSpotLight();
    }

    FragColor = vec4(result, 1.0f) * color;
}

vec3 calculateAmbientLighting() {
    float ambientStrength = 0.11f; // remove hardcode
    return ambientStrength * getDiffuseTexel();
}

vec3 getDiffuseTexel() {
	return (float(textureSize(texture_diffuse1, 0).x) > 1)
		? texture(texture_diffuse1, TexCoord).rgb : vec3(1.0f);
}

vec3 getSpecularTexel(){
    return (float(textureSize(texture_specular1, 0).x) > 1) ? texture(texture_specular1, TexCoord).rgb : vec3(1.0f);
}

vec3 calculatePointLight(const PointLight light, const vec3 norm, const vec3 viewDir) {
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance 
                        + light.quadratic * (distance * distance));
    
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * light.color * getDiffuseTexel();
    
    // specular
    float specularStrength = 0.3f; // hardcoded specular strength for now
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //32 = shininess property of Material
    vec3 specular = specularStrength * spec * light.color * getSpecularTexel();
    return (calculateAmbientLighting() + diffuse + specular) * attenuation * light.intensity;
}

vec3 calculateGlobalLighting(const GlobalLight light, const vec3 norm, const vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);    
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess); //32 = shininess property of Material
    float specularStrength = 0.3f;

    vec3 diffuse = diff * light.color * getDiffuseTexel();
    vec3 specular = specularStrength * spec * light.color * getSpecularTexel();
    return (calculateAmbientLighting() + diffuse + specular) * light.intensity;
}