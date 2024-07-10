#pragma once
#include <memory.h>

typedef struct _stArrSetI
{
	int*	Val;
	int		Size;

	_stArrSetI()
	{
		Val = nullptr;
		Size = 0;
	}

	~_stArrSetI()
	{
		Clear();
	}

	void Init(int size)
	{
		if (Val != nullptr && size == Size)
		{
			memset(Val, 0x00, sizeof(int) * Size);
			return;
		}

		Clear();

		Size = size;
		Val = new int[Size];
		memset(Val, 0x00, sizeof(int) * Size);
	}

	void Clear()
	{
		if (Val != nullptr)
		{
			delete[] Val;
			Val = nullptr;
		}
	}

}ARR_SET_I;

class FlatImage
{
private:
	ARR_SET_I ProjFactor;
	int		  *m_pArea;
public:
	FlatImage();
	~FlatImage();

	void MakeFlatLineScan(unsigned char* src, unsigned char* dst, int* profile, int flatBrt, int left, int width, int height, int pitch, int avg, int inspX1, int inspX2);
	void MakeFlatArea320(unsigned char* src, unsigned char* dst, int nBaseValue, int left, int width, int height, int pitch);
};
