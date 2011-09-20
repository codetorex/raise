#include "stdafx.h"
#include "tbufferedstream.h"


void TBufferedStream::Flush()
{
	if (VBuffer.Index > 0)
	{
		Source->Write(VBuffer.Data,1, VBuffer.Index);
		VBuffer.VirtualRewind();
		Source->Flush();
	}
}

int TBufferedStream::ReadNextChunk()
{
	ui32 CurRealPos = Source->Position();
	int ReadSize = Source->Read(VBuffer.Data,1,VBuffer.Capacity);
	if (ReadSize > 0)
	{
		VBuffer.SetVirtual(CurRealPos,CurRealPos + ReadSize);
	}
	else
	{
		VBuffer.VirtualRewind();
	}
	return ReadSize;
}

int TBufferedStream::ReadBigData( void* buffer, int total )
{
	ui32 RemainingSize = VBuffer.GetAvailable(); // data length in buffer
	if (RemainingSize > 0)
	{
		VBuffer.GetBytes((byte*)buffer,RemainingSize);
	}
	int ReadSize = Source->Read(buffer,1,total - RemainingSize);
	return ReadSize + RemainingSize;
}

int TBufferedStream::ReadSmallData( void* buffer,int total )
{
	int RemainingSize = VBuffer.GetAvailable(); // data length in buffer

	if (RemainingSize >= total)
	{
		VBuffer.GetBytes((byte*)buffer,total);
		return total;
	}
	else
	{
		int writtenBytes = 0;
		if (RemainingSize > 0)
		{
			VBuffer.GetBytes((byte*)buffer,RemainingSize);
			writtenBytes += RemainingSize;
			buffer = (byte*)buffer + RemainingSize;
			total -= RemainingSize;
		}

		if ( ReadNextChunk() != 0 )
		{
			writtenBytes += ReadSmallData(buffer,total);
		}
		return writtenBytes;
	}
}

void TBufferedStream::Write( const void* buffer,int size,int count )
{
	ui32 TotalSize = size * count;
	WriteBuffer = true;

	if (TotalSize > VBuffer.Capacity)
	{
		if (VBuffer.Index > 0)
			Flush(); // if there is still data inside buffer flush it

		Source->Write(buffer,size,count);
	}

	if ((ui32)VBuffer.GetAvailable() < TotalSize)
	{
		Flush(); // if there is still data inside buffer flush it
	}

	VBuffer.AddBytes((byte*)buffer,TotalSize);
}

int TBufferedStream::ReadByte()
{
	if (VBuffer.GetAvailable() == 0)
	{
		if ( ReadNextChunk() == 0 ) // 0 bytes readed
		{
			return -1;
		}
	}
	return VBuffer.ReadByteNoCheck();
}

void TBufferedStream::Seek( ui32 offset,SeekOrigin origin )
{
	switch( origin )
	{
	case sBegin:
		if (VBuffer.VirtualStart <= offset && VBuffer.VirtualEnd >= offset) // if offset is inside buffered area
		{
			VBuffer.Set(offset - VBuffer.VirtualStart);
			return;
		}
		break;

	case sCurrent:
		int available = VBuffer.GetAvailable();
		if (available > 0)
		{
			if (offset <= (ui32)available)
			{
				VBuffer.Advance(offset);
				return;
			}
			else
			{
				Source->Seek(offset - available,sCurrent);
				VBuffer.VirtualRewind();
				return;
			}
		}
		break;
	}

	Source->Seek(offset,origin);
	VBuffer.VirtualRewind();
}

void TBufferedStream::Close()
{
	if (WriteBuffer)
	{
		if (VBuffer.Index > 0)
		{
			Flush();
		}
	}

	Source->Close();
	Source = 0;
	delete this;
}

int TBufferedStream::PeekByte()
{
	if (VBuffer.GetAvailable() == 0)
	{
		if ( ReadNextChunk() == 0 ) // 0 bytes readed
		{
			return -1;
		}
	}
	return VBuffer.PeekByteNoCheck();
}

void TBufferedStream::WriteByte( byte value )
{
	if (VBuffer.GetAvailable() == 0)
	{
		Flush();
	}
	VBuffer.AddByteNoCheck(value);
}

