//////////////////////////////////////////////////////////////////////////////
//
//  NOTE FOR COPYRIGHT
//	�� �ҽ��ڵ�� ���쿡 ���� ������� ���̹Ƿ� �ٸ� ����� ����� ���մϴ�.
// 
//  FAST CHAIN CODE			                            1996. 7. 29  ���� 
//
/////////////////////////////////////////////////////////////////////////////


#ifndef CHAIN_H
#define CHAIN_H

#define V_HIGH       255  // ����� gray level��
#define V_LOW        0    // ��ü�� gray level�� 
#define CHAIN_PI	 3.141592
#define ROOT2        1.414213562

class  CChain
{
public:
   // Chain class�� ������ : ���Ҽ� �ִ� �ִ� ��ü�� ��(nMaxChsin)�� �� ��ü����(nMaxChainData)�� �����Ѵ�.
   CChain(int nMaxChain, long nMaxChainData);
   ~CChain();

   // ����Ÿ�� �����Ѵ�. FastChain�� �θ������� �ݵ�� �̸� Call����� �Ѵ�
   // argument�� �Ʒ� ������� ���� 
   void SetChainData(int object, unsigned char* fm, int skipx, int skipy, int minboundary,
				     long maxboundary, int width, int height);

   // ChainData�� ���Ѵ�.
   int  FastChain(int x1, int y1, int x2, int y2);
   
   
   // Area���ϴ� �Լ� ( N : chain ��ȣ ) , ©���� ������ �κ� ����
   double Chain_Area( int N);
   // Area���ϴ� �Լ� ( N : chain ��ȣ ) , ©���� ������ �κ� �������� ���� 
   double Chain_LogicalArea( int N);
  
   // Length���ϴ� �Լ� ( N : chain ��ȣ )
   double Chain_Length( int N);

   // Center ���ϴ� �Լ� ; Chain ����Ÿ �� ��赥��Ÿ������ ���ϴ� �߽�( N : chain ��ȣ )
   int  FindCenterWithBoundary(int N, double *cx, double *cy);

   // Center���ϴ� �Լ� (Area�� �����߽�)
   // N    : Chain ��ȣ 
   // Return�� : �߽� ( *cx, *cy ) 
   int    Chain_Center( int N, double *cx, double *cy);
   
   // Principal Angle�� ���ϴ� �Լ��̴�.
   double FindAngle(int N);
   
   // �� Chain�� ����Ÿ���� �� return�Ѵ�.
   int  GetChainDataNumber(int N)     {return m_chain[N]; };

   // �� Chain����Ÿ�� ���۹����� return�Ѵ�.
   int *GetChainData(int N) {return &m_chaindata[m_chainstart[N]]; };

   // Moment�� ���ؼ� ����
   double FindMoment(int N);
   double FindMoment(int N, double cx, double cy );

   // Circleness�� ���ؼ� ����
   double FindCompactness(int N);

   int  FindMaxX(int N); // ��赥��Ÿ�� ���� ū x��ǥ�� ���ؼ� ����
   int  FindMinX(int N); //                  ���� x 
   int  FindMaxY(int N); //                   ū y
   int  FindMinY(int N); //                  ���� y

   //�� ������ ����հŸ�, �����Ÿ�, ��հŸ����ϴ� �Լ� 
   int FineDistFromPoint(int N, double cx,double cy, double *min, double *max, double *avg);

   //������ ü���� YValue���� ���� ü���� �¿� x��ǥ 
   int GetLeft_RightBoundary(int N, int YValue, int *left, int *right);
   
	//Boundary Pixel Line Pattern Data ���� 
   int GetNormalizedLinePattern(int N, int *dist);
   //Line Pattern Data�� Pattern MAtching (return 0~1.0)
   double FindCorrelation(int *fmT, int *fmS, int nLineLength);

   //��ü�� ���� �ּ� �簢�� �׸���.
   int FindMinRect(int N, double Cx, double Cy, double angle,  double pos[][2]);

   int  m_checkInit;

protected:
   // ���������� Call�Ǵ� �Լ� (�Ű澵�� ����)
   int  FindBoundaryPixel(int startx, int starty, int *data, long emptyarray);
   int FindCrossPoint(double t1, double b1, double c1, double t2, double b2, double c2, double *cx, double *cy);

protected:

   
   unsigned char	OBJECT;         // ��ü�� ��Ⱚ  LOW  �ƴϸ� HIGH
   unsigned char	BACKGROUND;     // ����� ��Ⱚ  HIGT �ƴϸ� LOW
   unsigned char	BOUNDARY;       // ��� �ȼ��� �̰����� �����Ѵ�.
   int				MAXCHAIN;       // ���Ҽ� �ִ� Chain�� �� ( ��ü�� �� LIMIT)
   long				MAXCHAINDATA;   // ��ü Chain�� ���� (��ü�� ��ü ������ ����Ÿ LIMIT)
   int				m_chainCount;   // ���� Chain�� �� (��ü�� ��)
   int*				m_chain;		// �� Chain�� ����( [1]=N1, [2]=N2, [3]=N3....) 
   int*				m_chaindata;	// Chain Data ( x0��ǥ, y0��ǥ, x1��ǥ, y1��ǥ, ...) 
   long*			m_chainstart;	// ���κ��� (�迭�� Index������ ��� Chain �� total���̸� ����)
   unsigned char*	m_fm;			// ������ ���۹���
   int				m_skipx;        // ��ĵ�Ҷ� Skip�ϴ� �ȼ� �� (x����)
   int				m_skipy;        // ��ĵ�Ҷ� Skip�ϴ� �ȼ� �� (y����)
   int				m_minboundary;  // Chain���̰� �̰� �̻��϶��� ���Ѵ�.
   long				m_maxboundary;  // Chain���̰� �̰� �����϶��� ���Ѵ�.
   int				WIDTH_MEM;      // ������ x�� ��
   int				imageheight;	// ������ y�� ��
   bool				FLAG_SetData;   // ���������� ��� ( ���� Data�� ���õǾ������� TRUE)
};

#endif