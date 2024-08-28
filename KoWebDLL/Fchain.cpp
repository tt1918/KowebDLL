//////////////////////////////////////////////////////////////////////////////////////////////
//
//	Copyright (C) 1995    Ko, Young Woo  All Rights Reserverd
//
//
//////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "fchain.h"
#include <math.h>


CChain::CChain(int nMaxChain, long nMaxChainData )
{
    m_chain     = new int[nMaxChain]; // �ִ�� ���� Chain �� 
	m_chainstart= new long[nMaxChain];  // 
	m_chaindata = new int[2*nMaxChainData];  // ���� Chain�� ������ ���� MAXCHAINDATA�� ������ ����.
    MAXCHAIN     = nMaxChain;
	MAXCHAINDATA = nMaxChainData;
	FLAG_SetData = FALSE;

	if(m_chain!=NULL && m_chainstart!=NULL && m_chaindata!=NULL) m_checkInit=0;
	else														 m_checkInit=1;
}

CChain::~CChain()
{
    delete[] m_chain;
	delete[] m_chainstart;
	delete[] m_chaindata;
}

void CChain::SetChainData(int Object, unsigned char* fm, int skipx, int skipy, int minboundary,
						  long maxboundary, int width, int height)
{
	 if(Object==0) { OBJECT=V_LOW  ; BACKGROUND = V_HIGH; BOUNDARY=V_LOW+1; }
	 else          { OBJECT=V_HIGH ; BACKGROUND = V_LOW ; BOUNDARY=V_HIGH-1;}
	 m_fm          = fm;          // �޸𸮵���Ÿ�� ���۹��� 
     m_skipx	   = skipx;       // x���� skip �ȼ��� 
     m_skipy	   = skipy;       // y���� skip �ȼ��� 
	 m_minboundary = minboundary; // ������ chain�� ���� (�̰������� chain �� ����)
     m_maxboundary = maxboundary; // ������ chain�� ���� (�̰��̻��� chain �� ����)
	 WIDTH_MEM     = width;       // ������ x ���� ��
	 imageheight  = height;       // ������ y ���� ��
	 FLAG_SetData  = TRUE;
}

