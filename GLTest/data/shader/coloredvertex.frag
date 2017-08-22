#version 330 core

in vec4 Color;
in vec2 TextureUV;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float Time;

out vec4 FragmentColor;

vec2 FinalUV;

void main()
{

	FinalUV = vec2(TextureUV.x, (sin(Time + TextureUV.x)) + TextureUV.y);
	FragmentColor = Color * mix(texture(Texture2, FinalUV), texture(Texture1, FinalUV), (sin(Time * 2.0) + 1.0) / 2.0);

}