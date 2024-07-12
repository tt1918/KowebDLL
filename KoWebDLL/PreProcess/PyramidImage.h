#pragma once

#define COMP_LEVEL	5
class PyramidImage
{
private:
	unsigned char* _Image[COMP_LEVEL];
	int			   _Width[COMP_LEVEL];
	int			   _Height[COMP_LEVEL];

	int				_RefW;
	int				_RefH;

public:
	
public:
	PyramidImage();
	~PyramidImage();

	void SetImageInfo(int width, int height);

	unsigned char* GetImagePt(int level) { return _Image[level]; }
	int GetImageWidth(int level) { return _Width[level]; }
	int GetImageHeight(int level) { return _Height[level]; }

	bool MakeImage(unsigned char* buf, int width, int height);

private:
	void DeleteBuf();
	void CreateBuf();

};