int CChain::FastChain(int x1, int y1, int x2, int y2)
{
	 int i,j,k;
	 int dum;
	 int blackpoint, totaldatanumber=0;
          
     if(!FLAG_SetData) return -1; // Data Setting�� �ȵǾ����� -1�� return �Ѵ�. 

	 if(x1>x2) { dum=x1; x1=x2; x2=dum ; }
	 if(y1>y2) { dum=y1; y1=y2; y2=dum ; }
	 if(x1<0 || y1 <0 ) return -2;
	 if(x2>=WIDTH_MEM)    x2=WIDTH_MEM-1;
	 if(y2>=imageheight) y2=imageheight-1;

	 // initialize 
	 if(m_skipx<1) m_skipx=1;              // Skip�ϴ� �ȼ��� 2���� ������ ����.
	 if(m_skipy<1) m_skipy=1;              //
	 if(m_minboundary<1) m_minboundary=1;  // ���� chain�� ���̰� 5���� ������ ����.
	 
	 m_chainCount=0;

     //BEGIN-0 : ������ Line�� ���� ������� ä���. >>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 for(i=x1;i<=x2;i++)                   
	 {    
		  *(m_fm+WIDTH_MEM*y1+i)=BACKGROUND;
		  *(m_fm+WIDTH_MEM*y2+i)=BACKGROUND;  
	 }
	 for(i=y1;i<=y2;i++)
	 {  
		  *(m_fm+WIDTH_MEM*i+x1)=BACKGROUND;
		  *(m_fm+WIDTH_MEM*i+x2)=BACKGROUND; 
	 }
	 //END-0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	 //scan and find boundary pixel 
	 for(i=y1; i<=y2 ;i+=m_skipy)
		 for(j=x1; j<=x2 ;j+=m_skipx)
			 if(*(m_fm+i*WIDTH_MEM+j)==OBJECT) 
			 {
				 blackpoint=0;
				 for(k=j;k>=(j-m_skipx);k--)
					if(*(m_fm+i*WIDTH_MEM+k) == BACKGROUND )
					{   blackpoint=k+1;  break;   } // (blackpoint,i) is boundary pixel 
				 
				 // if this object didnt be checked yet
				 if( *(m_fm+i*WIDTH_MEM+blackpoint)!=BOUNDARY && blackpoint!=0)
				 {
					 m_chain[m_chainCount]=FindBoundaryPixel(blackpoint,i,
								&m_chaindata[2*totaldatanumber], MAXCHAINDATA-totaldatanumber);
					 if(m_chain[m_chainCount]>=m_minboundary)
					 {
						  totaldatanumber+=m_chain[m_chainCount];
						  m_chainCount++;
					 }
					 
					 //  if totaldatanumber is greater than MAXCHAINDATA or   
					 //  if m_chainCount is greater than MAXCHAIN--> STOP finding 
					 else if(m_chain[m_chainCount]==-1) goto END;
					 if(m_chainCount>=MAXCHAIN) goto END;
				 }
			 }
	 
END: totaldatanumber=0;
	 for(i=0;i<m_chainCount;i++)
     {
		 m_chainstart[i]=2*totaldatanumber;
         totaldatanumber+=m_chain[i];
	 }

	 return m_chainCount;

}
int  CChain::FindBoundaryPixel(int startx, int starty,int *data, long emptyarray)
{
	  long count=0;
	  int cx,cy;
	  int bcx,bcy;
	  LPBYTE cfm;
	  int SP;   // start point 

	  if(emptyarray<0) return -1;

	  cx=bcx=startx; cy=bcy=starty;
	  data[0]=cx; data[1]=cy; count++; SP=3;
	  cfm=m_fm+cy*WIDTH_MEM+cx;

	  do
	  {
			switch(SP)
			{
				case 1: 
					if( *(cfm-WIDTH_MEM-1)==OBJECT ||*(cfm-WIDTH_MEM-1)==BOUNDARY)
					{
						cx=cx-1; cy=cy-1;
						SP=7;
						break;
					}
				case 2: 
					if( *(cfm-1)==OBJECT || *(cfm-1)==BOUNDARY)
					{
						cx=cx-1; cy=cy;
						SP=1;
						break;
					}
				case 3: 
					if( *(cfm+WIDTH_MEM-1)==OBJECT || *(cfm+WIDTH_MEM-1)==BOUNDARY)
					{
						cx=cx-1; cy=cy+1;
						SP=1;
						break;
					}
				case 4: 
					if( *(cfm+WIDTH_MEM)==OBJECT || *(cfm+WIDTH_MEM)==BOUNDARY)
					{
						cx=cx; cy=cy+1;
						SP=3;
						break;
					}
				case 5: 
					if( *(cfm+WIDTH_MEM+1)==OBJECT || *(cfm+WIDTH_MEM+1)==BOUNDARY)
					{
						cx=cx+1; cy=cy+1;
						SP=3;
						break;
					}
				case 6: 
					if( *(cfm+1)==OBJECT || *(cfm+1)==BOUNDARY)
					{
						cx=cx+1; cy=cy;
						SP=5;
						break;
					}
				case 7: 
					if( *(cfm-WIDTH_MEM+1)==OBJECT || *(cfm-WIDTH_MEM+1)==BOUNDARY)
					{
						cx=cx+1; cy=cy-1;
						SP=5;
						break;
					}
				case 8:
					if( *(cfm-WIDTH_MEM)==OBJECT || *(cfm-WIDTH_MEM)==BOUNDARY)
					{
						cx=cx; cy=cy-1;
						SP=7;
						break;
					}

				default:
					if( *(cfm-WIDTH_MEM-1)==OBJECT || *(cfm-WIDTH_MEM-1)==BOUNDARY)
					{
						cx=cx-1; cy=cy-1;
						SP=7;
					}
					else if( *(cfm-1)==OBJECT || *(cfm-1)==BOUNDARY)
					{
						cx=cx-1; cy=cy;
						SP=1;
					}
					else if( *(cfm+WIDTH_MEM-1)==OBJECT || *(cfm+WIDTH_MEM-1)==BOUNDARY)
					{
						cx=cx-1; cy=cy+1;
						SP=1;
					}
					else if( *(cfm+WIDTH_MEM)==OBJECT || *(cfm+WIDTH_MEM)==BOUNDARY)
					{
						cx=cx; cy=cy+1;
						SP=3;
					}
					else if( *(cfm+WIDTH_MEM+1)==OBJECT || *(cfm+WIDTH_MEM+1)==BOUNDARY)
					{
						cx=cx+1; cy=cy+1;
						SP=3;
					}
					break;
			} 
			if(count >= emptyarray)		return -1;		// �� ������ �ִ� �޸𸮸� �ʰ��ϸ� -1�� ������
			data[2*count]  = cx;
			data[2*count+1]= cy;
			count++;
			cfm=m_fm+cy*WIDTH_MEM+cx;
			*(cfm)=BOUNDARY;
		}while( cx!=startx || cy!=starty);
		
	  if(--count<m_minboundary)		return 0;		// ���� boundary�� m_minboundary���� �۰ų� 
		else if(count>m_maxboundary)	return 0;		// m_maxboundary���� ũ�� 0�� �����ش�.
		else							return count;
}

