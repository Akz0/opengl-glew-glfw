#version 330 core

in vec3 Color;
in vec2 TexCord;
out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
	//FragColor = vec4(Color, 1.0f);
   FragColor = texture(tex0,TexCord);
}