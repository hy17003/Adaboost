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
	//��ȡ����������haar����
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
	//ѵ��ÿһ����������
	for (int j = 0; j < weaks.size(); j++)
	{
		if (weaks[j].bUsed)
		{
			weaks[j].dError = 1.0;
			continue;
		}
		weaks[j].iFeatIdx = j;
		//������������haar��������ȡ��Ӧ�������е�haar����
		TrainData weakTrainData(1);
		for (int i = 0; i < data.GetSampleNum(); i++)
		{
			//	if (data.fines[i] == 1)continue;
			//ֻѵ�����Ϊ��ѵ��������
			if (data.toBeTrain[i] != 1)continue;
			float weakFeat = data.trainData[i].first[j];
			int label = data.trainData[i].second;
			vector<float> singleFeat;
			singleFeat.push_back(weakFeat);
			TRAIN_ITEM item = make_pair(singleFeat, label);
			weakTrainData.AddItem(item);
		}
		//���������д�С��������
		weakTrainData.SortTrainItem();
		//ѵ����������
		weaks[j].Train(weakTrainData);
	}
}


void Cascade::Train(TrainData& trainData)
{
	//����haar������������������Ӧ����������������ÿ������������Ӧһ��haar����
	int iHaarNum = trainData.GetSampleDim();
	weakClassifiers = vector<WeakClassifier>(iHaarNum);
	//ѵ��ÿһ����������
	//for (int j = 0; j < weakClassifiers.size(); j++)
	//{
	//	weakClassifiers[j].iFeatIdx = j;
	//	//������������haar��������ȡ��Ӧ�������е�haar����
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
	//	//���������д�С��������
	//	weakTrainData.SortTrainItem();
	//	//ѵ����������
	//	weakClassifiers[j].Train(weakTrainData);
	//}
	//ѵ��ǿ������
	//	TrainWeakClassifier(trainData, weakClassifiers);
	for (int i = 0; i < iMaxStageNum; i++)
	{
		//ѵ����������
		TrainWeakClassifier(trainData, weakClassifiers);
		//�Է�������������
		vector<pair<int, float>> weakMap = SortWeakClassifier(weakClassifiers);
		StrongClassifier strongClassifier(&weakClassifiers, weakMap, 0.90, 0.60);
		strongClassifier.Train(trainData);
		strongClassifiers.push_back(strongClassifier);
		cout << "��" << i << "��ǿ������ѵ�����!" << endl;
		strongClassifier.CalculateConfusionMatrix(trainData);
		strongClassifier.PrintConfusionMatrix();
		//���Ե�ǰǿ����������ʶ����ȷ���������б�ǣ�ֱ���������������Ϊ��ȷ
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
		//��Ƿ�����ȷ����������������ĸ���������Ϊ��һ����ѵ������, �Ѿ����Ϊ0�ĸ�����Ҫ����
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
//	//ѵ����������
//	//1.������ֵ��С��������
//	trainData.SortTrainItem();
//	//2.������������
//	for (int i = 0; i < trainData.trainData.size() - 1; i++)
//	{
//		if (trainData.trainData[i].second != trainData.trainData[i + 1].second)
//		{
//			WeakClassifier wf;
//			wf.SetThreshold( 0.5 * (trainData.trainData[i].first[wf.iFeatIdx] + trainData.trainData[i + 1].first[wf.iFeatIdx]));
//			weakClassifiers.push_back(wf);
//		}
//	}
//	//3.��ÿ��������������������ȷ��
//	for (int i = 0; i < weakClassifiers.size(); i++)
//	{
//		weakClassifiers[i].TestBunch(trainData);
//		if (weakClassifiers[i].dError > 0.5)
//		{
//			weakClassifiers[i].InvertPolarity();
//			weakClassifiers[i].TestBunch(trainData);
//		}
//	}
//	//4.�Է�������������
//	SortWeakClassifier(weakClassifiers);
//	//5.ѵ��ǿ������
//	for (int i = 0; i < iMaxStageNum; i++)
//	{
//		StrongClassifier strongClassifier(&weakClassifiers, 0.90, 0.60);
//		strongClassifier.Train(trainData);
//		strongClassifiers.push_back(strongClassifier);
//		cout << "��" << i << "��ǿ������ѵ�����!" << endl;
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
		//��һ�� ���� �渺 �ڶ��� ���� �ٸ�
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
				//��
				if (iRes == 1)
				{
					//��
					ConfusionMatrix[0][0] += 1;
				}
				else
				{
					//��
					ConfusionMatrix[0][1] += 1;
				}
			}
			else
			{
				//��
				if (iRes == 1)
				{
					//��
					ConfusionMatrix[1][0] += 1;
				}
				else
				{
					//��
					ConfusionMatrix[1][1] += 1;
				}
			}
		}
		//������ȷ�����龯��
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