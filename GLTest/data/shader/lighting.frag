#version 330 core

in vec2 TextureUV;
in vec4 Normal;

uniform sampler2D Texture;

out vec4 FragmentColor;

float Light;
float Ambient = 0.0;
float LightStr = 1.0;
vec3 LightPos = vec3(-0.57735026919, -0.57735026919, -0.57735026919);

void main()
{

	Light = mix(Ambient, LightStr, max(-dot(Normal.xyz, LightPos), 0.0));

	FragmentColor = texture(Texture, TextureUV) * Light;

}