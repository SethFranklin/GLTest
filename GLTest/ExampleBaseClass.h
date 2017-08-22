#pragma once

#include "Renderer.h"
#include "Gravity.h"
#include "PointLight.h"

// Put all of the classes and their implementations in the same file since im lazy

class Sun : public Renderer, public PointLight
{

	Sun()
	{



	}

};

class Planet : public Renderer, public Gravity
{

	Planet()
	{



	}

};

class Star : public Renderer, public Gravity, public PointLight
{

	Star()
	{



	}

};