#include "stdafx.h"
#include "tbufferedstream.h"


void TBufferedStream::Flush()
{
	if (WriteBuffer )
	{
		if (VBuffer.Index > 0)
		{
			Source->Write(VBuffer.Data,1, VBuffer.Index);
			VBuffer.VirtualRewind();
			Source->Flush();
		}
	}
	else
	{
		dword RemainingSize = VBuffer.GetAvailable(); // data length in buffer
		if (RemainingSize > 0)
		{
			Source->Seek(VBuffer.VirtualEnd - RemainingSize,sBegin);
			VBuffer.VirtualRewind();
		}
	}
}

int TBufferedStream::Read( void* buffer,int size,int count )
{
	return Source->Read(buffer,size,count);
	// TODO: fix this.

	/*dword TotalSize = size * count;

	if (WriteBuffer) // Its uncommon to switch from writing to reading
	{
		Flush();
	}
	WriteBuffer = false;

	dword RemainingSize = VBuffer.GetAvailable(); // data length in buffer

	if (RemainingSize > 0 || VBuffer.Length > 0)
	{
		if (RemainingSize >= TotalSize) // if our data is bigger than requested
		{
			VBuffer.GetBytes((byte*)buffer,TotalSize);
			return TotalSize;
		}
		else
		{
			VBuffer.GetBytes((byte*)buffer,RemainingSize);
			int ReadSize = Source->Read((byte*)buffer+RemainingSize,1,TotalSize - RemainingSize);
			return ReadSize + RemainingSize;
		}
	}

	assert(RemainingSize >= 0);

	if (RemainingSize == 0 || VBuffer.Length == 0)
	{
		if (TotalSize >= VBuffer.Capacity)
		{
			int ReadSize = Source->Read((byte*)buffer,1,TotalSize);
			VBuffer.VirtualRewind();
			return ReadSize;
		}

		dword CurRealPos = Source->Position();
		int ReadSize = Source->Read(VBuffer.Data,1,VBuffer.Capacity);
		VBuffer.SetVirtual(CurRealPos,CurRealPos + ReadSize);
		if (ReadSize == 0) return 0;

		if (ReadSize != VBuffer.Capacity) // end of file
		{
			return Read(buffer,size,count);
		}
	}

	VBuffer.GetBytes((byte*)buffer,TotalSize);
	return TotalSize;*/
}

void TBufferedStream::Write( void* buffer,int size,int count )
{
	dword TotalSize = size * count;
	dword RemainingSize = VBuffer.GetAvailable(); // data length in buffer
	if ( !WriteBuffer && VBuffer.Length > 0 && RemainingSize > 0) // OMG THERE IS READ DATA INSIDE!
	{
		throw Exception("There is still data inside read buffer, try flushing it");
	}
	WriteBuffer = true;

	if (TotalSize > VBuffer.Capacity)
	{
		if (VBuffer.Index > 0)
			Flush(); // if there is still data inside buffer flush it

		Source->Write(buffer,size,count);
	}

	if ((dword)VBuffer.GetAvailable() < TotalSize)
	{
		Flush(); // if there is still data inside buffer flush it
	}

	VBuffer.AddBytes((byte*)buffer,TotalSize);
}

int TBufferedStream::ReadByte()
{
	byte r;
	int sz = Read(&r,1,1);
	if (sz == 0)
	{
		return -1;
	}
	return r;
	// TODO: fix this

	/*assert( VBuffer.Index <= VBuffer.Capacity );

	if (VBuffer.Index == VBuffer.Capacity)
	{
		VBuffer.VirtualRewind();
	}

	if(VBuffer.Length == 0)
	{
		dword CurRealPos = Source->Position();
		int ReadSize = Source->Read(VBuffer.Data,1,VBuffer.Capacity);
		if (ReadSize > 0)
		{
			VBuffer.SetVirtual(CurRealPos,CurRealPos + ReadSize);
		}
		else
		{
			throw Exception("End of stream");
		}
	}

	if (VBuffer.Index + VBuffer.VirtualStart == VBuffer.Length)
	{
		throw Exception("End of stream");
	}

	return VBuffer.GetByte();*/
}

void TBufferedStream::Seek( dword offset,SeekOrigin origin )
{
	switch( origin )
	{
	case sBegin:
		if (VBuffer.VirtualStart <= offset && VBuffer.VirtualEnd >= offset) // if offset is inside buffered area
		{
			VBuffer.SetPosition(offset - VBuffer.VirtualStart);
			return;
		}
		break;

	case sCurrent:
		int available = VBuffer.GetAvailable();
		if (available > 0)
		{
			if (offset <= (dword)available)
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

int TBufferedStream::Peek()
{
	int r = ReadByte();
	Source->Seek( Source->Position() -1 , sBegin);
	// TODO: fix this shit.

	/*if (r != -1)
	{
	VBuffer.Advance(-1);
	}*/
	return r;
}