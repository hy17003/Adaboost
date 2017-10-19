#pragma once
#include "define.h"
#include <opencv2\opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

class HaarExtractor
{
public:
	HaarExtractor();
	~HaarExtractor();
	void InitHaarExtractor(int iWidth, int iHeight);
	vector<double> GetHaarFeature(Mat image);
	int GetHaarFeatureNumber();
private:
	int iImageWidth;
	int iImageHeight;
	vector<HAARECT_STRU> struHaarRects;
	int* pIntergralImage;
};

