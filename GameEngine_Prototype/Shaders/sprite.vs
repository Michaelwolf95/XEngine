#version 330 core
//layout (location = 0) in vec3 aPos;
layout (location = 0) in vec4 vertex; 
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 MainColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//FragPos = vec3(model * vec4(aPos, 1.0));
	//FragPos = vec3(vec4(aPos, 1.0));
    //gl_Position = vec4(aPos, 1.0);
    MainColor = aColor;

	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	TexCoord = vertex.zw;

	//gl_Position = projection * view *  vec4(FragPos, 1.0);
	//gl_Position = projection * view *  vec4(FragPos, 1.0);
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
	//gl_Position = vec4(aPos, 1.0);
}