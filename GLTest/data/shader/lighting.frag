#version 330 core

in vec2 TextureUV;

uniform sampler2D Texture;

out vec4 FragmentColor;

void main()
{

	FragmentColor = texture(Texture, TextureUV);

}