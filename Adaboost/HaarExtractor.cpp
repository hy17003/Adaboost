#include "stdafx.h"
#include "HaarExtractor.h"


HaarExtractor::HaarExtractor()
{

}


HaarExtractor::~HaarExtractor()
{

}

void HaarExtractor::InitHaarExtractor(int iWidth, int iHeight)
{
	iImageWidth = iImageWidth;
	iImageHeight = iImageHeight;
	//第一个haar特征
	HAARECT_STRU haarRects0;
	haarRects0.Rects[0].x = 0;
	haarRects0.Rects[0].y = 20;
	haarRects0.Rects[0].width = 5;
	haarRects0.Rects[0].height = 25;
	haarRects0.weight[0] = 1.0;

	haarRects0.Rects[1].x = 5;
	haarRects0.Rects[1].y = 20;
	haarRects0.Rects[1].width = 5;
	haarRects0.Rects[1].height = 25;
	haarRects0.weight[1] = -2.0;

	haarRects0.Rects[2].x = 15;
	haarRects0.Rects[2].y = 20;
	haarRects0.Rects[2].width = 5;
	haarRects0.Rects[2].height = 25;
	haarRects0.weight[2] = 1.0;

	haarRects0.weight[3] = 0.0;

	//第二个haar特征
	HAARECT_STRU haarRects1;
	haarRects1.Rects[0].x = 15;
	haarRects1.Rects[0].y = 35;
	haarRects1.Rects[0].width = 20;
	haarRects1.Rects[0].height = 5;

	haarRects1.Rects[1].x = 15;
	haarRects1.Rects[1].y = 40;
	haarRects1.Rects[1].width = 20;
	haarRects1.Rects[1].height = 5;

	haarRects1.Rects[2].x = 15;
	haarRects1.Rects[2].y = 45;
	haarRects1.Rects[2].width = 20;
	haarRects1.Rects[2].height = 5;

	//第三个haar特征
	HAARECT_STRU haarRects2;
	haarRects2.Rects[0].x = 35;
	haarRects2.Rects[0].y = 20;
	haarRects2.Rects[0].width = 5;
	haarRects2.Rects[0].height = 25;

	haarRects2.Rects[1].x = 40;
	haarRects2.Rects[1].y = 20;
	haarRects2.Rects[1].width = 5;
	haarRects2.Rects[1].height = 25;

	haarRects2.Rects[2].x = 45;
	haarRects2.Rects[2].y = 20;
	haarRects2.Rects[2].width = 5;
	haarRects2.Rects[2].height = 25;
	
	struHaarRects.push_back(haarRects0);
	struHaarRects.push_back(haarRects1);
	struHaarRects.push_back(haarRects2);
}