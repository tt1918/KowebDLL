#pragma once
#include "Param.h"

// 
class CosBCRParam : Param
{
public:
	InspParam::BCR_PARAM Bcr;

public:
	CosBCRParam();
	~CosBCRParam();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;
public:

};