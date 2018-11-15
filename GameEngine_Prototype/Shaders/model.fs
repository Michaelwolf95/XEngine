#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

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
    FragColor = texture(Texture, TexCoord)* MainColor;
    //FragColor = texel* MainColor;
}