#pragma once
#include "Param.h"

// 
class CBCRParam : public Param
{
public:
	InspParam::BCR_PARAM Bcr;

public:
	CBCRParam();
	~CBCRParam();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

public:

};