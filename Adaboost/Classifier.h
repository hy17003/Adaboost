#pragma once
#include "TrainData.h"



class Classifier
{
public:
	Classifier();
	~Classifier();
	virtual void Train(TrainData& trainData) = 0;
	virtual int Predict(FEATURE feature) = 0;
};

