#pragma once
#include "define.h"
#include <opencv2\opencv.hpp>
#include "TrainData.h"
#include <vector>
using namespace std;
using namespace cv;

class HaarExtractor
{
public:
	HaarExtractor();
	~HaarExtractor();
	void InitHaarExtractor(int iWidth, int iHeight);
	vector<float> GetHaarFeature(Mat image);
	vector<HAARECT_STRU> GetHaarRects();
	int GetHaarFeatureNumber();
private:
	int MakeHaarRects(int x, int y, int dx, int dy, int type, int iWidth, int iHeight, HAARECT_STRU& rect);
private:
	int iImageWidth;
	int iImageHeight;
	vector<HAARECT_STRU> struHaarRects;
	int* pIntergralImage;
};

