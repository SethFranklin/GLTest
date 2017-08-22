
#include "Gravity.h"

struct Gravity::GravityImpl
{

	void UpdateObject(float DeltaTime);

	// Gravity parameters, mass, mass of center object
	// Center object at zero, zero

};

Gravity::Gravity()
{



}

void Gravity::UpdateObject(float DeltaTime)
{

	Grav->UpdateObject(DeltaTime);

}

void Gravity::GravityImpl::UpdateObject(float DeltaTime)
{

	// Physics code

}