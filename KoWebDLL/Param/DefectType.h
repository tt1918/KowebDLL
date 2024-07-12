#pragma once
class DefectType
{
public:
	int _SpotW;				//백점, 휘점
	int _SpotB;				//흑점
	int _SpotPress;			//찍힘
	int _Cunic;				//쿠닉
	int _LamiPress;			//Lami Press
	int _Scratch;			//스크라치
	int _LScratch;			//Long스크라치
	int _Kkopchim;			//꼽침
	int _Group;				//군집
	int _Mura;				//얼룩
	int _TacMura;			//TAC성 얼룩
	int _TacMuraB;			//TAC성 (흑)얼룩
	int _TacLine;			//TAC라인
	int _SpotKipo;			//이물성기포(정투과)
	int _Line;				//라인
	int _LineAngle;			//사선라인
	int _LineTD;			//TD라인
	int _KipoPress;			//기포눌림 (COS7 76)
	int _LineKipo;			//라인기포(COS7)
	int _BlackMura;			//투영흑얼룩
	int _blackGroup;		//투영흑군집
	int _SurfaceBumpy;		//표면요철 (COSA)

public:
	DefectType();
	~DefectType();
	void Save();
	void Load();

};
extern DefectType g_NGType;
