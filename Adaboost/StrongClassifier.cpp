#include "stdafx.h"
#include "StrongClassifier.h"
#include <iostream>


StrongClassifier::StrongClassifier(std::vector<WeakClassifier>* pWcs, std::vector<std::pair<int, float>>& _weakMap, 
	double minAccuracy, double maxFalseAlarm)
{
	pWeakClassifiers = pWcs;
	dMinAccuracy = minAccuracy;
	dMaxFalseAlarm = maxFalseAlarm;
	dThreshold = 0;
	weakMap = _weakMap;
}


StrongClassifier::~StrongClassifier()
{
}

void StrongClassifier::InitWeight(TrainData& trainData)
{
	int iPN = 0, iNN = 0;
	for (int i = 0; i < trainData.trainData.size(); i++)
	{
		if (!trainData.toBeTrain[i])
		{
			continue;
		}
		if (trainData.trainData[i].second == 1)
		{
			iPN++;
		}
		else
		{
			iNN++;
		}
	}
	for (int i = 0; i < trainData.trainData.size(); i++)
	{
		if (!trainData.toBeTrain[i])
		{
			trainData.weights[i] = 0.0;
			continue;
		}
		if (trainData.trainData[i].second == 1)
		{
			trainData.weights[i] = 1.0 / (2 * iPN);
		}
		else
		{
			trainData.weights[i] = 1.0 / (2 * iNN);
		}
	}
}