// Find Center with only boundary pixels ---------------------------------
int CChain::FindCenterWithBoundary(int N, double *cx, double *cy)
{
	 int i;
	 double sumx,sumy;
	 int Number;
     int *BPixel;      

	 Number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 sumx=sumy=0;
	 for(i=0;i<Number;i++)
	 {
			  sumx+= BPixel[2*i];;
			  sumy+= BPixel[2*i+1];
	 }

	 *cx = sumx / Number;
	 *cy = sumy / Number;

	 return 0;
}


//  Boundary �ٱ��� ©���� �κ��� �����ϱ� ���� ���̺�
BYTE L[9][9]={ {0,0,0,0,0,0,0,0,0},
			   {0,4,5,6,7,8,0,2,3},{0,3,4,5,6,7,8,0,0},{0,2,3,4,5,6,7,8,0},
			   {0,0,0,3,4,5,6,7,8},{0,8,0,2,3,4,5,6,7},{0,7,8,0,0,3,4,5,6},
			   {0,6,7,8,0,2,3,4,5},{0,5,6,7,8,0,0,3,4}};

double CChain::Chain_Area( int N)
{
 	 int FirstValue=0,PrevValue=0, CurrValue;
	 double  Area=0, ExceptedArea=0;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
	 if(number<2) return 1.0;
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 // 1�� �ȼ��� ������ �ȼ��� ���ؼ� 1�� �ȼ��� �˻��� �� ���� ����
	 if(BPixel[0] > BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=5; Area+=BPixel[1]-0.5;}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=7; Area+=BPixel[1]+0.5;}
		 else                                 { PrevValue=6; Area+=BPixel[1];}
	 }
	 else if(BPixel[0] < BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=3; Area-=BPixel[1]-0.5;}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=1; Area-=BPixel[1]+0.5;}
		 else                                 { PrevValue=2; Area-=BPixel[1];}
	 }
	 else
	 {
		 if(BPixel[1]>BPixel[2*number-1])      PrevValue=4;
		 else if(BPixel[1]<BPixel[2*number-1]) PrevValue=8;
		 else   
		 {
			 return 1.0;
		 }
	 }

	 FirstValue=PrevValue; // 1�� �ȼ��� �˻����� FirstValue�� �̸� ����

	 // 2�� �ȼ����� ������ �ȼ����� �˻����� ���� ����
	 for(int i=1;i<number;i++)
	 {
		if(BPixel[2*i] > BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=5; Area+=BPixel[2*i+1]-0.5;}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=7; Area+=BPixel[2*i+1]+0.5;}
		  else                                 { CurrValue=6; Area+=BPixel[2*i+1];}
		}
		else if(BPixel[2*i] < BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=3; Area-=BPixel[2*i+1]-0.5;}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=1; Area-=BPixel[2*i+1]+0.5;}
		  else                                 { CurrValue=2; Area-=BPixel[2*i+1];}
		}
		else
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      CurrValue=4;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) CurrValue=8;
		  else   
		  {
			  return 1.0;
		  }
		}

		// ©���� ������ ���⼭ ��������
		ExceptedArea+=L[PrevValue][CurrValue];   // 1�� ���� number-1 ����.

		PrevValue=CurrValue;		

	 }

	 ExceptedArea+=L[PrevValue][FirstValue]; // number ° ���� ����    //
	 ExceptedArea = ExceptedArea/8.0;								   //

	 Area+= ExceptedArea;											   //

	 return Area;
}

