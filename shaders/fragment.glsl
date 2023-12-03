#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCord;
in vec3 Normal;
in vec3 CurrentPosition;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform vec3 CameraPosition;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - CurrentPosition);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float SpecularLight = 0.5f;
	vec3 ViewDirection = normalize(CameraPosition - CurrentPosition);
	vec3 ReflectionDirection = reflect(-lightDirection, normal);
	float SpecularAmount = pow(max(dot(ViewDirection,ReflectionDirection),0.0f),16);
	float specular = SpecularAmount * SpecularLight;

	float ambient = 0.20f;

	FragColor = texture(tex0, TexCord) * lightColor * (diffuse + ambient) + texture(tex1, TexCord).r * specular;
}