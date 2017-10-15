#pragma once
#include "TrainItem.h"
#include <vector>
class TrainData
{
public:
	TrainData();
	~TrainData();
	void AddTrainItem(TrainItem& trainItem);
	void Print();
public:
	std::vector<TrainItem> trainItems;
private:
	int iSizeStep;
};