double CChain::Chain_LogicalArea( int N)
{
	 double  Area=0;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
	 if(number<2) return 1.0;
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 // 1�� �ȼ��� ������ �ȼ��� ���ؼ� 1�� �ȼ��� �˻��� �� ���� ����
	 if(BPixel[0] > BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     Area+=BPixel[1]-0.5;
		 else if(BPixel[1]<BPixel[2*number-1])Area+=BPixel[1]+0.5;
		 else                                 Area+=BPixel[1];
	 }
	 else if(BPixel[0] < BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     Area-=BPixel[1]-0.5;
		 else if(BPixel[1]<BPixel[2*number-1])Area-=BPixel[1]+0.5;
		 else                                 Area-=BPixel[1];
	 }
	 
	 // 2�� �ȼ����� ������ �ȼ����� �˻����� ���� ����
	 for(int i=1;i<number;i++)
	 {
		if(BPixel[2*i] > BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      Area+=BPixel[2*i+1]-0.5;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) Area+=BPixel[2*i+1]+0.5;
		  else                                 Area+=BPixel[2*i+1];
		}
		else if(BPixel[2*i] < BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      Area-=BPixel[2*i+1]-0.5;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) Area-=BPixel[2*i+1]+0.5;
		  else                                 Area-=BPixel[2*i+1];
		}
	
	 }

	 return Area;
}

// Chain�� ���̸� ���Ѵ�. �밢���� �����̴� 1.414�� ����Ͽ���.

double CChain::Chain_Length( int N)
{
	 double dLength=0;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
	 if(number<2) return 1.0;
     BPixel = &m_chaindata[m_chainstart[N]]; 

     for(int i=0 ;i<number-1;i++)
       if(BPixel[2*i]!=BPixel[2*i+2] && BPixel[2*i+1]!=BPixel[2*i+3])
	         dLength+=ROOT2;
	   else  dLength+=1;
     
     if(BPixel[0]!=BPixel[2*number-2] && BPixel[1]!=BPixel[2*number-1])
	       dLength+=ROOT2;
	 else  dLength+=1;

	 return dLength;
}

int CChain::Chain_Center( int N, double *cx, double *cy)
{
	 int i,FirstValue=0,PrevValue=0, CurrValue;
	 double XSum,YSum,LineSum,Area;
     double x,y, xExcept, yExcept;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 
	 *cx=BPixel[0];
	 *cy=BPixel[1];
	 if(number==1) return 0;
	
//BEGIN-0 : x�� ���Ϳ� Area�� ���Ѵ�.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	 LineSum=0;
	 XSum=0;
	 Area =0;
	 x=(BPixel[0] + BPixel[2*number-2])/2.0;
	 if(BPixel[0] > BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=5; LineSum=(BPixel[1]-0.5);}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=7; LineSum=(BPixel[1]+0.5);}
		 else                                 { PrevValue=6; LineSum=(BPixel[1]);}
	 }
	 else if(BPixel[0] < BPixel[2*number-2])
	 {
		 if(BPixel[1]>BPixel[2*number-1])     { PrevValue=3; LineSum=-(BPixel[1]-0.5);}
		 else if(BPixel[1]<BPixel[2*number-1]){ PrevValue=1; LineSum=-(BPixel[1]+0.5);}
		 else                                 { PrevValue=2; LineSum=-(BPixel[1]);}
	 }
	 else
	 {
		 if(BPixel[1]>BPixel[2*number-1])      PrevValue=4;
		 else if(BPixel[1]<BPixel[2*number-1]) PrevValue=8;
		 else   
		 {
			  return 0;
		 }		 
	 }

	 Area=LineSum;
	 FirstValue=PrevValue; // 1�� �ȼ��� �˻����� FirstValue�� �̸� ����
	 XSum=x*LineSum;
	 
	 for(i=1;i<number;i++)
	 {
        x=(BPixel[2*i] + BPixel[2*i-2])/2.0;
		LineSum=0;
		if(BPixel[2*i] > BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=5; LineSum=(BPixel[2*i+1]-0.5);}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=7; LineSum=(BPixel[2*i+1]+0.5);}
		  else                                 { CurrValue=6; LineSum=(BPixel[2*i+1]);}
		}
		else if(BPixel[2*i] < BPixel[2*i-2])
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      { CurrValue=3; LineSum=-(BPixel[2*i+1]-0.5);}
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) { CurrValue=1; LineSum=-(BPixel[2*i+1]+0.5);}
		  else                                 { CurrValue=2; LineSum=-(BPixel[2*i+1]);}
		}
		else
		{
		  if(BPixel[2*i+1]>BPixel[2*i-1])      CurrValue=4;
		  else if(BPixel[2*i+1]<BPixel[2*i-1]) CurrValue=8;
		  else   
		  {
			  return 0;
		  }
		}
 
		Area+=LineSum;
		XSum+=x*LineSum;

		// ©���� ������ ���⼭ ��������
		Area+=L[PrevValue][CurrValue]/8.0;
		xExcept=BPixel[2*i-2];
		XSum+=xExcept*L[PrevValue][CurrValue]/8.0;   // 1�� ���� number-1 ����.

		PrevValue=CurrValue;		

	 }

     Area+=L[PrevValue][FirstValue]/8.0;  // number ° ���� ����  
	 xExcept=BPixel[2*number-2];						  
	 XSum+=xExcept*L[PrevValue][FirstValue]/8.0; // number ° ���� ����     

     *cx=XSum/Area;
