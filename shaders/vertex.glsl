#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec2 TexCord;
out vec3 Color;
uniform float scale;

uniform mat4 CameraMatrix;
void main()
{
   gl_Position = CameraMatrix * vec4(aPos, 1.0);
   Color = aColor; 
   TexCord = aTexture;
}