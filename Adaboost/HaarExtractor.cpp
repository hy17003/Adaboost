#include "stdafx.h"
#include "HaarExtractor.h"
#include "TrainData.h"
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


int HaarExtractor::MakeHaarRects(int x, int y, int dx, int dy, int type, int iWidth, int iHeight, HAARECT_STRU& haarRect)
{
	int err = 0;
	if (type == 0)
	{
		//□■
		haarRect.Rects[0].x = x;
		haarRect.Rects[0].y = y;
		haarRect.Rects[0].width = dx;
		haarRect.Rects[0].height = dy;
		haarRect.weight[0] = 0.5;

		haarRect.Rects[1].x = x + dx;
		haarRect.Rects[1].y = y;
		haarRect.Rects[1].width = dx;
		haarRect.Rects[1].height = dy;
		haarRect.weight[1] = -0.5;

		haarRect.weight[2] = 0.0;
		haarRect.weight[3] = 0.0;
		if (haarRect.Rects[1].x + haarRect.Rects[1].width > iWidth ||
			haarRect.Rects[1].y + haarRect.Rects[1].height > iHeight)
		{
			err = -1;
		}
	}
	else if (type == 1)
	{
		//■
		//□
		haarRect.Rects[0].x = x;
		haarRect.Rects[0].y = y;
		haarRect.Rects[0].width = dx;
		haarRect.Rects[0].height = dy;
		haarRect.weight[0] = -0.5;

		haarRect.Rects[1].x = x;
		haarRect.Rects[1].y = y + dy;
		haarRect.Rects[1].width = dx;
		haarRect.Rects[1].height = dy;
		haarRect.weight[1] = 0.5;

		haarRect.weight[2] = 0.0;
		haarRect.weight[3] = 0.0;

		if (haarRect.Rects[1].x + haarRect.Rects[1].width > iWidth ||
			haarRect.Rects[1].y + haarRect.Rects[1].height > iHeight)
		{
			err = -1;
		}
	}
	else if (type == 2)
	{
		//□■□
		haarRect.Rects[0].x = x;
		haarRect.Rects[0].y = y;
		haarRect.Rects[0].width = dx;
		haarRect.Rects[0].height = dy;
		haarRect.weight[0] = 0.25;

		haarRect.Rects[1].x = x + dx;
		haarRect.Rects[1].y = y;
		haarRect.Rects[1].width = dx;
		haarRect.Rects[1].height = dy;
		haarRect.weight[1] = -0.5;

		haarRect.Rects[2].x = x + 2 * dx;
		haarRect.Rects[2].y = y;
		haarRect.Rects[2].width = dx;
		haarRect.Rects[2].height = dy;
		haarRect.weight[2] = 0.25;

		haarRect.weight[3] = 0.0;

		if (haarRect.Rects[2].x + haarRect.Rects[2].width > iWidth ||
			haarRect.Rects[2].y + haarRect.Rects[2].height > iHeight)
		{
			err = -1;
		}
	}
	else if (type == 3)
	{
		//□■
		//■□
		haarRect.Rects[0].x = x;
		haarRect.Rects[0].y = y;
		haarRect.Rects[0].width = dx;
		haarRect.Rects[0].height = dy;
		haarRect.weight[0] = 0.25;

		haarRect.Rects[1].x = x + dx;
		haarRect.Rects[1].y = y;
		haarRect.Rects[1].width = dx;
		haarRect.Rects[1].height = dy;
		haarRect.weight[1] = -0.25;

		haarRect.Rects[2].x = x;
		haarRect.Rects[2].y = y + dy;
		haarRect.Rects[2].width = dx;
		haarRect.Rects[2].height = dy;
		haarRect.weight[2] = -0.25;

		haarRect.Rects[3].x = x + dx;
		haarRect.Rects[3].y = y + dy;
		haarRect.Rects[3].width = dx;
		haarRect.Rects[3].height = dy;
		haarRect.weight[3] = 0.25;

		if (haarRect.Rects[3].x + haarRect.Rects[3].width > iWidth ||
			haarRect.Rects[3].y + haarRect.Rects[3].height > iHeight)
		{
			err = -1;
		}
	}
	return err;
}


