#ifndef MMATRIX_H
#define	MMATRIX_H

#include "raisetypes.h"
#include "mvector3.h"
#include "tmemorydriver.h"

class MMatrix4x4
{
public:
	union 
	{
		struct 
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
		float n[16]; // careful using this: _11 = 0, _21 = 1 , _31 = 2...
	};

	MMatrix4x4()
	{
		Identity();
	}

	inline void Identity()
	{
		MemoryDriver::Set(&n,0,16 * sizeof(float));
		_11 = _22 = _33 = _44 = 1.0f;
	}

	/**
	* Changes translation values of matrix.
	* May need to Identity() first, if it was dirty.
	*/
	inline void Translate(const vec3& value)
	{
		_41 = value.x;
		_42 = value.y;
		_43 = value.z;
	}


	/**
	* Changes translation values of matrix.
	* May need to Identity() first, if it was dirty.
	*/
	inline void Translate(float x,float y,float z)
	{
		_41 = x;
		_42 = y;
		_43 = z;
	}

	/**
	* Changes scale values of matrix.
	* May need to Identity() first, if it was dirty.
	*/
	inline void Scale(float x, float y, float z)
	{
		_11 = x;
		_22 = y;
		_33 = z;
	}

	/**
	* Changes scale values of matrix.
	* May need to Identity() first, if it was dirty.
	*/
	inline void Scale(const vec3& value)
	{
		_11 = value.x;
		_22 = value.y;
		_33 = value.z;
	}
};


class MProjectionMatrix: public MMatrix4x4
{
public:

	/**
	* Creates right handed orthographic projection matrix.
	*/
	inline void OrthoR(float width,float height, float znear, float zfar)
	{
		_11 = 2.0f / width;
		_22 = 2.0f / height;

		float depth = znear - zfar;
		_33 = 1.0f / (depth);
		_43 = znear / depth;
	}

	/**
	* Creates left handed orthographic projection matrix.
	*/
	inline void OrthoL(float width,float height, float znear, float zfar)
	{
		_11 = 2.0f / width;
		_22 = 2.0f / height;
		_33 = 1.0f / (zfar - znear);
		_43 = znear / znear - zfar;
	}

	inline void OrthoOffCenterR(float left,float right,float top,float bottom,float znear,float zfar )
	{
		_11 = 2/ (right - left);
		_22 = 2/ (top - bottom);
		_33 = 1 / (znear - zfar);
		_41 = (left+right)/(left-right);
		_42 = (top+bottom) / (bottom-top);
		_43 = znear / (znear - zfar);
	}

	inline void OrthoOffCenterL(float left,float right,float top,float bottom,float znear,float zfar )
	{
		_11 = 2/ (right - left);
		_22 = 2/ (top - bottom);
		_33 = 1 / (zfar - znear);
		_41 = (left+right)/(left-right);
		_42 = (top+bottom) / (bottom-top);
		_43 = znear / (znear - zfar);
	}
};

typedef MMatrix4x4 mat4;

#endif