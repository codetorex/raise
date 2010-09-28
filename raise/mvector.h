#ifndef MVECTOR_H
#define MVECTOR_H

#include "raisetypes.h"
#include <math.h>

// Faster one with assembly SSE or MMX can be implemented.

template <class T>
class RDLL MVector3
{
public:
	union
	{
		struct  
		{
			T x,y,z;
		};
		
		T v[3];
	};
	

	MVector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	MVector3 (T x_,T y_,T z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	void set(T x_, T y_, T z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	void set(T* k)
	{
		x = k[0];
		y = k[1];
		z = k[2];
	}
	
	inline T length() const
	{
		return( (T)sqrt( x * x + y * y + z * z ) );
	}

	void normalize()
	{
		T fLength = length();

		x = x / fLength;
		y = y / fLength;
		z = z / fLength;
	}

	inline void clamp(T maxx,T maxy,T maxz)
	{
		if (x > maxx) x = maxx;
		if (y > maxy) y = maxy;
		if (z > maxz) z = maxz;
	}

	inline T distance(const MVector3& v2) const
	{
		T dx = x - v2.x;
		T dy = y - v2.y;
		T dz = z - v2.z;

		return (T)sqrt( dx * dx + dy * dy + dz * dz );
	}

	inline static T dot(const MVector3& v1,const MVector3& v2)
	{
		return v1.dot(v2);
	}

	inline static MVector3 cross(const MVector3& v1,const MVector3& v2)
	{
		return v1.cross(v2);
	}


	inline T dot(const MVector3& v2) const
	{
		return (x * v2.x) + (y * v2.y) + (z * v2.z);
	}

	inline MVector3& cross(const MVector3& v2) const
	{
		MVector3 vCrossProduct;

		vCrossProduct.x =  y * v2.z - z * v2.y;
		vCrossProduct.y = -x * v2.z + z * v2.x;
		vCrossProduct.z =  x * v2.y - y * v2.x;

		return vCrossProduct;
	}
	
	inline MVector3& interpolate(const MVector3& vdest, T tween)
	{
		MVector3 tw(vdest);
		tw *= tween;
		tw +=((1.0f-tween) * *this);
		return tw;
	}

	/**
	* Extend as in line.
	*/
	inline MVector3& extend(const MVector3& vdir, T factor)
	{
		MVector3 resul(*this);
		resul -= (factor * vdir);
		return resul;
	}

	/**
	* angle between two vectors.
	*/
	inline T angle(const MVector3& v2)
	{
		T k = this->dot(v2);
		k /= (length() * v2.length());
		k = RADTODEG( ((T)acos(k)) );
		return k;
	}


	/**
	* basic reflection function
	*/
	inline MVector3& reflect(const MVector3& normal)
	{
		MVector3 res(*this); // R = I - 2*(N·I)*N
		res -= (2.0f * dot(normal)) * normal;
		return res;
	}


	MVector3 operator + (const MVector3 &other)
	{
		MVector3 vResult;

		vResult.x = x + other.x;
		vResult.y = y + other.y;
		vResult.z = z + other.z;

		return vResult;
	}

	MVector3 operator -  (const MVector3 &other)
	{
		MVector3 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x - other.x;
		vResult.y = y - other.y;
		vResult.z = z - other.z;

		return vResult;
	}

	MVector3 operator *  (const MVector3 &other)
	{
		MVector3 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x * other.x;
		vResult.y = y * other.y;
		vResult.z = z * other.z;

		return vResult;
	}

	MVector3 operator /  (const MVector3 &other)
	{
		MVector3 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x / other.x;
		vResult.y = y / other.y;
		vResult.z = z / other.z;

		return vResult;
	}


	MVector3 operator /	(const T factor)
	{
		MVector3 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x / factor;
		vResult.y = y / factor;
		vResult.z = z / factor;

		return vResult;
	}

	MVector3 operator *  (const T factor)
	{
		MVector3 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x * factor;
		vResult.y = y * factor;
		vResult.z = z * factor;

		return vResult;
	}

	friend MVector3 operator * (const T factor, const MVector3 &other)
	{
		MVector3 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = other.x * factor;
		vResult.y = other.y * factor;
		vResult.z = other.z * factor;

		return vResult;
	}

	MVector3& operator *= (const T factor)
	{
		x *= factor;
		y *= factor;
		z *= factor;
		return *this;
	}

	MVector3& operator =  (const MVector3 &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	MVector3& operator =  (T* other)
	{
		x = other[0];
		y = other[1];
		z = other[2];
		return *this;
	}

	MVector3& operator += (const MVector3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	MVector3& operator -= (const MVector3 &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	MVector3& operator /= (const T factor)
	{
		x /= factor;
		y /= factor;
		z /= factor;
		return *this;
	}

	MVector3 operator + (void) const;

	MVector3 operator - (void) const
	{
		MVector3 vResult(-x, -y, -z);
		return vResult;
	}
};


typedef MVector3<float>		vec3;
typedef MVector3<double>	vecd3;

#endif