//END-0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//BEGIN-0 : Y�� ���͸� ���Ѵ�. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 
     YSum=0;
	 y=(BPixel[1] + BPixel[2*number-1])/2.0;
	 if(BPixel[1] > BPixel[2*number-1])
	 {
		 if(BPixel[0]>BPixel[2*number-2])     { PrevValue=5; YSum-=y*(BPixel[0]-0.5);}
		 else if(BPixel[0]<BPixel[2*number-2]){ PrevValue=3; YSum-=y*(BPixel[0]+0.5);}
		 else                                 { PrevValue=4; YSum-=y*(BPixel[0]);}
	 }
	 else if(BPixel[1] < BPixel[2*number-1])
	 {
		 if(BPixel[0]>BPixel[2*number-2])     { PrevValue=7; YSum+=y*(BPixel[0]-0.5);}
		 else if(BPixel[0]<BPixel[2*number-2]){ PrevValue=1; YSum+=y*(BPixel[0]+0.5);}
		 else                                 { PrevValue=8; YSum+=y*(BPixel[0]);}
	 }
	 else
	 {
		 if(BPixel[0]>BPixel[2*number-2])      PrevValue=6;
		 else if(BPixel[0]<BPixel[2*number-2]) PrevValue=2;
		 else   
		 {
			  return 0;
		 }
	 }

	 FirstValue=PrevValue; // 1�� �ȼ��� �˻����� FirstValue�� �̸� ����

	 
	 for(i=1;i<number;i++)
	 {
        y=(BPixel[2*i+1] + BPixel[2*i-1])/2.0;
		if(BPixel[2*i+1] > BPixel[2*i-1])
		{
		  if(BPixel[2*i]>BPixel[2*i-2])      { CurrValue=5; YSum-=y*(BPixel[2*i]-0.5);}
		  else if(BPixel[2*i]<BPixel[2*i-2]) { CurrValue=3; YSum-=y*(BPixel[2*i]+0.5);}
		  else                               { CurrValue=4; YSum-=y*(BPixel[2*i]);}
		  
		}
		else if(BPixel[2*i+1] < BPixel[2*i-1])
		{
		  if(BPixel[2*i]>BPixel[2*i-2])      { CurrValue=7; YSum+=y*(BPixel[2*i]-0.5);}
		  else if(BPixel[2*i]<BPixel[2*i-2]) { CurrValue=1; YSum+=y*(BPixel[2*i]+0.5);}
		  else                               { CurrValue=8; YSum+=y*(BPixel[2*i]);}
		}
		else
		{
		  if(BPixel[2*i]>BPixel[2*i-2])      CurrValue=6;
		  else if(BPixel[2*i]<BPixel[2*i-2]) CurrValue=2;
		  else   
		  {
			  return 0;
		  }
		}

		// ©���� ������ ���⼭ ��������
		yExcept=BPixel[2*i-1];
		YSum+=yExcept*L[PrevValue][CurrValue]/8.0;   // 1�� ���� number-1 ����.

		PrevValue=CurrValue;		

	 }

	 yExcept=BPixel[2*number-1];									   
	 YSum+=yExcept*L[PrevValue][FirstValue]/8.0; // number ° ���� ����      //

     *cy=YSum/Area;

	 return 0;
}

int CChain::FindMaxX(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[0];
     for(i=1;i<number;i++)
		 if(BPixel[2*i]>Data) Data=BPixel[2*i];

	 return Data;
}

