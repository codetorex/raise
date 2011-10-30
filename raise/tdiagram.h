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

	void Connect( const TDiagramOutput<T>& Output );
};

class TDiagramOutputMemory
{
public:
	enum TTargetType
	{
		TT_BYTE,
		TT_SHORT,
		TT_INT,
		TT_USHORT,
		TT_UINT,
		TT_FLOAT,
	};

	TTargetType TargetType;
	void* TargetPtr;
};

template <class T>
class TDiagramOutput
{
public:
	typedef delegate1<void,T> OutputEvent;

	T								Value;
	event < OutputEvent >			ValueChanged; // used for event connection ( maximum 8 connections )
	TArray< TDiagramInput<T>* >		Connections;
	TArray< T* >					MemoryConnections; // dangerous! low level but fast!
	TArray< TDiagramOutputMemory >	MemoryConvertingConnections;
	
	void ConnectMemory(T* ptrInput)
	{
		MemoryConnections.Add(ptrInput);
	}

	void ConnectMemoryConverting(void* ptrInput, TDiagramOutputMemory::TTargetType convertType)
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
		for (ui32 i=0;i<MemoryConnections.Count;i++)
		{
			*MemoryConnections.Item[i] = Value;
		}

		for (ui32 i=0;i<MemoryConvertingConnections.Count;i++)
		{
			TDiagramOutputMemory& curOutput = MemoryConvertingConnections.Item[i];
			switch(curOutput.TargetType)
			{
			case TDiagramOutputMemory::TT_BYTE:
				*(byte*)curOutput.TargetPtr = (byte)Value;
				continue;

			case TDiagramOutputMemory::TT_SHORT:
				*(short*)curOutput.TargetPtr = (short)Value;
				continue;

			case TDiagramOutputMemory::TT_INT:
				*(int*)curOutput.TargetPtr = (int)Value;
				continue;

			case TDiagramOutputMemory::TT_USHORT:
				*(unsigned short*)curOutput.TargetPtr = (unsigned short)Value;
				continue;

			case TDiagramOutputMemory::TT_UINT:
				*(ui32*)curOutput.TargetPtr = (ui32)Value;
				continue;

			case TDiagramOutputMemory::TT_FLOAT:
				*(float*)curOutput.TargetPtr = (float)Value;
				continue;
			}
		}

		//ValueChanged.call(Value);

		/*for (int i=0;i<Connections.Count;i++)
		{
			TDiagramInput* currentInput = Connections[i];

			switch(currentInput->InputType)
			{
			case TDiagramInput::IPA_NONE:
				continue;

			case TDiagramInput::IPA_VALUE:
				currentInput->Value = Value;
				continue;

			case TDiagramInput::IPA_MEMORY:
				*currentInput->ValuePtr = Value;
				continue;

			case TDiagramInput::IPA_EVENT:
				currentInput->ValueEvent.call(Value);
				continue;
			}
		}*/
	}

	inline void Set(const T& v)
	{
		Value = v;
		UpdateConnections();
	}

	/*inline TDiagramOutput<T>& operator= (const T& v)
	{
		Value = v;
		UpdateConnections();
	}*/
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
	TArray< TDiagramSocket > Inputs;
	TArray< TDiagramSocket > Outputs;
};

#endif