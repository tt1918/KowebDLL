#pragma once
#include "Param.h"

// Á¤¹Ý»ç. 
class Cos3Param : Param
{
public:


public:
	Cos3Param();
	~Cos3Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

	void to_json(json& j, const Param& p);
	void from_json(json& j, const Param& p);

public:

};