int CChain::FindMinX(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[0];
     for(i=1;i<number;i++)
		 if(BPixel[2*i]<Data) Data=BPixel[2*i];

	 return Data;
}
int CChain::FindMaxY(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[1];
     for(i=1;i<number;i++)
		 if(BPixel[2*i+1]>Data) Data=BPixel[2*i+1];

	 return Data;
}
int CChain::FindMinY(int N)
{
	 int  i,Data;
	 int number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 Data=BPixel[1];
     for(i=1;i<number;i++)
		 if(BPixel[2*i+1]<Data) Data=BPixel[2*i+1];

	 return Data;
}


// �߽ɿ����� ���Ʈ�� ���Ѵ�.
double CChain::FindMoment(int N)
{
	 double cx,cy;
     Chain_Center( N, &cx, &cy);
     return FindMoment(N, cx, cy);
}
double CChain::FindMoment(int N, double cx, double cy)
{
	 int number;
     int *BPixel;      
	 double dMoment;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

     dMoment=0;
	 for(int i=0;i<number;i++)
		 dMoment+=(BPixel[2*i]-cx)*(BPixel[2*i]-cx)+(BPixel[2*i+1]-cy)*(BPixel[2*i+1]-cy);

	 return (dMoment/number);
}


double CChain::FindCompactness(int N)
{
	 double dCompactness;
	 double dLength;       
	 double dArea;

	 dLength = Chain_Length(N);
	 dArea   = Chain_Area(N);
	 dCompactness=(4*CHAIN_PI*dArea)/(dLength*dLength);

     if(dCompactness>1.0) dCompactness=1.0;

	 return dCompactness;
}

// angle = atan2( 2*(M00*M11-M10*M01),(M00*M20-M10*M10)-(M00*M02-M01*M01))*90/3.141592;
// angle�� degree�̴�.
double CChain::FindAngle(int N)
{
	 int number;
     int *BPixel; 
     double M00, M11, M10, M01, M20, M02;
	 double dAngle;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 M00=M11=M10=M01=M20=M02=0;
	 for(int i=0;i < number ;i++)
     {
		 M00++;
		 M10+=(BPixel[2*i]);
		 M01+=(BPixel[2*i+1]);
		 M11+=(BPixel[2*i])*(BPixel[2*i+1]);
		 M20+=(BPixel[2*i])*(BPixel[2*i]);
		 M02+=(BPixel[2*i+1])*(BPixel[2*i+1]);
     }
	 dAngle = atan2(2*(M00*M11-M10*M01),(M00*M20-M10*M10)-(M00*M02-M01*M01))*90/CHAIN_PI;

	 return  dAngle;
}

//�� ������ ����հŸ�, �����Ÿ�, ��հŸ����ϴ� �Լ� 
int CChain::FineDistFromPoint(int N, double cx,double cy, double *min, double *max, double *avg)
{
	 int i, number;
     int *BPixel;      
	 double dist,sum;

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 
	
	 dist=sqrt((BPixel[0]-cx)*(BPixel[0]-cx)+(BPixel[1]-cy)*(BPixel[1]-cy));
	 *min=*max=sum=dist;
	 for(i=1;i<number;i++)
	 {
		 dist=sqrt((BPixel[2*i]-cx)*(BPixel[2*i]-cx)+(BPixel[2*i+1]-cy)*(BPixel[2*i+1]-cy));
		 if(dist<*min) *min=dist;
		 if(dist>*max) *max=dist;
		 sum+=dist;
	 }
	 *avg=sum/number;

	return 0;
}

//������ ü���� YValue���� ���� ü���� �¿� x��ǥ 
int CChain::GetLeft_RightBoundary(int N, int YValue, int *left, int *right)
{
	 int i, number;
     int *BPixel;      

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 *left=WIDTH_MEM-1;
	 *right=0;
	for(i=0;i<number;i++)
	{
		if(BPixel[2*i+1]==YValue)
		{
			if(BPixel[2*i]<*left) *left=BPixel[2*i];
			if(BPixel[2*i]>*right) *right=BPixel[2*i];
		}
	}
	return 0;
}


// ����� ������ Box�� ����� Blob�ּ� �簢���� ã�´�.
// N: Blob��ȣ
// (Cx, Cy) Blob �߽�
// angle : ���ఢ�� 
//
//		*[0]
//	   *   *		
//	  * [1]  *[3]
//      *   *
//		   *[2]
//
//

