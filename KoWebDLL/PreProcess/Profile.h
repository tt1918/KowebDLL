#pragma once
class Profile
{
private:
	int* _pProfile;			// �������� ������
	int* _pProfile100;		// �������� ����(100��)
	int* _pProfileH;		// ���������� W�� 1/2�� ����

	int  _nProfileWidth;
	
	int* _pTmpProf;
	int* _pTmpCnt;

	int	 _AvgBright;

public:
	Profile();
	~Profile();

	void MakeProfileData(unsigned char* src, int width, int height, int pitch);

	int	CalcAvgBright(int stX, int edX, int refBright);
	int GetAvgBright() { return _AvgBright; }


	int* GetProfile() { return _pProfile; }
	int  GetWidth() { return _nProfileWidth; }

private:
	void CreateProfile(int width);
	void DeleteProfile();


};

