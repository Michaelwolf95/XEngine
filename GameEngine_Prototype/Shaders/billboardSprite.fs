#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;

uniform float LifeLevel;
uniform vec4 MainColor;

void main()
{
	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, UV ) * MainColor;
	
	//// Hardcoded life level, should be in a separate texture.
	//if (UV.x < LifeLevel && UV.y > 0.3 && UV.y < 0.7 && UV.x > 0.04 )
	//	color = vec4(0.2, 0.8, 0.2, 1.0); // Opaque green
}