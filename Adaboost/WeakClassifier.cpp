#include "stdafx.h"
#include "WeakClassifier.h"


WeakClassifier::WeakClassifier()
{
	dThreshold = 0;
	dError = 1.0;
	bPolarity = true;
	bUsed = false;
}


WeakClassifier::~WeakClassifier()
{

}

void WeakClassifier::Train(TrainData& trainData)
{

}

int WeakClassifier::Predict(FEATURE feature)
{
	int res = 1;
	if (bPolarity)
	{
		if (feature >= dThreshold)
		{
			res = 1;
		}
		else
		{
			res = -1;
		}
	}
	else
	{
		if (feature <= dThreshold)
		{
			res = 1;
		}
		else
		{
			res = -1;
		}
	}
	return res;
}

void WeakClassifier::SetThreshold(double _threshold)
{
	dThreshold = _threshold;
}


void WeakClassifier::InvertPolarity()
{
	bPolarity = !bPolarity;
}

void WeakClassifier::TestBunch(TrainData trainData)
{
	int iRightNum = 0;
	int iWrongNum = 0;
	for (long i = 0; i < trainData.trainData.size(); i++)
	{
		TRAIN_ITEM& item = trainData.trainData[i];
		if (item.second == Predict(item.first))
		{
			iRightNum++;
		}
		else
		{
			iWrongNum++;
		}
	}
	dError = (double)iWrongNum / (iRightNum + iWrongNum);
}