#pragma once
#include "../Param/Param.h"
#include "../Param/TempParam.h"
#include "../PreProcess/PreProc.h"

class Inspect
{
private:
	Param*					_pParam;		// �˻� �Ķ���� (�ּҸ� ����)
	TempParam::TEMP_PARAM*	_pTmpParam;		// �˻� �ӽ� �Ķ���� (�ּҸ� ����)

	FlatImage*				_pFlatImg;		// �̹��� ��Ȱȭ Ŭ����
	Profile*				_pProfile;		// �̹��� �������� Ŭ����
	FindEdge*				_pEdgeFinder;	// ���� ���� Ŭ����	

public:
	Inspect();
	virtual ~Inspect();

	void Init();

	void SetParam(Param* pParam) { _pParam = pParam; }
	void SetTempParam(TempParam::TEMP_PARAM* pTmpParam) { _pTmpParam = pTmpParam; }

	virtual void Create();
	virtual void Release();

	virtual void Run();


	void SpotInspect();
};