int CChain::FindMinRect(int N, double Cx, double Cy, double angle,  double pos[][2])
{
//ty=ax+b;
	int i;
	int number;
    int *BPixel;   
	double t, a, b;
	double at[4], aa[4], ab[4];
	int    Pos[4][3];			//[0]:x  [1]:y  [2]: angle 
	double dist, maxDist=0, minDist=1000;
	double divideConst;


	number = m_chain[N];
    BPixel = &m_chaindata[m_chainstart[N]]; 

	if(angle<0.01 && angle>-0.01) 
	{
		pos[0][0]=pos[2][0]=BPixel[0];
		pos[0][1]=pos[2][1]=BPixel[1];
		
		for(i=1;i<number;i++)
		{
			if(BPixel[2*i]<pos[0][0])			pos[0][0]=BPixel[2*i];
			else if(BPixel[2*i]>pos[2][0])		pos[2][0]=BPixel[2*i];

			if(BPixel[2*i+1]<pos[0][1])			pos[0][1]=BPixel[2*i+1];
			else if(BPixel[2*i+1]>pos[2][1])	pos[2][1]=BPixel[2*i+1];
		}
		pos[1][0]=pos[0][0];
		pos[1][1]=pos[2][1];
		pos[3][0]=pos[2][0];
		pos[3][1]=pos[0][1];
		return 0;
	}

	Pos[0][0]=Pos[1][0]=Pos[2][0]=Pos[3][0]=-1;
	//�¿� 2�� -----------------------------
	while(fabs(angle)>90.0)
	{
		if(angle>90.0) angle-=180.0;
		else           angle+=180.0;
	}

	if( fabs(angle-90.0)<0.00001)
	{
		t=0;
		a=1;
		b=-Cx;
	}
	else
	{
		t=1;
		a=tan(angle*CHAIN_PI/180.0);
		b=Cy-a*Cx;
	}

	Pos[0][2]=Pos[1][2]=(int)angle;
	divideConst=sqrt(t*t+a*a);
	for(i=0;i<number;i++)
	{
		dist=(a*BPixel[2*i]+b-BPixel[2*i+1])/divideConst;
		if(dist<minDist){
			minDist=dist;
			Pos[0][0]=BPixel[2*i];
			Pos[0][1]=BPixel[2*i+1];
		}	
		else if(dist>maxDist){
			maxDist=dist;
			Pos[1][0]=BPixel[2*i];
			Pos[1][1]=BPixel[2*i+1];
		}
	}
	//----------------------------------------------------


	//���� 2�� -------------------------------------------
	minDist=1000;
	maxDist=0;
	angle+=90;
	while(fabs(angle)>90.0)
	{
		if(angle>90.0) angle-=180.0;
		else           angle+=180.0;
	}
	if( fabs(angle-90.0)<0.00001)
	{
		t=0;
		a=1;
		b=-Cx;
	}
	else
	{
		t=1;
		a=tan(angle*CHAIN_PI/180.0);
		b=Cy-a*Cx;
	}	

	Pos[2][2]=Pos[3][2]=(int)angle;
	divideConst=sqrt(t*t+a*a);
	for(i=0;i<number;i++)
	{
		dist=(a*BPixel[2*i]+b-BPixel[2*i+1])/divideConst;
		if(dist<minDist){
			minDist=dist;
			Pos[2][0]=BPixel[2*i];
			Pos[2][1]=BPixel[2*i+1];
		}	
		else if(dist>maxDist){
			maxDist=dist;
			Pos[3][0]=BPixel[2*i];
			Pos[3][1]=BPixel[2*i+1];
		}
	}
	//--------------------------------------------------

	if(Pos[0][0]>0 && Pos[1][0]>0 && Pos[2][0]>0 && Pos[3][0]>0)
	{
		//Pos Data�� ���� 4���� ������ ���Ѵ�.
		for(i=0;i<4;i++)
		{
			if( fabs(Pos[i][2]-90.0)<0.00001)
			{
				at[i]=0;
				aa[i]=1;
				ab[i]=-Pos[i][0];
			}
			else
			{
				at[i]=1;
				aa[i]=tan(Pos[i][2]*CHAIN_PI/180.0);
				ab[i]=Pos[i][1]-aa[i]*Pos[i][0];
			}
		}

		//4���� ������ ������ ���Ѵ�.
		FindCrossPoint(at[0], aa[0], ab[0], at[2], aa[2], ab[2], &pos[0][0], &pos[0][1]);
		FindCrossPoint(at[0], aa[0], ab[0], at[3], aa[3], ab[3], &pos[1][0], &pos[1][1]);
		FindCrossPoint(at[1], aa[1], ab[1], at[3], aa[3], ab[3], &pos[2][0], &pos[2][1]);
		FindCrossPoint(at[1], aa[1], ab[1], at[2], aa[2], ab[2], &pos[3][0], &pos[3][1]);
	}
	else
	{
		pos[0][0]=pos[2][0]=BPixel[0];
		pos[0][1]=pos[2][1]=BPixel[1];
		
		for(i=1;i<number;i++)
		{
			if(BPixel[2*i]<pos[0][0])			pos[0][0]=BPixel[2*i];
			else if(BPixel[2*i]>pos[2][0])		pos[2][0]=BPixel[2*i];

			if(BPixel[2*i+1]<pos[0][1])			pos[0][1]=BPixel[2*i+1];
			else if(BPixel[2*i+1]>pos[2][1])	pos[2][1]=BPixel[2*i+1];
		}

		pos[1][0]=pos[0][0];
		pos[1][1]=pos[2][1];
		pos[3][0]=pos[2][0];
		pos[3][1]=pos[0][1];
	}

	return 0;
}

