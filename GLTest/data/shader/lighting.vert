#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TextureCoord;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

out vec2 TextureUV;

void main()
{

	gl_Position = Projection * View * Model * vec4(Position, 1.0);
	TextureUV = TextureCoord;

}