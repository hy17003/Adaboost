#include "stdafx.h"
#include "WeakClassifier.h"


WeakClassifier::WeakClassifier()
{
	dThreshold = 0;
	dError = 1.0;
	bPolarity = true;
	bUsed = false;
	iFeatIdx = 0;
}


WeakClassifier::~WeakClassifier()
{

}

void WeakClassifier::Train(TrainData& trainData)
{
	std::vector<WeakClassifier> tmpWeakClassifiers;
	for (int i = 0; i < trainData.trainData.size() - 1; i++)
	{
		//如果样本不是这一级要训练的，则跳过
		if (trainData.toBeTrain[i] != 1)continue;
		if (trainData.trainData[i].second != trainData.trainData[i + 1].second)
		{
			WeakClassifier wf;
			wf.SetThreshold(0.5 * (trainData.trainData[i].first[wf.iFeatIdx] 
				+ trainData.trainData[i + 1].first[wf.iFeatIdx]));
			wf.TestBunch(trainData);
			if (wf.dError > 0.5)
			{
				wf.InvertPolarity();
			}
			tmpWeakClassifiers.push_back(wf);
		}
	}
	for (int i = 0; i < tmpWeakClassifiers.size(); i++)
	{
		if (dError > tmpWeakClassifiers[i].dError)
		{
			dError = tmpWeakClassifiers[i].dError;
			dThreshold = tmpWeakClassifiers[i].GetThreshold();
			bPolarity = tmpWeakClassifiers[i].bPolarity;
		}
	}
}

int WeakClassifier::Predict(FEATURE feature)
{
	int res = 1;
	if (bPolarity)
	{
		if (feature[iFeatIdx] >= dThreshold)
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
		if (feature[iFeatIdx] <= dThreshold)
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

void WeakClassifier::SetThreshold(float _threshold)
{
	dThreshold = _threshold;
}


void WeakClassifier::InvertPolarity()
{
	bPolarity = !bPolarity;
	dError = 1.0 - dError;
}

void WeakClassifier::TestBunch(TrainData trainData)
{
	int iRightNum = 0;
	int iWrongNum = 0;
	for (long i = 0; i < trainData.trainData.size(); i++)
	{
		if (trainData.toBeTrain[i] != 1)continue;
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
	dError = (float)iWrongNum / (iRightNum + iWrongNum);
}

float WeakClassifier::GetThreshold()
{
	return dThreshold;
}