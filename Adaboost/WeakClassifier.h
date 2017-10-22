#pragma once
#include "Classifier.h"
class WeakClassifier :
	public Classifier
{
public:
	WeakClassifier();
	~WeakClassifier();
	void Train(TrainData& trainData);
	int Predict(FEATURE feature);
	void TestBunch(TrainData trainData);
	void SetThreshold(float _threshold);
	float GetThreshold();
	void InvertPolarity();
public:
	//阈值
	float dThreshold;
	//错误率
	float dError;
	//极性
	bool bPolarity;
	//是否已被某个强分类器使用了
	bool bUsed;
	//对应的特征维度序号，如一个样本有10个特征，
	//iFeatIdx = 5，则该弱分类器只对第5个特征进行判别
	int iFeatIdx;
};

