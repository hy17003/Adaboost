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
	//��ȡ����������haar����
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
	//����haar������������������Ӧ����������������ÿ������������Ӧһ��haar����
	int iHaarNum = haarExtractor.GetHaarFeatureNumber();
	weakClassifiers = vector<WeakClassifier>(iHaarNum);
	//ѵ��ÿһ����������
	for (int i = 0; i < weakClassifiers.size(); i++)
	{
		
	}

}


void Cascade::Train(TrainData& trainData)
{
	//ѵ����������
	//1.������ֵ��С��������
	SortTrainData(trainData);
	//2.������������
	for (int i = 0; i < trainData.trainData.size() - 1; i++)
	{
		if (trainData.trainData[i].second != trainData.trainData[i + 1].second)
		{
			WeakClassifier wf;
			wf.SetThreshold( 0.5 * (trainData.trainData[i].first + trainData.trainData[i + 1].first));
			weakClassifiers.push_back(wf);
		}
	}
	//3.��ÿ��������������������ȷ��
	for (int i = 0; i < weakClassifiers.size(); i++)
	{
		weakClassifiers[i].TestBunch(trainData);
		if (weakClassifiers[i].dError > 0.5)
		{
			weakClassifiers[i].InvertPolarity();
			weakClassifiers[i].TestBunch(trainData);
		}
	}
	//4.�Է�������������
	SortWeakClassifier(weakClassifiers);
	//5.ѵ��ǿ������
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