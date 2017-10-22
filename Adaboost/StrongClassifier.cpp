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

//输入为训练数据及排序后的弱分类器序号-误差表
void StrongClassifier::Train(TrainData& trainData)
{
	//初始化样本权重
	InitWeight(trainData);
	for (int i = 0; i < weakMap.size(); i++)
	{
		int weakIdx = weakMap[i].first;
		WeakClassifier& wclf = (*pWeakClassifiers)[weakIdx];
		if (wclf.bUsed)continue;
		//对所有样本，使用该分类器进行分类，计算其正确率
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
		//计算分类器系数
		float a = 1.0;
		if (wclf.dError > 0)
		{
			a = 0.5 * log((1 - wclf.dError) / wclf.dError);
			//将弱分类器对应的特征序号及对应的分类器系数保存到弱分类器流中
			weakClassiferStream.push_back(std::make_pair(weakIdx, a));
			wclf.bUsed = true;
		}
		else
		{
			//将弱分类器对应的特征序号及对应的分类器系数保存到弱分类器流中
			weakClassiferStream.push_back(std::make_pair(weakIdx, a));
			wclf.bUsed = true;
			break;
		}
		//a = 0.5 * log((1 - wclf.dError) / wclf.dError);
		////将弱分类器对应的特征序号及对应的分类器系数保存到弱分类器流中
		//weakClassiferStream.push_back(std::make_pair(weakIdx, a));
		//wclf.bUsed = true;
		//更新样本权值
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
		//结束条件
		if (bShouldStop(trainData))
		{
			break;
		}
	}


	//for (int i = 0; i < pWeakClassifiers->size(); i++)
	//{
	//	WeakClassifier& wclf = (*pWeakClassifiers)[i];
	//	if (wclf.bUsed)continue;
	//	//对所有样本，使用该分类器进行分类，计算其正确率
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
	//	//计算分类器系数
	//	float a = 0.0;
	//	a = 0.5 * log((1 - wclf.dError) / wclf.dError);
	//	//将弱分类器对应的特征序号及对应的分类器系数保存到弱分类器流中
	//	weakClassiferStream.push_back(std::make_pair(wclf.iFeatIdx, a));
	//	wclf.bUsed = true;
	//	//更新样本权值
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
	//	//结束条件
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
//	std::cout << "本轮通过的序号：" << std::endl;
	//第一行 真正 真负 第二行 假正 假负
	for (int i = 0; i < trainData.trainData.size(); i++)
	{
		if (trainData.toBeTrain[i] != 1)continue;
		int iRes = Predict(trainData.trainData[i].first);
		if (trainData.trainData[i].second == iRes)
		{
			
			//真
			if (iRes == 1)
			{
		//		std::cout << "正样本:" << i << std::endl;
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
		//		std::cout << "负样本:" << i << std::endl;
				ConfusionMatrix[1][0] += 1;
			}
			else
			{
				//负
				ConfusionMatrix[1][1] += 1;
			}
		}
	}
}

void StrongClassifier::PrintConfusionMatrix()
{
	std::cout << "\t真\t假" << std::endl;
	std::cout << "正\t" << ConfusionMatrix[0][0] << "\t" << ConfusionMatrix[1][0] << std::endl;
	std::cout << "负\t" << ConfusionMatrix[0][1] << "\t" << ConfusionMatrix[1][1] << std::endl;
	std::cout << std::endl;
}

bool StrongClassifier::bShouldStop(TrainData& trainData)
{
	bool bRes = false;
	CalculateConfusionMatrix(trainData);
	//计算正确率与虚警率
	float dAccuracy = float(ConfusionMatrix[0][0] + ConfusionMatrix[0][1]) /
		(ConfusionMatrix[0][0] + ConfusionMatrix[0][1] + ConfusionMatrix[1][0] + ConfusionMatrix[1][1]);
	float dFalseAlarm = float(ConfusionMatrix[1][0]) / (ConfusionMatrix[0][1] + ConfusionMatrix[1][1]);
	if (dAccuracy > dMinAccuracy && dFalseAlarm < dMaxFalseAlarm)
	{
		bRes = true;
	}
	return bRes;
}