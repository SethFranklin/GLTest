
#include <string>
#include <map>

#include <glew.h>

#include "Shader.h"
#include "Read.h"
#include "Debug.h"

struct Shader::Impl_
{

	Impl_(std::string Name, std::map<Uniform, std::string> Map);
	~Impl_();

	void Use();

	void SetUniform(Uniform UniformToSet, int Data);
	void SetUniform(Uniform UniformToSet, float Data);
	void SetUniform(Uniform UniformToSet, float D1, float D2, float D3, float D4);
	void SetUniform(Uniform UniformToSet, bool Data);

	std::map<Uniform, int> UniformMap;
	GLuint ShaderProgram;

};

Shader::Shader(std::string Name, std::map<Uniform, std::string> StringMap)
{

	Impl = new Impl_(Name, StringMap);

}

Shader::~Shader()
{

	delete Impl;

}

void Shader::Use()
{

	Impl->Use();

}

void Shader::SetUniform(Uniform UniformToSet, int Data)
{

	Impl->SetUniform(UniformToSet, Data);

}

void Shader::SetUniform(Uniform UniformToSet, float Data)
{

	Impl->SetUniform(UniformToSet, Data);

}

void Shader::SetUniform(Uniform UniformToSet, float D1, float D2, float D3, float D4)
{

	Impl->SetUniform(UniformToSet, D1, D2, D3, D4);

}

void Shader::SetUniform(Uniform UniformToSet, bool Data)
{

	Impl->SetUniform(UniformToSet, Data);

}

// Implementation Start

Shader::Impl_::Impl_(std::string Name, std::map<Uniform, std::string> StringMap)
{

	std::string VertexData = Read::File("data/shader/" + Name + ".vert");
	std::string FragmentData = Read::File("data/shader/" + Name + ".frag");

	const char* VertexSource = VertexData.c_str();
	const char* FragmentSource = FragmentData.c_str();

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(VertexShader, 1, &VertexSource, NULL);
	glShaderSource(FragmentShader, 1, &FragmentSource, NULL);

	glCompileShader(VertexShader);
	glCompileShader(FragmentShader);

	int Success;
	char InfoLog[512];

	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{

		glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);

		Debug::Log("Vertex Shader Compile Error: " + std::string(InfoLog));

	}

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{

		glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);

		Debug::Log("Fragment Shader Compile Error: " + std::string(InfoLog));

	}

	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);

	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (!Success)
	{

		glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);

		Debug::Log("Shader Program Link Error: " + std::string(InfoLog));

	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	for (std::map<Uniform, std::string>::iterator i = StringMap.begin(); i != StringMap.end(); i++)
	{

		UniformMap.insert(std::pair<Uniform, int>(i->first, glGetUniformLocation(ShaderProgram, i->second.c_str())));

	}

}

Shader::Impl_::~Impl_()
{

	glDeleteProgram(ShaderProgram);

}

void Shader::Impl_::Use()
{

	glUseProgram(ShaderProgram);

}

void Shader::Impl_::SetUniform(Uniform UniformToSet, int Data)
{

	glUniform1i(UniformMap[UniformToSet], Data);

}

void Shader::Impl_::SetUniform(Uniform UniformToSet, float Data)
{

	glUniform1f(UniformMap[UniformToSet], Data);

}

void Shader::Impl_::SetUniform(Uniform UniformToSet, float D1, float D2, float D3, float D4)
{

	glUniform4f(UniformMap[UniformToSet], D1, D2, D3, D4);

}

void Shader::Impl_::SetUniform(Uniform UniformToSet, bool Data)
{

	glUniform1i(UniformMap[UniformToSet], int(Data));

}