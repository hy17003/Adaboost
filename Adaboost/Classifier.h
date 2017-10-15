#pragma once
#include <vector>

#define FEATURE double
#define TRAIN_ITEM std::pair<FEATURE, int>

class TrainData
{
public:
	TrainData(){
		iPosNum = 0;
		iNegNum = 0;
	};
	void AddItem(TRAIN_ITEM item)
	{
		trainData.push_back(item);
		weights.push_back(0);
		if (item.second == 1)
		{
			iPosNum++;
		}
		else
		{
			iNegNum++;
		}
	}
public:
	std::vector<TRAIN_ITEM> trainData;
	std::vector<double> weights;
	int iPosNum;
	int iNegNum;
};


//
//typedef struct FEATURE {
//	double* pFeatureValue;
//	int featureNum;
//}FEATURE, *PFEATURE;
//
//typedef struct TRAIN_ITEM {
//	PFEATURE pFeature;
//	int label;
//}TRAIN_ITEM, *PTRAIN_ITEM;
//
//typedef struct TrainData {
//
//}TrainData, *PTrainData;




class Classifier
{
public:
	Classifier();
	~Classifier();
	virtual void Train(TrainData& trainData) = 0;
	virtual int Predict(FEATURE feature) = 0;
};

