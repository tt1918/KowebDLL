#pragma once
#include "../Param/TempParam.h"
#include <vector>

#define MAX_EDGE_COUNT 20

////////////////////////////////////////////////////////////////////////////
// 검사 한 에지 정보를 저장해둔다.
typedef struct _stEdgeInfo
{
	int count;
	int arrEdge[MAX_EDGE_COUNT];
	int foundEdge;


	_stEdgeInfo()
	{
		Reset();
	}

	~_stEdgeInfo()
	{

	}

	void Reset()
	{
		count = 0;
		for (int i = 0; i < MAX_EDGE_COUNT; i++) arrEdge[i] = -1;
		foundEdge = 0;
	}

}EDGE_INFO, *LP_EDGE_INFO;
////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
typedef struct _stFindEdgeResult
{
	int InspType;		// 검사 데이터 성격 규정
	int IsInspOK;		// 검사 결과 정보 : false(실패), true(성공)
	double StX;			// 시작 에지 정보
	double EdX;			// 끝 에지 정보
}FIND_EDGE_RESULT;
#pragma pack(pop)

class FindEdge
{
private:
	int* m_pProfileEdge;
	int m_nWidth;
	int m_nHeight;

	int* m_pTooProfile;
	int  m_nImagePart;
	int  m_nImagePartH;

public:
	int m_nInspX1;
	int m_nInspX2;

	int m_nInspEdgeX1;
	int m_nInspEdgeX2;

	int m_nEdgeDir;

	EDGE_INFO m_stFoundEdge;

public:
	FindEdge();
	~FindEdge();

	void Init(int width, int height);
	void Destroy();
	void SetImagePartH(int nCount, int height) { m_nImagePart = nCount;  m_nImagePartH = height; }

	bool GetEdge(TempParam::INSP_EDGE_PARAM* edgeData, unsigned char* src, int width, int height, double dScaleX, int nEdgeTh, int nAlgorithm, int nOffset, int nEdgeDir, int notInspArea, bool isDualEdge = false);

	// 외부 호출 함수로만 사용. 
	bool InspectEdge(unsigned char* src, int width, int height, int pitch, int depth, double dScaleX, double dScaleY, int nProdCnt, int nAlgorithm, int nEdgeTH, int nEdgeCnt, double nProdSize, double nProdGab, double *cntX, std::vector<FIND_EDGE_RESULT>* res);

private:
	int GetEdge_Sub0(unsigned char* src, int left, int top, int right, int bottom, int pitch, int offset, int thUp, int notInspArea);
	int GetEdge_Sub1(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int notInspArea);
	int GetEdge_Sub2(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int notInspArea);
	int GetEdge_Sub3(unsigned char* src, int pitch, int nThreshold, int notInspArea);
	int GetEdge_Sub4(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int edgeOffset, int notInspArea);
	void GetEdge_Sub9(unsigned char* src, int left, int top, int right, int bottom, int pitch, int thUp, int notInspArea, int* pEdgeL, int* pEdgeR);
	int GetEdge_Sub10(unsigned char* src, int left, int top, int right, int bottom, int pitch, int offset, int thUp, int notInspArea);


	int FindEdgeDetail(int nDir, int *pProfile, int N1, int N2);

	int GetMediumEdge(int nEdge);
	int GetMediumEdgeSub(int count, int *value);
};

