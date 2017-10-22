// Adaboost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Classifier.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include "Cascade.h"
#include "HaarExtractor.h"

using namespace std;
using namespace cv;

#define PRINT(VAL) cout << VAL << ' ';
#define FOR_EACH(VECTOR, FUNC) for(int i = 0;i<VECTOR.size();i++)FUNC(VECTOR[i]);


//
//void main1()
//{
//	TrainData td(1);
//	float x[10] = { 0,1,2,3,4,5,6,7,8,9 };
//	int y[10] = { 1,1,1,-1,-1,-1,1,1,1,-1 };
//	for (int i = 0; i < 10; i++)
//	{
//		vector<float> feat;
//		feat.push_back(x[i]);
//		td.AddItem(make_pair(feat, y[i]));
//	}
//	Cascade cascade;
//	cascade.Train(td);
//	for (int i = 0; i < 10; i++)
//	{
//		vector<float> feat;
//		feat.push_back(x[i]);
//		cout << cascade.Predict(feat) << endl;
//	}
//	system("pause");
//}

void main()
{
	Cascade cascade;
	vector<string> strImagePath;
	vector<int> labels;
	TrainData trainData;
	readTxtToVector("train.txt", &strImagePath, &labels);
	cascade.InitCascade(10, 10);
	cascade.ExtractHaarFeature(strImagePath, labels, trainData);
	trainData.Save("haarFeatures.bin");
	TrainData td;
	td.Load("haarFeatures.bin");
	cascade.Train(td);
	//测试
	int rightNum = 0, wrongNum = 0;
	for (int i = 0; i < strImagePath.size(); i++)
	{
		Mat image = imread(strImagePath[i], 0);
		if (image.data)
		{
			imshow("Image", image);
			int res = cascade.Predict(image);
			if (labels[i] == res)
			{
				rightNum++;
			}
			else
			{
				wrongNum++;
			}
			cout << cascade.Predict(image) << " acc: " << double(rightNum) / (rightNum + wrongNum) << endl;
			waitKey(10);
		}
	}
	system("pause");
}

//
//void main()
//{
//	HaarExtractor haarExtractor;
//	haarExtractor.InitHaarExtractor(10, 10);
//	vector<HAARECT_STRU> haarRects = haarExtractor.GetHaarRects();
//	for (int i = 0; i < haarRects.size(); i++)
//	{
//		Mat haarMat(Size(500, 500), CV_8UC3, Scalar::all(128));
//		for (int j = 0; j < 4; j++)
//		{
//			Rect rect;
//			rect = haarRects[i].Rects[j];
//			rect.x *= 50;
//			rect.y *= 50;
//			rect.width *= 50;
//			rect.height *= 50;
//			if (haarRects[i].weight[j] > 0.1)
//			{
//				rectangle(haarMat, rect, Scalar(255, 255, 255), -1);
//				rectangle(haarMat, rect, Scalar(0, 255, 0), 1);
//			}
//			else if (haarRects[i].weight[j] < -0.1)
//			{
//				rectangle(haarMat, rect, Scalar(0, 0, 0), -1);
//				rectangle(haarMat, rect, Scalar(0, 255, 0), 1);
//			}
//		}
//		imshow("haarMat", haarMat);
//		cout << "index: " << i << endl;
//		waitKey(100);
//	}
//}