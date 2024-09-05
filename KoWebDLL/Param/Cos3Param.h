#pragma once
#include "Param.h"

// Á¤¹Ý»ç. 
class Cos3Param : public Param
{
public:


public:
	Cos3Param();
	~Cos3Param();

	void SetParam() override;
	void GetParam() override;
	void Apply() override;

	void SetAddress() override;

public:

};