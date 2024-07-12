#pragma once
class DefectType
{
public:
	int _SpotW;				//����, ����
	int _SpotB;				//����
	int _SpotPress;			//����
	int _Cunic;				//���
	int _LamiPress;			//Lami Press
	int _Scratch;			//��ũ��ġ
	int _LScratch;			//Long��ũ��ġ
	int _Kkopchim;			//��ħ
	int _Group;				//����
	int _Mura;				//���
	int _TacMura;			//TAC�� ���
	int _TacMuraB;			//TAC�� (��)���
	int _TacLine;			//TAC����
	int _SpotKipo;			//�̹�������(������)
	int _Line;				//����
	int _LineAngle;			//�缱����
	int _LineTD;			//TD����
	int _KipoPress;			//�������� (COS7 76)
	int _LineKipo;			//���α���(COS7)
	int _BlackMura;			//��������
	int _blackGroup;		//�����決��
	int _SurfaceBumpy;		//ǥ���ö (COSA)

public:
	DefectType();
	~DefectType();
	void Save();
	void Load();

};
extern DefectType g_NGType;
