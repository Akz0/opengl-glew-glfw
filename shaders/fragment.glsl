#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCord;
in vec3 Normal;
in vec3 CurrentPosition;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform vec3 CameraPosition;


vec4 PointLight(){
	
	vec3 LightVector = lightPosition - CurrentPosition;
	float distance = length(LightVector);

	float a = 3.1f;
	float b = 1.2f;

	float intensity = 1.0f / (a*distance*distance + b*distance + 1.0f);
	
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - CurrentPosition);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float SpecularLight = 0.5f;
	vec3 ViewDirection = normalize(CameraPosition - CurrentPosition);
	vec3 ReflectionDirection = reflect(-lightDirection, normal);
	float SpecularAmount = pow(max(dot(ViewDirection,ReflectionDirection),0.0f),16);
	float specular = SpecularAmount * SpecularLight;

	float ambient = 0.20f;

	return (texture(diffuse0, TexCord) * (diffuse + ambient + intensity) + texture(specular0, TexCord).r * specular) * lightColor;
}

vec4 SpotLight(){
	float outerCone = 0.90f;
	float innerCone = 0.95f;
	
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - CurrentPosition);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float SpecularLight = 0.5f;
	vec3 ViewDirection = normalize(CameraPosition - CurrentPosition);
	vec3 ReflectionDirection = reflect(-lightDirection, normal);
	float SpecularAmount = pow(max(dot(ViewDirection,ReflectionDirection),0.0f),16);
	float specular = SpecularAmount * SpecularLight;

	float ambient = 0.20f;

	float angle = dot(vec3(0.0f,-1.0f,0.0f),-lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone),0.0f,1.0f);
	return (texture(diffuse0, TexCord) * (diffuse + ambient + intensity) + texture(specular0, TexCord).r * specular *intensity) * lightColor;
}

vec4 DirectionLight(){
	
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f,1.0f,0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float SpecularLight = 0.5f;
	vec3 ViewDirection = normalize(CameraPosition - CurrentPosition);
	vec3 ReflectionDirection = reflect(-lightDirection, normal);
	float SpecularAmount = pow(max(dot(ViewDirection,ReflectionDirection),0.0f),16);
	float specular = SpecularAmount * SpecularLight;

	float ambient = 0.20f;

	return (texture(diffuse0, TexCord) * (diffuse + ambient ) + texture(specular0, TexCord).r * specular) * lightColor;
}

void main()
{
	FragColor = PointLight();
}