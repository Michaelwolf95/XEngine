#version 330 core
out vec4 FragColor;

//struct Material {
//    sampler2D diffuse;
//    vec3 specular;    
//    float shininess;
//}; 

//struct Light {
//    vec3 position;

//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

in vec3 FragPos;  
in vec2 TexCoord;
in vec3 Normal;  
  
uniform vec3 lightPos;
//uniform Material material;
//uniform Light light;

uniform vec3 viewPos; 
uniform vec3 lightColor;
//uniform vec3 objectColor

uniform vec4 MainColor;
uniform sampler2D Texture;

const int NUM_LIGHTS = 100;
uniform vec3 lights[NUM_LIGHTS];
uniform vec3 lightsPos[NUM_LIGHTS];
uniform int numLights; // inside simple model component to limit for loop iterations

vec3 calculateAmbientLighting(const vec4 texel); // NOTE: cannot pass by reference / use inline, 
vec3 calculatePointLight(const vec3 lightPos, const vec3 lightColor, const vec3 norm, const vec4 texel);

void main()
{
    vec4 texel;// = texture(Texture, TexCoord);
    if(float(textureSize(Texture,0).x) > 1)
    {
        texel = texture(Texture, TexCoord);
    }
    else
    {
        texel = vec4(1.0,1.0,1.0,1.0);
    }
  	
    // diffuse 
    vec3 norm = normalize(Normal);

    int i;
    vec3 result = vec3(0.0f);
    for (i = 0; i < numLights; i++) {
        result += calculatePointLight(lightsPos[i], lights[i], norm, texel);
    }
    FragColor = vec4(result, 1.0) * MainColor;
}

vec3 calculateAmbientLighting(const vec4 texel) {
    float ambientStrength = 0.11;
    return ambientStrength * texel.rgb;
}

vec3 calculatePointLight(const vec3 lightPos, const vec3 lightColor, const vec3 norm, const vec4 texel) {
    vec3 lightDir = normalize(lightPos - FragPos);    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texel.rgb;
    
    // specular
    float specularStrength = 0.0; // 0.5;
    vec3 viewDir = normalize(viewPos - FragPos); // TODO: assign viewPos. Currently nothing inputted
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //32 = shininess
    vec3 specular = specularStrength * spec * lightColor;
    return calculateAmbientLighting(texel) + diffuse + specular;
}