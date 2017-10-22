#include "stdafx.h"
#include "Cascade.h"
#include <algorithm>

bool CompareWeakMap(pair<int, float>& w1, pair<int, float>& w2)
{
	return (w1.second < w2.second);
}


bool CompareWeakClassifier(WeakClassifier& wf1, WeakClassifier& wf2)
{
	return (wf1.dError < wf2.dError);
}

Cascade::Cascade()
{
	dMinAccuracy = 0.90;
	dMaxFalseAlarm = 0.60;
	iMaxStageNum = 40;
}


Cascade::~Cascade()
{

}


void Cascade::InitCascade(int iWidth, int iHeight, float _dMaxFalseAlarm, int _iMaxStageNum)
{
	haarExtractor.InitHaarExtractor(iWidth, iHeight);
	dMaxFalseAlarm = _dMaxFalseAlarm;
	iMaxStageNum = _iMaxStageNum;
}

void Cascade::Train(vector<string> imagePath, vector<int> labels)
{
	//提取所有样本的haar特征
	TrainData trainData(haarExtractor.GetHaarFeatureNumber());
	for (int i = 0; i < imagePath.size(); i++)
	{
		Mat image = imread(imagePath[i], 0);
		if (!image.data)
			continue;
		vector<float> feats = haarExtractor.GetHaarFeature(image);
		trainData.AddItem(make_pair(feats, labels[i]));
	}
	Train(trainData);
}

void Cascade::TrainWeakClassifier(TrainData& data, vector<WeakClassifier>& weaks)
{
	//训练每一个弱分类器
	for (int j = 0; j < weaks.size(); j++)
	{
		if (weaks[j].bUsed)
		{
			weaks[j].dError = 1.0;
			continue;
		}
		weaks[j].iFeatIdx = j;
		//从所有样本的haar特征中提取相应类型所有的haar特征
		TrainData weakTrainData(1);
		for (int i = 0; i < data.GetSampleNum(); i++)
		{
			//	if (data.fines[i] == 1)continue;
			//只训练标记为被训练的样本
			if (data.toBeTrain[i] != 1)continue;
			float weakFeat = data.trainData[i].first[j];
			int label = data.trainData[i].second;
			vector<float> singleFeat;
			singleFeat.push_back(weakFeat);
			TRAIN_ITEM item = make_pair(singleFeat, label);
			weakTrainData.AddItem(item);
		}
		//对特征进行从小到大排序
		weakTrainData.SortTrainItem();
		//训练弱分类器
		weaks[j].Train(weakTrainData);
	}
}


void Cascade::Train(TrainData& trainData)
{
	//根据haar特征的数量，生成相应数量的弱分类器，每个弱分类器对应一个haar特征
	int iHaarNum = trainData.GetSampleDim();
	weakClassifiers = vector<WeakClassifier>(iHaarNum);
	//训练每一个弱分类器
	//for (int j = 0; j < weakClassifiers.size(); j++)
	//{
	//	weakClassifiers[j].iFeatIdx = j;
	//	//从所有样本的haar特征中提取相应类型所有的haar特征
	//	TrainData weakTrainData(1);
	//	for (int i = 0; i < trainData.GetSampleNum(); i++)
	//	{
	//		float weakFeat = trainData.trainData[i].first[j];
	//		int label = trainData.trainData[i].second;
	//		vector<float> singleFeat;
	//		singleFeat.push_back(weakFeat);
	//		TRAIN_ITEM item = make_pair(singleFeat, label);
	//		weakTrainData.AddItem(item);
	//	}
	//	//对特征进行从小到大排序
	//	weakTrainData.SortTrainItem();
	//	//训练弱分类器
	//	weakClassifiers[j].Train(weakTrainData);
	//}
	//训练强分类器
	//	TrainWeakClassifier(trainData, weakClassifiers);
	for (int i = 0; i < iMaxStageNum; i++)
	{
		//训练弱分类器
		TrainWeakClassifier(trainData, weakClassifiers);
		//对分类器进行排序
		vector<pair<int, float>> weakMap = SortWeakClassifier(weakClassifiers);
		StrongClassifier strongClassifier(&weakClassifiers, weakMap, 0.90, 0.60);
		strongClassifier.Train(trainData);
		strongClassifiers.push_back(strongClassifier);
		cout << "第" << i << "级强分类器训练完成!" << endl;
		strongClassifier.CalculateConfusionMatrix(trainData);
		strongClassifier.PrintConfusionMatrix();
		//测试当前强分类器，将识别正确的样本进行标记，直到所有样本被标记为正确
		//break;
		LabelTrainData(trainData);
	//	break;
		int bContinue = false;
		for (int j = 0; j < trainData.GetSampleNum(); j++)
		{
			if (trainData.toBeTrain[j] != 0 && trainData.trainData[j].second != 1)
			{
				bContinue = true;
				break;
			}
		}
		
		if (!bContinue)
		{
			break;
		}
	}
}