void HaarExtractor::InitHaarExtractor(int iWidth, int iHeight)
{
	iImageWidth = iWidth;
	iImageHeight = iHeight;
	//生成haar特征
	for (int dx = 2; dx < iWidth; dx++)
	{
		for (int dy = 2; dy < iHeight; dy++)
		{
			for (int x = 0; x < iWidth; x+=2)
			{
				for (int y = 0; y < iHeight; y+=2)
				{
					HAARECT_STRU haarRects0;
					if (MakeHaarRects(x, y, dx, dy, 0, iWidth, iHeight, haarRects0) != -1)
					{
						struHaarRects.push_back(haarRects0);
					}
					// debug if (struHaarRects.size() > 30)return;
					HAARECT_STRU haarRects1;
					if (MakeHaarRects(x, y, dx, dy, 1, iWidth, iHeight, haarRects1) != -1)
					{
						struHaarRects.push_back(haarRects1);
					}
					HAARECT_STRU haarRects2;
					if (MakeHaarRects(x, y, dx, dy, 2, iWidth, iHeight, haarRects2) != -1)
					{
						struHaarRects.push_back(haarRects2);
					}
					HAARECT_STRU haarRects3;
					if (MakeHaarRects(x, y, dx, dy, 3, iWidth, iHeight, haarRects3) != -1)
					{
						struHaarRects.push_back(haarRects3);
					}
				}
			}
		}
	}

	//HAARECT_STRU haarRects0;
	//haarRects0.Rects[0].x = 0;
	//haarRects0.Rects[0].y = 4;
	//haarRects0.Rects[0].width = 1;
	//haarRects0.Rects[0].height = 5;
	//haarRects0.weight[0] = 1.0;

	//haarRects0.Rects[1].x = 1;
	//haarRects0.Rects[1].y = 4;
	//haarRects0.Rects[1].width = 1;
	//haarRects0.Rects[1].height = 5;
	//haarRects0.weight[1] = -2.0;

	//haarRects0.Rects[2].x = 3;
	//haarRects0.Rects[2].y = 4;
	//haarRects0.Rects[2].width = 1;
	//haarRects0.Rects[2].height = 5;
	//haarRects0.weight[2] = 1.0;

	//haarRects0.weight[3] = 0.0;

	////第二个haar特征
	//HAARECT_STRU haarRects1;
	//haarRects1.Rects[0].x = 3;
	//haarRects1.Rects[0].y = 7;
	//haarRects1.Rects[0].width = 4;
	//haarRects1.Rects[0].height = 1;
	//haarRects1.weight[0] = 1.0;

	//haarRects1.Rects[1].x = 3;
	//haarRects1.Rects[1].y = 8;
	//haarRects1.Rects[1].width = 4;
	//haarRects1.Rects[1].height = 1;
	//haarRects1.weight[1] = -2.0;

	//haarRects1.Rects[2].x = 3;
	//haarRects1.Rects[2].y = 9;
	//haarRects1.Rects[2].width = 4;
	//haarRects1.Rects[2].height = 1;
	//haarRects1.weight[2] = 1.0;

	//haarRects1.weight[3] = 0.0;

	////第三个haar特征
	//HAARECT_STRU haarRects2;
	//haarRects2.Rects[0].x = 7;
	//haarRects2.Rects[0].y = 4;
	//haarRects2.Rects[0].width = 1;
	//haarRects2.Rects[0].height = 5;
	//haarRects2.weight[0] = 1.0;

	//haarRects2.Rects[1].x = 8;
	//haarRects2.Rects[1].y = 4;
	//haarRects2.Rects[1].width = 1;
	//haarRects2.Rects[1].height = 5;
	//haarRects2.weight[1] = -2.0;

	//haarRects2.Rects[2].x = 9;
	//haarRects2.Rects[2].y = 4;
	//haarRects2.Rects[2].width = 1;
	//haarRects2.Rects[2].height = 5;
	//haarRects2.weight[2] = 1.0;

	//haarRects2.weight[3] = 0.0;

	//struHaarRects.push_back(haarRects0);
	//struHaarRects.push_back(haarRects1);
	//struHaarRects.push_back(haarRects2);
}

vector<float> HaarExtractor::GetHaarFeature(Mat image)
{
	resize(image, image, Size(iImageWidth, iImageHeight));
	vector<float> features;
	//计算积分图
	Mat interalImage;
	integral(image, interalImage);
	for (int i = 0; i < struHaarRects.size(); i++)
	{
		HAARECT_STRU& featureRect = struHaarRects[i];
		float val = 0;
		for (int j = 0; j < 4; j++)
		{
			float localVal = 0.0;
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

vector<HAARECT_STRU> HaarExtractor::GetHaarRects()
{
	return struHaarRects;
}

