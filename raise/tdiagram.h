#ifndef TDIAGRAM_H
#define TDIAGRAM_H

#include "raisetypes.h"
#include "tevent.h"
#include "tarray.h"

class TDiagram
{
public:

};


template <class T>
class TDiagramOutput;

template <class T>
class TDiagramInput
{
public:
	T Value; // used for simple connection
	
	TDiagramOutput< T >* Connection;

	inline void Connect( const TDiagramOutput<T>& Output )
	{
		Output.Connections.Add(&Value);
		Value = Output.Value;
		Connection = &Output;
	}

	inline void Disconnect()
	{
		if (Connection)
		{
			Connection->Connections.Remove(&Value);
			Connection = 0;
		}
	}

	TDiagramInput()
	{
		Connection = 0;
	}

	~TDiagramInput()
	{
		Disconnect();
	}
};

template <class iT,class oT>
class TDiagramConverter
{
public:
	oT* TargetPtr;
	
	void UpdateValue(iT value)
	{
		*oT = (oT)value;
	}
};


enum TDiagramOutputType
{
	TDG_BOOL,
	TDG_BYTE,
	TDG_SHORT,
	TDG_INT,
	TDG_USHORT,
	TDG_UINT,
	TDG_FLOAT,
};

class TDiagramOutputMemory
{
public:
	TDiagramOutputType TargetType;
	void* TargetPtr;
};

template <class T>
class TDiagramOutput
{
public:
	typedef delegate1<void,T> OutputEvent;

	T								Value;
	event < OutputEvent >			ValueChanged; // used for event connection ( maximum 8 connections )
	Array< T* >					Connections; // dangerous! low level but fast!
	Array< TDiagramOutputMemory >	MemoryConvertingConnections;
	
	void ConnectMemory(T* ptrInput)
	{
		Connections.Add(ptrInput);
	}

	void ConnectMemoryConverting(void* ptrInput, TDiagramOutputType convertType)
	{
		TDiagramOutputMemory om;
		om.TargetType = convertType;
		om.TargetPtr = ptrInput;
		MemoryConvertingConnections.Add(om);
	}

	/**
	 * Updates InputsPins in UpdatableConnections array.
	 */
	void UpdateConnections()
	{
		for (ui32 i=0;i<Connections.Count;i++)
		{
			*Connections.Items[i] = Value;
		}

		for (ui32 i=0;i<MemoryConvertingConnections.Count;i++)
		{
			TDiagramOutputMemory& curOutput = MemoryConvertingConnections.Items[i];
			switch(curOutput.TargetType)
			{
			case TDG_BOOL:
				*(bool*)curOutput.TargetPtr = Value > 0.000001f; // epsilon
				continue;

			case TDG_BYTE:
				*(byte*)curOutput.TargetPtr = (byte)Value;
				continue;

			case TDG_SHORT:
				*(short*)curOutput.TargetPtr = (short)Value;
				continue;

			case TDG_INT:
				*(int*)curOutput.TargetPtr = (int)Value;
				continue;

			case TDG_USHORT:
				*(unsigned short*)curOutput.TargetPtr = (unsigned short)Value;
				continue;

			case TDG_UINT:
				*(ui32*)curOutput.TargetPtr = (ui32)Value;
				continue;

			case TDG_FLOAT:
				*(float*)curOutput.TargetPtr = (float)Value;
				continue;
			}
		}

		ValueChanged.call(Value);
	}

	inline void Set(const T& v)
	{
		Value = v;
		UpdateConnections();
	}
};

class TDiagramSocket
{
public:
	enum TDiagramSocketType
	{
		DS_INPUT,
		DS_OUTPUT,
	};

	TDiagramSocketType Type;
	int Size;
	int Offset;
};

class TDiagramObjectDescription
{
public:
	Array< TDiagramSocket > Inputs;
	Array< TDiagramSocket > Outputs;
};

#endif