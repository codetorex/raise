#ifndef MVECTOR_H
#define MVECTOR_H

#include "math.h"

class RDLL MVector3f
{
public:
	union
	{
		struct  
		{
			float		x,y,z;
		};
		
		float		v[3];
	};
	

	MVector3f		();
	MVector3f		(float x_,float y_,float z_);

	void		set(float x_, float y_, float z_);
	void		set(float* k);
	float		length();
	void		normalize();

	inline void clamp(float maxx,float maxy,float maxz);

	MVector3f	operator +  (const MVector3f &other);
	MVector3f	operator -  (const MVector3f &other);
	MVector3f	operator *  (const MVector3f &other);
	MVector3f	operator /  (const MVector3f &other);


	MVector3f	operator /	(const float factor);
	MVector3f	operator *  (const float factor);
	friend MVector3f operator * (const float factor, const MVector3f &other);

	MVector3f&	operator *= (const float factor);
	MVector3f&	operator =  (const MVector3f &other);
	MVector3f&	operator =  (float* other);
	MVector3f&	operator += (const MVector3f &other);
	MVector3f&	operator -= (const MVector3f &other);
	MVector3f&	operator /= (const float factor);

	MVector3f	operator + (void) const;
	MVector3f	operator - (void) const;
};

// vec3 functions
MVector3f::MVector3f()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

MVector3f::MVector3f(float x_,float y_,float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

void MVector3f::set(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

float MVector3f::length()
{
	return( (float)sqrt( x * x + y * y + z * z ) );
}

void MVector3f::normalize()
{
	float fLength = length();

	x = x / fLength;
	y = y / fLength;
	z = z / fLength;
}

void MVector3f::clamp(float maxx,float maxy,float maxz)
{
	if (x > maxx) x = maxx;
	if (y > maxy) y = maxy;
	if (z > maxz) z = maxz;
}
MVector3f MVector3f::operator + ( const MVector3f &other )
{
	MVector3f vResult;

	vResult.x = x + other.x;
	vResult.y = y + other.y;
	vResult.z = z + other.z;

	return vResult;
}

MVector3f MVector3f::operator - (   const MVector3f &other )
{
	MVector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x - other.x;
	vResult.y = y - other.y;
	vResult.z = z - other.z;

	return vResult;
}

MVector3f MVector3f::operator - ( void ) const
{
	MVector3f vResult(-x, -y, -z);

	return vResult;
}

MVector3f& MVector3f::operator *= ( const float factor)
{
	x *= factor;
	y *= factor;
	z *= factor;
	return *this;
}

MVector3f MVector3f::operator * (   const MVector3f &other )
{
	MVector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x * other.x;
	vResult.y = y * other.y;
	vResult.z = z * other.z;

	return vResult;
}

MVector3f MVector3f::operator * (  const float factor )
{
	MVector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x * factor;
	vResult.y = y * factor;
	vResult.z = z * factor;

	return vResult;
}

MVector3f& MVector3f::operator /= ( const float factor )
{
	x /= factor;
	y /= factor;
	z /= factor;
	return *this;
}

MVector3f MVector3f::operator / ( const float factor )
{
	MVector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x / factor;
	vResult.y = y / factor;
	vResult.z = z / factor;

	return vResult;
}

MVector3f operator * (  const float factor,   const MVector3f &other )
{
	MVector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = other.x * factor;
	vResult.y = other.y * factor;
	vResult.z = other.z * factor;

	return vResult;
}

MVector3f MVector3f::operator / (   const MVector3f & other )
{
	MVector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x / other.x;
	vResult.y = y / other.y;
	vResult.z = z / other.z;

	return vResult;
}

MVector3f& MVector3f::operator = (   const MVector3f & other )
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

MVector3f& MVector3f::operator = (   float *other )
{
	x = other[0];
	y = other[1];
	z = other[2];

	return *this;
}


MVector3f& MVector3f::operator += (   const MVector3f & other )
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

MVector3f& MVector3f::operator -= (   const MVector3f & other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

void MVector3f::set(float *k)
{
	x = k[0];
	y = k[1];
	z = k[2];
}

typedef MVector3f vec3;

#endif