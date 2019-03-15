#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;   // unused for now
uniform sampler2D texture_normal1;     // unused for now
uniform sampler2D texture_height1;     // unused for now

uniform vec4 MainColor;

void main()
{   
    vec4 texel;

    if(float(textureSize(texture_diffuse1,0).x) > 1) {
        texel = texture(texture_diffuse1, TexCoords);
    }
    else {
        texel = vec4(1.0,1.0,1.0,1.0);
    }
    //vec4 output1 = vec4(0.0f);
    //output1 = texture(texture_specular1, TexCoords);

    
    vec3 norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos - FragPos); // TODO: assign viewPos. Currently nothing inputted

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse =  diff * lightColor * texel.rgb;
    //output1 += texture(texture_specular1, TexCoords);  // unused
    //output1 += texture(texture_normal1, TexCoords);    // for
    //output1 += texture(texture_height1, TexCoords);    // now

    float specularStrength = 0.3f; // hardcoded specular strength for now
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16); //32 = shininess property of Material
    vec3 specular = specularStrength * spec * lightColor;

    float ambientStrength = 0.11f;
    vec3 ambience = vec3(0.1f) * ambientStrength; // ambient hardcoded for now
    


    //FragColor = texture(texture_diffuse1, TexCoords) * MainColor;
    FragColor = vec4(ambience + diffuse + specular, 1.0f);// * MainColor;
}