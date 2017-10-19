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


void main1()
{
	TrainData td;
	double x[10] = { 0,1,2,3,4,5,6,7,8,9 };
	int y[10] = { 1,1,1,-1,-1,-1,1,1,1,-1 };
	for (int i = 0; i < 10; i++)
	{
		td.AddItem(make_pair(x[i], y[i]));
	}
	Cascade cascade;
	cascade.Train(td);
	for (int i = 0; i < 10; i++)
	{
		cout << cascade.Predict(x[i]) << endl;
	}
	system("pause");
}

//void main()
//{
//	Mat testMat(Size(10, 10), CV_8UC1, Scalar::all(1));
//	HaarExtractor haarExtractor;
//	haarExtractor.InitHaarExtractor(50, 50);
//	vector<double> features = haarExtractor.getHaarFeature(testMat);
//	FOR_EACH(features, PRINT);
//}

void main()
{
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	Mat mat(Size(3, 1), CV_32SC1, vec.data());
	cout << mat << endl;
	Mat line(Size(3, 1), CV_32SC1, vec.data());
	mat.push_back(line);
	cout << mat << endl;
}