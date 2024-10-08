//////////////////////////////////////////////////////////////////////////////
//
//  NOTE FOR COPYRIGHT
//	이 소스코드는 고영우에 의해 만들어진 것이므로 다른 사람의 사용을 금합니다.
// 
//  FAST CHAIN CODE			                            1996. 7. 29  고영우 
//
/////////////////////////////////////////////////////////////////////////////


#ifndef CHAIN_H
#define CHAIN_H

#define V_HIGH       255  // 배경의 gray level값
#define V_LOW        0    // 물체의 gray level값 
#define CHAIN_PI	 3.141592
#define ROOT2        1.414213562

class  CChain
{
public:
   // Chain class의 생성자 : 구할수 있는 최대 물체의 수(nMaxChsin)와 그 전체길이(nMaxChainData)를 설정한다.
   CChain(int nMaxChain, long nMaxChainData);
   ~CChain();

   // 데이타를 세팅한다. FastChain을 부르기전에 반드시 이를 Call해줘야 한다
   // argument는 아래 멤버변수 참조 
   void SetChainData(int object, unsigned char* fm, int skipx, int skipy, int minboundary,
				     long maxboundary, int width, int height);

   // ChainData를 구한다.
   int  FastChain(int x1, int y1, int x2, int y2);
   
   
   // Area구하는 함수 ( N : chain 번호 ) , 짤려져 나가는 부분 보상
   double Chain_Area( int N);
   // Area구하는 함수 ( N : chain 번호 ) , 짤려져 나가는 부분 보상하지 않음 
   double Chain_LogicalArea( int N);
  
   // Length구하는 함수 ( N : chain 번호 )
   double Chain_Length( int N);

   // Center 구하는 함수 ; Chain 데이타 즉 경계데이타만으로 구하는 중심( N : chain 번호 )
   int  FindCenterWithBoundary(int N, double *cx, double *cy);

   // Center구하는 함수 (Area의 무게중심)
   // N    : Chain 번호 
   // Return값 : 중심 ( *cx, *cy ) 
   int    Chain_Center( int N, double *cx, double *cy);
   
   // Principal Angle을 구하는 함수이다.
   double FindAngle(int N);
   
   // 각 Chain의 데이타수를 를 return한다.
   int  GetChainDataNumber(int N)     {return m_chain[N]; };

   // 각 Chain데이타의 시작번지를 return한다.
   int *GetChainData(int N) {return &m_chaindata[m_chainstart[N]]; };

   // Moment를 구해서 리턴
   double FindMoment(int N);
   double FindMoment(int N, double cx, double cy );

   // Circleness를 구해서 리턴
   double FindCompactness(int N);

   int  FindMaxX(int N); // 경계데이타중 가장 큰 x좌표를 구해서 리턴
   int  FindMinX(int N); //                  작은 x 
   int  FindMaxY(int N); //                   큰 y
   int  FindMinY(int N); //                  작은 y

   //한 점에서 가장먼거리, 가까운거리, 평균거리구하는 함수 
   int FineDistFromPoint(int N, double cx,double cy, double *min, double *max, double *avg);

   //임의의 체인중 YValue값을 갖는 체인의 좌우 x좌표 
   int GetLeft_RightBoundary(int N, int YValue, int *left, int *right);
   
	//Boundary Pixel Line Pattern Data 구성 
   int GetNormalizedLinePattern(int N, int *dist);
   //Line Pattern Data간 Pattern MAtching (return 0~1.0)
   double FindCorrelation(int *fmT, int *fmS, int nLineLength);

   //물체에 접한 최소 사각형 그린다.
   int FindMinRect(int N, double Cx, double Cy, double angle,  double pos[][2]);

   int  m_checkInit;

protected:
   // 내부적으로 Call되는 함수 (신경쓸것 없음)
   int  FindBoundaryPixel(int startx, int starty, int *data, long emptyarray);
   int FindCrossPoint(double t1, double b1, double c1, double t2, double b2, double c2, double *cx, double *cy);

protected:

   
   unsigned char	OBJECT;         // 물체의 밝기값  LOW  아니면 HIGH
   unsigned char	BACKGROUND;     // 배경의 밝기값  HIGT 아니면 LOW
   unsigned char	BOUNDARY;       // 경계 픽셀을 이값으로 세팅한다.
   int				MAXCHAIN;       // 구할수 있는 Chain의 수 ( 물체의 수 LIMIT)
   long				MAXCHAINDATA;   // 전체 Chain의 길이 (물체의 전체 경계길이 데이타 LIMIT)
   int				m_chainCount;   // 구한 Chain의 수 (물체의 수)
   int*				m_chain;		// 각 Chain의 길이( [1]=N1, [2]=N2, [3]=N3....) 
   int*				m_chaindata;	// Chain Data ( x0좌표, y0좌표, x1좌표, y1좌표, ...) 
   long*			m_chainstart;	// 내부변수 (배열의 Index까지의 모든 Chain 의 total길이를 저장)
   unsigned char*	m_fm;			// 영상의 시작번지
   int				m_skipx;        // 스캔할때 Skip하는 픽셀 수 (x방향)
   int				m_skipy;        // 스캔할때 Skip하는 픽셀 수 (y방향)
   int				m_minboundary;  // Chain길이가 이값 이상일때만 구한다.
   long				m_maxboundary;  // Chain길이가 이값 이하일때만 구한다.
   int				WIDTH_MEM;      // 영상의 x쪽 폭
   int				imageheight;	// 영상의 y쪽 폭
   bool				FLAG_SetData;   // 내부적으로 사용 ( 위의 Data가 세팅되었을때만 TRUE)
};

#endif