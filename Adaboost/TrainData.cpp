#include "stdafx.h"
#include "TrainData.h"
#include <iostream>

TrainData::TrainData()
{

}


TrainData::~TrainData()
{

}

void TrainData::AddTrainItem(TrainItem& trainItem)
{
	trainItems.push_back()
}

void TrainData::Print()
{
	for (int i = 0; i < iItemNum; i++)
	{
		std::cout << "Item Idx: " << i << " ";
		pTrainItems[i].Print();
	}
}