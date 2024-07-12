#pragma once
typedef struct _stMarkOptic
{
	int Data[10][10];	// 마킹데이타 (1 마킹함, 0 마킹안함)   [10:Defect종류][10:Level]
	int Marking[10][10];	// 최종 마킹 (1 마킹함, 0 마킹안함)    [10:Defect종류][10:Level]
}MARK_OPTIC;