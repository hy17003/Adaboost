#include "stdafx.h"
#include "StrongClassifier.h"


StrongClassifier::StrongClassifier(std::vector<WeakClassifier>* pWcs)
{
	pWeakClassifiers = pWcs;
	dMinAccuracy = 0.95;
	dMaxFalseAlarm = 0.60;
	dThreshold = 0;
}


StrongClassifier::~StrongClassifier()
{
}

void StrongClassifier::Train(TrainData& trainData)
{
	//��ʼ������Ȩ��
	double w0 = 1.0 / (trainData.iPosNum + trainData.iNegNum);
	for (int i = 0; i < trainData.trainData.size(); i++)
	{
		trainData.weights[i] = w0;
	}
	for (int i = 0; i < pWeakClassifiers->size(); i++)
	{
		WeakClassifier& wclf = (*pWeakClassifiers)[i];
		if (wclf.bUsed)continue;
		//������������ʹ�ø÷��������з��࣬��������ȷ��
		std::vector<int> result(trainData.trainData.size());
		int iRightNum = 0;
		wclf.dError = 0.0;
		for (int j = 0; j < trainData.trainData.size(); j++)
		{
			if (trainData.trainData[j].second == wclf.Predict(trainData.trainData[j].first))
			{
				iRightNum++;
				result[j] = 1;
			}
			else
			{
				wclf.dError += trainData.weights[j];
			}
		}
		//���������ϵ��
		double a = 0.5 * log((1 - wclf.dError )/wclf.dError);
		weakClassiferStream.push_back(std::make_pair(i, a));
		wclf.bUsed = true;
		//��������Ȩֵ
		double Z = 0.0;
		for (int j = 0; j < result.size(); j++)
		{
			if (result[j])
			{
				trainData.weights[j] = trainData.weights[j] * exp(-a);
				Z += trainData.weights[j];
			}
			else
			{
				trainData.weights[j] = trainData.weights[j] * exp(a);
				Z += trainData.weights[j];
			}
		}
		for (int j = 0; j < trainData.weights.size(); j++)
		{
			trainData.weights[j] /= Z;
		}
		//��������
		if (bShouldStop(trainData))
		{
			break;
		}
	}
}

int StrongClassifier::Predict(FEATURE featrue)
{
	int iRes = -1;
	double dScore = 0;
	for (int i = 0; i < weakClassiferStream.size(); i++)
	{
		int idx = weakClassiferStream[i].first;
		double a = weakClassiferStream[i].second;
		dScore += ((*pWeakClassifiers)[idx].Predict(featrue)) * a;
	}
	if (dScore > dThreshold)
	{
		iRes = 1;
	}
	return iRes;
}

void StrongClassifier::SetTerminateCriteria(double _dMinAccuracy, double _dMaxFalseAlarm)
{
	dMinAccuracy = _dMinAccuracy;
	dMaxFalseAlarm = _dMaxFalseAlarm;
}

bool StrongClassifier::bShouldStop(TrainData& trainData)
{
	int bRes = false;
	//��һ�� ���� �渺 �ڶ��� ���� �ٸ�
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ConfusionMatrix[i][j] = 0;
		}
	}
	for (int i = 0; i < trainData.trainData.size(); i++)
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
	double dAccuracy = double(ConfusionMatrix[0][0] + ConfusionMatrix[0][1]) / trainData.trainData.size();
	double dFalseAlarm = double(ConfusionMatrix[1][0]) / trainData.iNegNum;
	if (dAccuracy > dMinAccuracy && dFalseAlarm < dMaxFalseAlarm)
	{
		bRes = true;
	}
	return bRes;
}