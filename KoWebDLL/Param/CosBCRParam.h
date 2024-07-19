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

	void to_json(json& j, const Param& p);
	void from_json(json& j, const Param& p);

public:

};