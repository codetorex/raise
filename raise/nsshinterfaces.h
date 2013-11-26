#ifndef NSSHINTERFACES_H
#define NSSHINTERFACES_H

#include "tarray.h"

enum NSSHCipherMode
{
	Encypt = 0,
	Decrypt = 1,
};

class NSSHCipher
{
public:
	NSSHCipherMode Mode;

	ui32 initVectorSize;
	ui32 blockSize;

	void Initialize(NSSHCipherMode mMode, Array<byte>& Key, Array<byte>& InitVector) = 0;
	void Transform(Array<byte>& inputBuffer, int inputOffset, int inputLength, Array<byte>& outputBuffer, int outputOffset) = 0;
};

class NSSHHash
{
public:
	int blockSize;

	void Initialize() = 0;
	void Update(Array<byte>& inputBuffer, int inputOffset, int inputLength) = 0;
	Array<byte> Finalize() = 0;
};

class NSSHKEXCert
{
public:
	bool Verify(Array<byte>& H, Array<byte>& sig_of_H) = 0;
}


class NSSHMAC
{
public:
	int blockSize;
	void Initialize(Array<byte>& Key) = 0;
	void Update(Array<byte>& inputBuffer, int inputOffset, int inputLength) = 0;
	void Update(int input) = 0;
	Array<byte> GetFinal() = 0;
}



#endif