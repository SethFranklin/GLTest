
#pragma once

#include "GameObject.h"

class Renderer : public GameObject
{

public:

	Renderer();
	virtual void RenderObject();

private:

protected:

	struct RenderImpl;
	RenderImpl* Render;

};