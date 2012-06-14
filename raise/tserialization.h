#ifndef TSERIALIZATION_H
#define TSERIALIZATION_H

#include "ttype.h"

/**
* Serialize interface that serializes the objects.
*/
class TSerializer
{
public:
	virtual void SerializeObject(TType* minfo, void* object) = 0;

	inline void Serialize(TObject* obj)
	{
		TType* minfo = obj->GetType();
		SerializeObject(minfo,obj);
	}
};

/**
 * Deserialize interface that deserializes the objects.
 */
class TDeserializer
{
public:
	virtual void DeserializeObject(TType* minfo, void* object) = 0;
	
	inline void Deserialize(TObject* obj)
	{
		TType* minfo = obj->GetType();
		DeserializeObject(minfo,obj);
	}
};


#endif