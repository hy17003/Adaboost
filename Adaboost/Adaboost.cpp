// Adaboost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Classifier.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include "Cascade.h"

using namespace std;

void main()
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