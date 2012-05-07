#ifndef MRAY_H
#define MRAY_H

#include "raisetypes.h"
#include "mvector3.h"

class MRay3
{
public:
	vec3 Point; // point
	vec3 Direction; // direction
	/*void extend(float mult); // extend it
	float dist(); // distance*/
};

#endif