void Cascade::LabelTrainData(TrainData& data)
{
	for (int i = 0; i < data.GetSampleNum(); i++)
	{
		int iRes = Predict(data.trainData[i].first);
		//标记分类正确的正样本与分类错误的负样本，作为下一级的训练样本, 已经标记为0的负样本要跳过
		if (data.trainData[i].second != 1 && data.toBeTrain[i] == 0)
		{
			continue;
		}
		if ((data.trainData[i].second == iRes && data.trainData[i].second == 1) ||
			data.trainData[i].second != iRes && data.trainData[i].second != 1)
		{
			data.toBeTrain[i] = 1;
		}
		else
		{
			data.toBeTrain[i] = 0;
		}
	}
}

//
//void Cascade::Train(TrainData& trainData)
//{
//	//训练弱分类器
//	//1.将特征值从小到大排序
//	trainData.SortTrainItem();
//	//2.生成弱分类器
//	for (int i = 0; i < trainData.trainData.size() - 1; i++)
//	{
//		if (trainData.trainData[i].second != trainData.trainData[i + 1].second)
//		{
//			WeakClassifier wf;
//			wf.SetThreshold( 0.5 * (trainData.trainData[i].first[wf.iFeatIdx] + trainData.trainData[i + 1].first[wf.iFeatIdx]));
//			weakClassifiers.push_back(wf);
//		}
//	}
//	//3.对每个弱分类器，计算其正确率
//	for (int i = 0; i < weakClassifiers.size(); i++)
//	{
//		weakClassifiers[i].TestBunch(trainData);
//		if (weakClassifiers[i].dError > 0.5)
//		{
//			weakClassifiers[i].InvertPolarity();
//			weakClassifiers[i].TestBunch(trainData);
//		}
//	}
//	//4.对分类器进行排序
//	SortWeakClassifier(weakClassifiers);
//	//5.训练强分类器
//	for (int i = 0; i < iMaxStageNum; i++)
//	{
//		StrongClassifier strongClassifier(&weakClassifiers, 0.90, 0.60);
//		strongClassifier.Train(trainData);
//		strongClassifiers.push_back(strongClassifier);
//		cout << "第" << i << "级强分类器训练完成!" << endl;
//		strongClassifier.CalculateConfusionMatrix(trainData);
//		strongClassifier.PrintConfusionMatrix();
//		if (bShouldStop(trainData))
//		{
//			break;
//		}
//	}
//}

vector<pair<int, float>> Cascade::SortWeakClassifier(vector<WeakClassifier>& wfs)
{
	vector<pair<int, float>> weakMap;
	for (int i = 0; i < wfs.size(); i++)
	{
		weakMap.push_back(make_pair(i, wfs[i].dError));
	}
	std::sort(weakMap.begin(), weakMap.end(), CompareWeakMap);
	return weakMap;
//	std::sort(wfs.begin(), wfs.end(), CompareWeakClassifier);
}

int Cascade::Predict(Mat image)
{
	FEATURE feature = haarExtractor.GetHaarFeature(image);
	return Predict(feature);
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

void Cascade::SetTerminateCriteria(int _iMaxStageNum, float _dMinAccuracy, float _dMaxFalseAlarm)
{
	iMaxStageNum = _iMaxStageNum;
	dMinAccuracy = _dMinAccuracy;
	dMaxFalseAlarm = _dMaxFalseAlarm;
}


bool Cascade::bShouldStop(TrainData& trainData)
{
	bool bRes = false;
	if (strongClassifiers.size() < iMaxStageNum)
	{
		//第一行 真正 真负 第二行 假正 假负
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ConfusionMatrix[i][j] = 0;
			}
		}
		for (int i = 0; i < trainData.GetSampleNum(); i++)
		{
			int iRes = Predict(trainData.trainData[i].first);
			if (trainData.trainData[i].second == iRes)
			{
				//真
				if (iRes == 1)
				{
					//正
					ConfusionMatrix[0][0] += 1;
				}
				else
				{
					//负
					ConfusionMatrix[0][1] += 1;
				}
			}
			else
			{
				//假
				if (iRes == 1)
				{
					//正
					ConfusionMatrix[1][0] += 1;
				}
				else
				{
					//负
					ConfusionMatrix[1][1] += 1;
				}
			}
		}
		//计算正确率与虚警率
		float dAccuracy = float(ConfusionMatrix[0][0] + ConfusionMatrix[0][1]) / trainData.trainData.size();
		float dAccuracyThreshold = pow(dMinAccuracy, strongClassifiers.size());
		if (dAccuracy > dAccuracyThreshold)
		{
			bRes = true;
		}
	}
	return bRes;
}

void Cascade::ExtractHaarFeature(vector<string> imagePath, vector<int> labels, TrainData& trainData)
{
	trainData.CreateTrainData(haarExtractor.GetHaarFeatureNumber());
	for (int i = 0; i < imagePath.size(); i++)
	{
		Mat image = imread(imagePath[i], 0);
		if (!image.data)
			continue;
		vector<float> feats = haarExtractor.GetHaarFeature(image);
		trainData.AddItem(make_pair(feats, labels[i]));
	}
}