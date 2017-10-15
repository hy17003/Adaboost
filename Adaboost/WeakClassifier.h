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
	void SetThreshold(double _threshold);
	void InvertPolarity();
public:
	double dThreshold;
	double dError;
	bool bPolarity;
	bool bUsed;
};

