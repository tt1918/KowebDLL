#pragma once
typedef struct _stMarkOptic
{
	int Data[10][10];	// ��ŷ����Ÿ (1 ��ŷ��, 0 ��ŷ����)   [10:Defect����][10:Level]
	int Marking[10][10];	// ���� ��ŷ (1 ��ŷ��, 0 ��ŷ����)    [10:Defect����][10:Level]
}MARK_OPTIC;