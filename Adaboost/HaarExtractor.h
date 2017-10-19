#pragma once
#include "define.h"
#include <opencv2\opencv.hpp>
#include <vector>
using namespace std;


class HaarExtractor
{
public:
	HaarExtractor();
	~HaarExtractor();
	void InitHaarExtractor(int iWidth, int iHeight);
	vector<double> getHaarFeature(unsigned char* pImageData, int iWidth, int iHeihgt);
private:
	int iImageWidth;
	int iImageHeight;
	vector<HAARECT_STRU> struHaarRects;
};

