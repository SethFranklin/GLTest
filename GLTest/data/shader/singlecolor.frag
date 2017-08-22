#version 330 core

out vec4 FragmentColor;

uniform float Time;
uniform vec4 Color;

void main()
{

	FragmentColor = (sin(Time * 2.0) + 1.0) * Color / 2.0;

}