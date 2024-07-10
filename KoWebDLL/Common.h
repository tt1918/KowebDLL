#pragma once
namespace Common
{
	typedef struct _stPointI
	{
		int X;
		int Y;
	}POINT_I, *LPPOINT_I;

	typedef struct _stPointD
	{
		double X;
		double Y;
	}POINT_D, *LPPOINT_D;

	typedef struct _stPointF
	{
		float X;
		float Y;
	}POINT_F, *LPPOINT_F;

	typedef struct _stRectI
	{
		int top;
		int left;
		int bottom;
		int right;
	}RECT_I, *LPRECT_I;
}