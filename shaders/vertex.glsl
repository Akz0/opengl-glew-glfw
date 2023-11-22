#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in vec3 aNormal;

out vec2 TexCord;
out vec3 Color;
out vec3 Normal;
out vec3 CurrentPosition;

uniform mat4 CameraMatrix;
uniform mat4 model;

void main()
{
   CurrentPosition = vec3(model * vec4(aPos,1.0f));
   gl_Position = CameraMatrix * vec4(CurrentPosition, 1.0);
   
   Color = aColor; 
   TexCord = aTexture;
   Normal = aNormal;
}