int CChain::FindCrossPoint(double t1, double a1, double b1, double t2, double a2, double b2, double *cx, double *cy)
{
	double local_LIMIT=1e-10;
	double dd;

	dd=-a1*t2+a2*t1;

	if(fabs(dd)<local_LIMIT) return -1;

	*cx=(t2*b1-t1*b2)/dd;
	*cy=(a2*b1-a1*b2)/dd;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
//	Line Pattern Matching    YWKO
//
//////////////////////////////////////////////////////////////////////////////
#define PATTERNSIZE			200
#define MAXBOUNDARYSIZE		1000
int CChain::GetNormalizedLinePattern(int N, int *dist)
{
	 int i;
	 int number;
     int *BPixel;   
	 double cx, cy;
	 int  Dist[1000];

	 FindCenterWithBoundary(N, &cx, &cy);

	 number = m_chain[N];
     BPixel = &m_chaindata[m_chainstart[N]]; 

	 if(number<=0) return 0;
	 if(number>MAXBOUNDARYSIZE) number=MAXBOUNDARYSIZE;
	 for(i=0;i<number;i++)
	 {
		Dist[i]=(int)sqrt((cx-BPixel[2*i])*(cx-BPixel[2*i])+
						  (cy-BPixel[2*i+1])*(cy-BPixel[2*i+1])  );

	 }

	 for(i=0;i<PATTERNSIZE;i++)
	 {
		dist[i]=Dist[(int)(i*number/PATTERNSIZE)];
	 }

	 return PATTERNSIZE;
}

double CChain::FindCorrelation(int *fmT, int *fmS, int nLineLength)
{
	int i,j, Loop;
	double Area, Sig_Ti, Sig_Ti2;          /* Correlation �߰��� ���� */
	double       Sig_Si, Sig_Si2, Sig_TiSi;/*             "           */
    int tempT, tempS, temp;
	double dDiv;
	double dCorrData, dMaxCorrData=0;

	Area = (double)nLineLength;
	Sig_Ti=Sig_Ti2=0;
	for(i=0;i<nLineLength;i++) 
	{
		tempT    = *(fmT+i);
		Sig_Ti  += tempT;
		Sig_Ti2 += tempT*tempT;
	}

	for(Loop=0;Loop<nLineLength;Loop++)
	{
		Sig_Si = Sig_Si2  = Sig_TiSi =0;
		for(i=0;i<nLineLength;i++) 
		{
			tempS   = *(fmS+i);
			Sig_Si  += tempS;
			Sig_Si2 += tempS*tempS;
			Sig_TiSi+= tempS**(fmT+i); 
		}    		

		dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		if (dDiv > 0.0001 ) dCorrData = ( Area*Sig_TiSi - Sig_Ti*Sig_Si)/dDiv;
		else                dCorrData = 0.0;

		if(dCorrData>dMaxCorrData) dMaxCorrData=dCorrData;


		temp=*(fmS);				//Array ó�� ����
		for(j=0;j<nLineLength-1;j++)
		{
			*(fmS+j)=*(fmS+j+1);
		}
		*(fmS+nLineLength-1)=temp;  //Array ó���� ���� ���� 

	}
	return dMaxCorrData;
}