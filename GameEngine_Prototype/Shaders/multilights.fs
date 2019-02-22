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

in vec3 FragPos;  
in vec2 TexCoord;
in vec3 Normal;  
  
uniform vec3 lightPos;
//uniform Material material;
//uniform Light light;

uniform vec3 viewPos; 
uniform vec3 lightColor;
//uniform vec3 objectColor;

uniform vec4 MainColor;
uniform sampler2D Texture;


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

    // ambient
    float ambientStrength = 0.11;
    vec3 ambient = ambientStrength * texel.rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  diff * lightColor * texel.rgb;
    
    // specular
    float specularStrength = 0.0; // 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //32 = shininess
    vec3 specular = specularStrength * spec * lightColor;  
        
    //vec4 result = vec4((ambient + diffuse + specular),1.0) * MainColor;    
    vec4 result = vec4((ambient + diffuse + specular), 1.0) * MainColor;  // does MainColor even make a difference?

    FragColor = result;
}