#include "stdafx.h"
#include "Cascade.h"
#include <algorithm>
#include "HaarExtractor.h"

bool CompareTrainItem(TRAIN_ITEM& ti1, TRAIN_ITEM& ti2)
{
	return ti1.first < ti2.first;
}

bool ComareWeakClassifier(WeakClassifier& wf1, WeakClassifier& wf2)
{
	return wf1.dError < wf2.dError;
}


Cascade::Cascade()
{
	dMinAccuracy = 0.95;
	dMaxFalseAlarm = 0.60;
	iMaxStageNum = 40;
}


Cascade::~Cascade()
{

}

void Cascade::Train(vector<string> imagePath, vector<int> labels)
{
	//提取所有样本的haar特征
	HaarExtractor haarExtractor;
	haarExtractor.InitHaarExtractor(10, 10);
	Mat trainMat;
	Mat trainLabel(Size(labels.size(),1), CV_32SC1, labels.data());
	for (int i = 0; i < imagePath.size(); i++)
	{
		Mat image = imread(imagePath[i], 0);
		vector<double> feats = haarExtractor.GetHaarFeature(image);
		if (i == 0)
		{
			trainMat = Mat(Size(1, feats.size()), CV_32FC1, feats.data());
		}
		else
		{
			Mat line(Size(1, feats.size()), CV_32FC1, feats.data());
			trainMat.push_back(line);
		}
	}
	//根据haar特征的数量，生成相应数量的弱分类器，每个弱分类器对应一个haar特征
	int iHaarNum = haarExtractor.GetHaarFeatureNumber();
	weakClassifiers = vector<WeakClassifier>(iHaarNum);
	//训练每一个弱分类器
	for (int i = 0; i < weakClassifiers.size(); i++)
	{
		
	}

}


void Cascade::Train(TrainData& trainData)
{
	//训练弱分类器
	//1.将特征值从小到大排序
	SortTrainData(trainData);
	//2.生成弱分类器
	for (int i = 0; i < trainData.trainData.size() - 1; i++)
	{
		if (trainData.trainData[i].second != trainData.trainData[i + 1].second)
		{
			WeakClassifier wf;
			wf.SetThreshold( 0.5 * (trainData.trainData[i].first + trainData.trainData[i + 1].first));
			weakClassifiers.push_back(wf);
		}
	}
	//3.对每个弱分类器，计算其正确率
	for (int i = 0; i < weakClassifiers.size(); i++)
	{
		weakClassifiers[i].TestBunch(trainData);
		if (weakClassifiers[i].dError > 0.5)
		{
			weakClassifiers[i].InvertPolarity();
			weakClassifiers[i].TestBunch(trainData);
		}
	}
	//4.对分类器进行排序
	SortWeakClassifier(weakClassifiers);
	//5.训练强分类器
	StrongClassifier strongClassifier(&weakClassifiers);
	strongClassifier.Train(trainData);
	strongClassifiers.push_back(strongClassifier);

}

void Cascade::SortTrainData(TrainData& trainData)
{
	std::sort(trainData.trainData.begin(), trainData.trainData.end(), CompareTrainItem);
}

void Cascade::SortWeakClassifier(vector<WeakClassifier>& wfs)
{
	std::sort(wfs.begin(), wfs.end(), ComareWeakClassifier);
}

int Cascade::Predict(FEATURE feature)
{
	int iRes = 1;
	for (int i = 0; i < strongClassifiers.size(); i++)
	{
		if (strongClassifiers[i].Predict(feature) == -1)
		{
			iRes = -1;
			break;
		}
	}
	return iRes;
}

void Cascade::SetTerminateCriteria(int _iMaxStageNum, double _dMinAccuracy, double _dMaxFalseAlarm)
{
	iMaxStageNum = _iMaxStageNum;
	dMinAccuracy = _dMinAccuracy;
	dMaxFalseAlarm = _dMaxFalseAlarm;
}