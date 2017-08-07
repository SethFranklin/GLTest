
#pragma once

#include <string>
#include <map>

enum Uniform
{

	COLOR_UNIFORM,
	LIGHTING_UNIFORM,

};

class Shader
{

public:

	Shader(std::string Name, std::map<Uniform, std::string> StringMap);
	~Shader();

	void Use();

	void SetUniform(Uniform UniformToSet, int Data);
	void SetUniform(Uniform UniformToSet, float Data);
	void SetUniform(Uniform UniformToSet, float D1, float D2, float D3, float D4);
	void SetUniform(Uniform UniformToSet, bool Data);

private:
	
	struct Impl_;
	Impl_* Impl;

};