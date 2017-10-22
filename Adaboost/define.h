#ifndef DEFINE_H
#define DEFINE_H
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;


typedef struct RECT_STRU
{
	int x;
	int y;
	int width;
	int height;
}RECT_STRU, *PRECT_STRU;

typedef struct HAARECT_STRU
{
	Rect Rects[4];
	float weight[4];
}HAARECT_STRU, *PHAARECT_STRU;


void readTxtToVector(string filename, vector<string>* pImagePath, vector<int>* pLabel = NULL);

#endif