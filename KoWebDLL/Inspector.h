#pragma once

#include "Inspect\Inspect.h"
#include "Param\Param.h"
#include "Param\TempParam.h"

class Inspector
{
public:
	Param *m_pParam;
	TempParam::TEMP_PARAM m_tempParam;

public:
	Inspector();
	~Inspector();

	void Initialize();
};