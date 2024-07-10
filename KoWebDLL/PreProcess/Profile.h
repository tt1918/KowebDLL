#pragma once
class Profile
{
private:
	int* m_pProfile;		// �������� ������
	int* m_pProfile100;		// �������� ����(100��)
	int* m_pProfileH;		// ���������� W�� 1/2�� ����

	int  m_nProfileWidth;
	
	int* m_pTmpProf;
	int* m_pTmpCnt;

public:
	Profile();
	~Profile();

	void MakeProfileData(unsigned char* src, int width, int height, int pitch);

private:
	void CreateProfile(int width);
	void DeleteProfile();


};

