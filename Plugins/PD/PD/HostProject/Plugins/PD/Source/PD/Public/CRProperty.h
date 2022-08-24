#pragma once
#include "PDTypes.h"

struct CRProperty
{
	PDScalar l0;		//keep m now.
	PDScalar E;
	PDScalar Poisson;
	PDScalar Density;
	PDScalar r0;
	PDScalar A;
	Vec3 e3;
	PDScalar EndM;

	CRProperty(PDScalar l0,
	PDScalar E,
	PDScalar poisson,
	PDScalar density,
	PDScalar r0,
	Vec3 e3):l0(l0),E(E),Poisson(poisson),Density(density),r0(r0),e3(e3),EndM(0)
	{
		A = r0*r0*PI;
	};
	CRProperty(PDScalar l0,
	PDScalar E,
	PDScalar poisson,
	PDScalar density,
	PDScalar r0,
	Vec3 e3,
	PDScalar EndM):l0(l0),E(E),Poisson(poisson),Density(density),r0(r0),e3(e3),EndM(EndM)
	{
		A = r0*r0*PI;
	};
};
