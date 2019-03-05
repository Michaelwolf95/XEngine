#version 330 core
out vec4 FragColor;

uniform vec4 MainColor;
uniform sampler2D Texture;
in vec2 TexCoord;
in vec3 FragPos; 

// texture samplers
//uniform sampler2D texture1;
//uniform sampler2D texture2;


void main()
{
    FragColor = texture(Texture, TexCoord) * MainColor;
}