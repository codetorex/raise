#ifndef TCOMPOSITIONMANAGER_H
#define TCOMPOSITIONMANAGER_H

#include "tcompositebuffer.h"
#include "tcompositeconverter.h"

class TCompositeFormatFactory
{
public:
	virtual TBufferFormat* CreateFormat(const String& name, PrimitiveArray* primitives, const String& components) = 0;
	virtual TBufferFormat* CreateFormat(const String& longname, const String& shortname,PrimitiveArray* primitives, const String& components) = 0;
};

class TCompositeFormatFactoryGeneric: public TCompositeFormatFactory
{
public:
	static TCompositeFormatFactoryGeneric Instance;

	TBufferFormat* CreateFormat(const String& name, PrimitiveArray* primitives, const String& components)
	{
		return new TBufferFormat(name,primitives,components);
	}

	TBufferFormat* CreateFormat(const String& longname, const String& shortname,PrimitiveArray* primitives, const String& components) 
	{
		return new TBufferFormat(longname,shortname,primitives,components);
	}
};

class TCompositionManager
{
public:
	Array<TCompositionPrimitive*> Primitives;

	Array<TBufferFormat*> Formats;

	TCompositeFormatFactory* Factory;

	TCompositionManager(TCompositeFormatFactory* _factory = 0)
	{
		if (_factory == 0)
		{
			Factory = &TCompositeFormatFactoryGeneric::Instance;
		}
		else
		{
			Factory = _factory;
		}
	}

	inline TBufferFormat* GetFormat(const String& shortname)
	{
		TArrayEnumerator< TBufferFormat* > bf(Formats);
		while(bf.MoveNext())
		{
			if (bf.Current->ShortName == shortname)
			{
				return bf.Current;
			}
		}

		return 0;
	}

	inline void AddConverter(TCompositeConverter* conv)
	{
		conv->SourceFormat->Converters.Add(conv);
	}

	void AddConverter(const String& src, const String& dst, TCompositeConverter* conv)
	{
		TBufferFormat* srcFormat = GetFormat(src);
		TBufferFormat* dstFormat = GetFormat(dst);

		if (srcFormat == 0 || dstFormat == 0)
		{
			throw Exception("No format found");
		}

		conv->SetDirection(srcFormat,dstFormat);

		AddConverter(conv);
	}

	inline void AddPrimitive(TCompositionPrimitive* prm)
	{
		Primitives.Add(prm);
	}

	inline void AddFormat(TBufferFormat* fmt)
	{
		Formats.Add(fmt);
	}

	/**
	 * Sets the primitive data and adds it to registry.
	 */
	inline void SetPrimitive(TCompositionPrimitive& prm, const String& _name, const String& _short, int _dataType )
	{
		prm.Set(_name,_short,_dataType);
		AddPrimitive(&prm);
	}

	/**
	 * Sets the composite primitive data and adds it to registry.
	 */
	inline void SetCompositePrimitive(TComposition& comp, const String& _name, const String& _short, const String& components)
	{
		comp.Set(_name,_short,tc_group);
		comp.CreateElementList(&Primitives,components);
		AddPrimitive(&comp);
	}

	/**
	 * Creates a primitive, adds it to registry and returns a pointer to it.
	 */
	inline TCompositionPrimitive* CreatePrimitive(const String& _name, const String& _short, int _dataType)
	{
		TCompositionPrimitive* result = new TCompositionPrimitive(_name,_short,_dataType);
		AddPrimitive(result);
		return result;
	}

	/**
	 * Creates a format using long naming and short naming and adds it to format registry.
	 */
	inline TBufferFormat* CreateFormat(const String& longname,const String& shortname, const String& components)
	{
		TBufferFormat* result = Factory->CreateFormat(longname,shortname, &Primitives,components);
		AddFormat(result);
		return result;
	}

	/**
	 * Creates a format using shortname and adds it to format registry.
	 */
	inline TBufferFormat* CreateFormat(const String& name, const String& components)
	{
		TBufferFormat* result = Factory->CreateFormat(name, &Primitives,components);
		AddFormat(result);
		return result;
	}
};

#endif