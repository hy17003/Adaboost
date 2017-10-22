#pragma once
#include <vector>

#define FEATURE std::vector<float>
#define TRAIN_ITEM std::pair<FEATURE, int>

class TrainData
{
public:
	TrainData();
	~TrainData();
	TrainData(int featDims);
	void CreateTrainData(int featDim);
	void AddItem(TRAIN_ITEM item);
	void SortTrainItem();
	int GetSampleNum();
	int GetSampleDim();
	int Save(std::string filename);
	int Load(std::string filename);

public:
	std::vector<TRAIN_ITEM> trainData;
	std::vector<float> weights;
	std::vector<int> toBeTrain;
	int iPosNum;
	int iNegNum;
	int iDims;
};

