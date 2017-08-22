#pragma once

#include "GameObject.h"

class Gravity : public GameObject
{

public:

	Gravity();
	virtual void UpdateObject(float DeltaTime);

private:

protected:

	struct GravityImpl;
	GravityImpl* Grav;

};