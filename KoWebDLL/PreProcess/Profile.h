#pragma once
class Profile
{
private:
	int* _pProfile;			// 프로파일 데이터
	int* _pProfile100;		// 프로파일 정보(100배)
	int* _pProfileH;		// 프로파일을 W를 1/2로 만듦

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

