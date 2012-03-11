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

	void Initialize(NSSHCipherMode mMode, TArray<byte>& Key, TArray<byte>& InitVector) = 0;
	void Transform(TArray<byte>& inputBuffer, int inputOffset, int inputLength, TArray<byte>& outputBuffer, int outputOffset) = 0;
};

class NSSHHash
{
public:
	int blockSize;

	void Initialize() = 0;
	void Update(TArray<byte>& inputBuffer, int inputOffset, int inputLength) = 0;
	TArray<byte> Finalize() = 0;
};

class NSSHKEXCert
{
public:
	bool Verify(TArray<byte>& H, TArray<byte>& sig_of_H) = 0;
}


class NSSHMAC
{
public:
	int blockSize;
	void Initialize(TArray<byte>& Key) = 0;
	void Update(TArray<byte>& inputBuffer, int inputOffset, int inputLength) = 0;
	void Update(int input) = 0;
	TArray<byte> GetFinal() = 0;
}



#endif