//����Ϊѵ�����ݼ������������������-����
void StrongClassifier::Train(TrainData& trainData)
{
	//��ʼ������Ȩ��
	InitWeight(trainData);
	for (int i = 0; i < weakMap.size(); i++)
	{
		int weakIdx = weakMap[i].first;
		WeakClassifier& wclf = (*pWeakClassifiers)[weakIdx];
		if (wclf.bUsed)continue;
		//������������ʹ�ø÷��������з��࣬��������ȷ��
		std::vector<int> result(trainData.trainData.size());
		int iRightNum = 0;
		wclf.dError = 0.0;
		for (int j = 0; j < trainData.trainData.size(); j++)
		{
			//std::cout << trainData.trainData[j].first[wclf.iFeatIdx] << " " << trainData.trainData[j].second << std::endl;
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
		float a = 1.0;
		if (wclf.dError > 0)
		{
			a = 0.5 * log((1 - wclf.dError) / wclf.dError);
			//������������Ӧ��������ż���Ӧ�ķ�����ϵ�����浽������������
			weakClassiferStream.push_back(std::make_pair(weakIdx, a));
			wclf.bUsed = true;
		}
		else
		{
			//������������Ӧ��������ż���Ӧ�ķ�����ϵ�����浽������������
			weakClassiferStream.push_back(std::make_pair(weakIdx, a));
			wclf.bUsed = true;
			break;
		}
		//a = 0.5 * log((1 - wclf.dError) / wclf.dError);
		////������������Ӧ��������ż���Ӧ�ķ�����ϵ�����浽������������
		//weakClassiferStream.push_back(std::make_pair(weakIdx, a));
		//wclf.bUsed = true;
		//��������Ȩֵ
		float Z = 0.0;
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


	//for (int i = 0; i < pWeakClassifiers->size(); i++)
	//{
	//	WeakClassifier& wclf = (*pWeakClassifiers)[i];
	//	if (wclf.bUsed)continue;
	//	//������������ʹ�ø÷��������з��࣬��������ȷ��
	//	std::vector<int> result(trainData.trainData.size());
	//	int iRightNum = 0;
	//	wclf.dError = 0.0;
	//	for (int j = 0; j < trainData.trainData.size(); j++)
	//	{
	//		//std::cout << trainData.trainData[j].first[wclf.iFeatIdx] << " " << trainData.trainData[j].second << std::endl;
	//		if (trainData.trainData[j].second == wclf.Predict(trainData.trainData[j].first))
	//		{
	//			iRightNum++;
	//			result[j] = 1;
	//		}
	//		else
	//		{
	//			wclf.dError += trainData.weights[j];
	//		}
	//	}
	//	//���������ϵ��
	//	float a = 0.0;
	//	a = 0.5 * log((1 - wclf.dError) / wclf.dError);
	//	//������������Ӧ��������ż���Ӧ�ķ�����ϵ�����浽������������
	//	weakClassiferStream.push_back(std::make_pair(wclf.iFeatIdx, a));
	//	wclf.bUsed = true;
	//	//��������Ȩֵ
	//	float Z = 0.0;
	//	for (int j = 0; j < result.size(); j++)
	//	{
	//		if (result[j])
	//		{
	//			trainData.weights[j] = trainData.weights[j] * exp(-a);
	//			Z += trainData.weights[j];
	//		}
	//		else
	//		{
	//			trainData.weights[j] = trainData.weights[j] * exp(a);
	//			Z += trainData.weights[j];
	//		}
	//	}
	//	for (int j = 0; j < trainData.weights.size(); j++)
	//	{
	//		trainData.weights[j] /= Z;
	//	}
	//	//��������
	//	if (bShouldStop(trainData))
	//	{
	//		break;
	//	}
	//}
	iLayerNum = weakClassiferStream.size();
}

int StrongClassifier::Predict(FEATURE featrue)
{
	int iRes = -1;
	float dScore = 0;
	for (int i = 0; i < weakClassiferStream.size(); i++)
	{
		int idx = weakClassiferStream[i].first;
		float a = weakClassiferStream[i].second;
		dScore += ((*pWeakClassifiers)[idx].Predict(featrue)) * a;
	}
	if (dScore > dThreshold)
	{
		iRes = 1;
	}
	return iRes;
}

void StrongClassifier::SetTerminateCriteria(float _dMinAccuracy, float _dMaxFalseAlarm)
{
	dMinAccuracy = _dMinAccuracy;
	dMaxFalseAlarm = _dMaxFalseAlarm;
}

void StrongClassifier::CalculateConfusionMatrix(TrainData& trainData)
{
	ConfusionMatrix[0][0] = 0;
	ConfusionMatrix[0][1] = 0;
	ConfusionMatrix[1][0] = 0;
	ConfusionMatrix[1][1] = 0;
//	std::cout << "����ͨ������ţ�" << std::endl;
	//��һ�� ���� �渺 �ڶ��� ���� �ٸ�
	for (int i = 0; i < trainData.trainData.size(); i++)
	{
		if (trainData.toBeTrain[i] != 1)continue;
		int iRes = Predict(trainData.trainData[i].first);
		if (trainData.trainData[i].second == iRes)
		{
			
			//��
			if (iRes == 1)
			{
		//		std::cout << "������:" << i << std::endl;
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
		//		std::cout << "������:" << i << std::endl;
				ConfusionMatrix[1][0] += 1;
			}
			else
			{
				//��
				ConfusionMatrix[1][1] += 1;
			}
		}
	}
}

void StrongClassifier::PrintConfusionMatrix()
{
	std::cout << "\t��\t��" << std::endl;
	std::cout << "��\t" << ConfusionMatrix[0][0] << "\t" << ConfusionMatrix[1][0] << std::endl;
	std::cout << "��\t" << ConfusionMatrix[0][1] << "\t" << ConfusionMatrix[1][1] << std::endl;
	std::cout << std::endl;
}

bool StrongClassifier::bShouldStop(TrainData& trainData)
{
	bool bRes = false;
	CalculateConfusionMatrix(trainData);
	//������ȷ�����龯��
	float dAccuracy = float(ConfusionMatrix[0][0] + ConfusionMatrix[0][1]) /
		(ConfusionMatrix[0][0] + ConfusionMatrix[0][1] + ConfusionMatrix[1][0] + ConfusionMatrix[1][1]);
	float dFalseAlarm = float(ConfusionMatrix[1][0]) / (ConfusionMatrix[0][1] + ConfusionMatrix[1][1]);
	if (dAccuracy > dMinAccuracy && dFalseAlarm < dMaxFalseAlarm)
	{
		bRes = true;
	}
	return bRes;
}