#include "stdafx.h"
#include "HaarExtractor.h"
#include <iostream>


HaarExtractor::HaarExtractor()
{
	pIntergralImage = NULL;
}


HaarExtractor::~HaarExtractor()
{
	if (pIntergralImage != NULL)
		delete pIntergralImage;
	pIntergralImage = NULL;
}

void HaarExtractor::InitHaarExtractor(int iWidth, int iHeight)
{
	iImageWidth = iImageWidth;
	iImageHeight = iImageHeight;
	////第一个haar特征
	//HAARECT_STRU haarRects0;
	//haarRects0.Rects[0].x = 0;
	//haarRects0.Rects[0].y = 20;
	//haarRects0.Rects[0].width = 5;
	//haarRects0.Rects[0].height = 25;
	//haarRects0.weight[0] = 1.0;

	//haarRects0.Rects[1].x = 5;
	//haarRects0.Rects[1].y = 20;
	//haarRects0.Rects[1].width = 5;
	//haarRects0.Rects[1].height = 25;
	//haarRects0.weight[1] = -2.0;

	//haarRects0.Rects[2].x = 15;
	//haarRects0.Rects[2].y = 20;
	//haarRects0.Rects[2].width = 5;
	//haarRects0.Rects[2].height = 25;
	//haarRects0.weight[2] = 1.0;

	//haarRects0.weight[3] = 0.0;

	////第二个haar特征
	//HAARECT_STRU haarRects1;
	//haarRects1.Rects[0].x = 15;
	//haarRects1.Rects[0].y = 35;
	//haarRects1.Rects[0].width = 20;
	//haarRects1.Rects[0].height = 5;

	//haarRects1.Rects[1].x = 15;
	//haarRects1.Rects[1].y = 40;
	//haarRects1.Rects[1].width = 20;
	//haarRects1.Rects[1].height = 5;

	//haarRects1.Rects[2].x = 15;
	//haarRects1.Rects[2].y = 45;
	//haarRects1.Rects[2].width = 20;
	//haarRects1.Rects[2].height = 5;

	////第三个haar特征
	//HAARECT_STRU haarRects2;
	//haarRects2.Rects[0].x = 35;
	//haarRects2.Rects[0].y = 20;
	//haarRects2.Rects[0].width = 5;
	//haarRects2.Rects[0].height = 25;

	//haarRects2.Rects[1].x = 40;
	//haarRects2.Rects[1].y = 20;
	//haarRects2.Rects[1].width = 5;
	//haarRects2.Rects[1].height = 25;

	//haarRects2.Rects[2].x = 45;
	//haarRects2.Rects[2].y = 20;
	//haarRects2.Rects[2].width = 5;
	//haarRects2.Rects[2].height = 25;

	HAARECT_STRU haarRects0;
	haarRects0.Rects[0].x = 0;
	haarRects0.Rects[0].y = 4;
	haarRects0.Rects[0].width = 1;
	haarRects0.Rects[0].height = 5;
	haarRects0.weight[0] = 1.0;

	haarRects0.Rects[1].x = 1;
	haarRects0.Rects[1].y = 4;
	haarRects0.Rects[1].width = 1;
	haarRects0.Rects[1].height = 5;
	haarRects0.weight[1] = -2.0;

	haarRects0.Rects[2].x = 3;
	haarRects0.Rects[2].y = 4;
	haarRects0.Rects[2].width = 1;
	haarRects0.Rects[2].height = 5;
	haarRects0.weight[2] = 1.0;

	haarRects0.weight[3] = 0.0;

	//第二个haar特征
	HAARECT_STRU haarRects1;
	haarRects1.Rects[0].x = 3;
	haarRects1.Rects[0].y = 7;
	haarRects1.Rects[0].width = 4;
	haarRects1.Rects[0].height = 1;
	haarRects1.weight[0] = 1.0;

	haarRects1.Rects[1].x = 3;
	haarRects1.Rects[1].y = 8;
	haarRects1.Rects[1].width = 4;
	haarRects1.Rects[1].height = 1;
	haarRects1.weight[1] = -2.0;

	haarRects1.Rects[2].x = 3;
	haarRects1.Rects[2].y = 9;
	haarRects1.Rects[2].width = 4;
	haarRects1.Rects[2].height = 1;
	haarRects1.weight[2] = 1.0;

	haarRects1.weight[3] = 0.0;

	//第三个haar特征
	HAARECT_STRU haarRects2;
	haarRects2.Rects[0].x = 7;
	haarRects2.Rects[0].y = 4;
	haarRects2.Rects[0].width = 1;
	haarRects2.Rects[0].height = 5;
	haarRects2.weight[0] = 1.0;

	haarRects2.Rects[1].x = 8;
	haarRects2.Rects[1].y = 4;
	haarRects2.Rects[1].width = 1;
	haarRects2.Rects[1].height = 5;
	haarRects2.weight[1] = -2.0;

	haarRects2.Rects[2].x = 9;
	haarRects2.Rects[2].y = 4;
	haarRects2.Rects[2].width = 1;
	haarRects2.Rects[2].height = 5;
	haarRects2.weight[2] = 1.0;

	haarRects2.weight[3] = 0.0;

	struHaarRects.push_back(haarRects0);
	struHaarRects.push_back(haarRects1);
	struHaarRects.push_back(haarRects2);
}

vector<double> HaarExtractor::GetHaarFeature(Mat image)
{
	resize(image, image, Size(iImageWidth, iImageHeight));
	vector<double> features;
	//计算积分图
	Mat interalImage;
	integral(image, interalImage);
	for (int i = 0; i < struHaarRects.size(); i++)
	{
		HAARECT_STRU& featureRect = struHaarRects[i];
		double val = 0;
		for (int j = 0; j < 4; j++)
		{
			double localVal = 0.0;
			int x = featureRect.Rects[j].x;
			int y = featureRect.Rects[j].y;
			int w = featureRect.Rects[j].width;
			int h = featureRect.Rects[j].height;
			Point pt1 = Point(x, y);
			Point pt2 = Point(x + w, y);
			Point pt3 = Point(x + w, y + h);
			Point pt4 = Point(x, y + h);
			localVal = interalImage.at<int>(pt3) - interalImage.at<int>(pt2) -
				interalImage.at<int>(pt4) + interalImage.at<int>(pt1);
			val += localVal * featureRect.weight[j];
		}
		features.push_back(val);
	}
	return features;
}

int HaarExtractor::GetHaarFeatureNumber()
{
	return struHaarRects.size();
}