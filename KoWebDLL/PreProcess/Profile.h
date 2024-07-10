#pragma once
class Profile
{
private:
	int* m_pProfile;		// 프로파일 데이터
	int* m_pProfile100;		// 프로파일 정보(100배)
	int* m_pProfileH;		// 프로파일을 W를 1/2로 만듦

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

