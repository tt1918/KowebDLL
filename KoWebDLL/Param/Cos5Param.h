#pragma once
#include "Param.h"

// �̼����� ���� �˻縦 ����
class Cos5Param : Param
{
public:
	InspParam::GROUP_PARAM		Group;			// ���� ���� �Ķ����

public:
	Cos5Param();
	~Cos5Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;
public:

};