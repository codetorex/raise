#ifndef MRAY_H
#define MRAY_H

#include "raisetypes.h"
#include "mvector3.h"

class MRay
{
public:
	vec3 p; // point
	vec3 d; // direction
	/*void extend(float mult); // extend it
	float dist(); // distance*/
};

#endif