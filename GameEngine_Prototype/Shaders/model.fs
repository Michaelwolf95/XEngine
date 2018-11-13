#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 MainColor;
uniform sampler2D Texture;


void main()
{
    FragColor = texture(Texture, TexCoord)* MainColor;
}