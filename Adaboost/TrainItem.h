#pragma once
#include "Feature.h"

class TrainItem
{
public:
	TrainItem(Feature _feature, int _label);
	TrainItem();
	~TrainItem();
	void Print();
public:
	Feature feature;
	int label;
};

