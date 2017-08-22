
#pragma once

#include <string>
#include <map>

#include <glm.hpp>

enum Uniform
{

	COLOR_UNIFORM,
	LIGHTING_UNIFORM,
	TIME_UNIFORM,
	TEXTURE0_UNIFORM,
	TEXTURE1_UNIFORM,
	TEXTURE2_UNIFORM,
	TEXTURE3_UNIFORM,
	TEXTURE4_UNIFORM,
	TEXTURE5_UNIFORM,
	TEXTURE6_UNIFORM,
	TEXTURE7_UNIFORM,
	TEXTURE8_UNIFORM,
	TEXTURE9_UNIFORM,
	TEXTURE10_UNIFORM,
	TEXTURE11_UNIFORM,
	TEXTURE12_UNIFORM,
	TEXTURE13_UNIFORM,
	TEXTURE14_UNIFORM,
	TRANSFORM_UNIFORM,
	MODEL_UNIFORM,
	PROJECTION_UNIFORM,
	VIEW_UNIFORM

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
	void SetUniform(Uniform UniformToSet, glm::mat4 Data);

private:
	
	struct Impl_;
	Impl_* Impl;

};