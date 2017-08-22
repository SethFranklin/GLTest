
#pragma once

#include "Transform.h"

class GameObject
{

public:

	GameObject()
	{



	}

	Transform transform;

	virtual void UpdateObject(float DeltaTime) = 0;
	virtual void RenderObject() = 0;

private:



};