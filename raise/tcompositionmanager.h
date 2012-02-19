#ifndef TCOMPOSITIONMANAGER_H
#define TCOMPOSITIONMANAGER_H

#include "tcompositebuffer.h"

class TCompositeFormatFactory
{
public:
	virtual TBufferFormat* CreateFormat(const TString& name, PrimitiveArray* primitives, const TString& components) = 0;
	virtual TBufferFormat* CreateFormat(const TString& longname, const TString& shortname,PrimitiveArray* primitives, const TString& components) = 0;
};

class TCompositeFormatFactoryGeneric: public TCompositeFormatFactory
{
public:
	static TCompositeFormatFactoryGeneric Instance;

	TBufferFormat* CreateFormat(const TString& name, PrimitiveArray* primitives, const TString& components)
	{
		return new TBufferFormat(name,primitives,components);
	}

	TBufferFormat* CreateFormat(const TString& longname, const TString& shortname,PrimitiveArray* primitives, const TString& components) 
	{
		return new TBufferFormat(longname,shortname,primitives,components);
	}
};

class TCompositionManager
{
public:
	TArray<TCompositionPrimitive*> Primitives;

	TArray<TBufferFormat*> Formats;

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
	inline void SetPrimitive(TCompositionPrimitive& prm, const TString& _name, const TString& _short, int _dataType )
	{
		prm.Set(_name,_short,_dataType);
		AddPrimitive(&prm);
	}

	/**
	 * Sets the composite primitive data and adds it to registry.
	 */
	inline void SetCompositePrimitive(TComposition& comp, const TString& _name, const TString& _short, const TString& components)
	{
		comp.Set(_name,_short,tc_group);
		comp.CreateElementList(&Primitives,components);
		AddPrimitive(&comp);
	}

	/**
	 * Creates a primitive, adds it to registry and returns a pointer to it.
	 */
	inline TCompositionPrimitive* CreatePrimitive(const TString& _name, const TString& _short, int _dataType)
	{
		TCompositionPrimitive* result = new TCompositionPrimitive(_name,_short,_dataType);
		AddPrimitive(result);
		return result;
	}

	/**
	 * Creates a format using long naming and short naming and adds it to format registry.
	 */
	inline TBufferFormat* CreateFormat(const TString& longname,const TString& shortname, const TString& components)
	{
		TBufferFormat* result = Factory->CreateFormat(longname,shortname, &Primitives,components);
		AddFormat(result);
		return result;
	}

	/**
	 * Creates a format using shortname and adds it to format registry.
	 */
	inline TBufferFormat* CreateFormat(const TString& name, const TString& components)
	{
		TBufferFormat* result = Factory->CreateFormat(name, &Primitives,components);
		AddFormat(result);
		return result;
	}
};

#endif