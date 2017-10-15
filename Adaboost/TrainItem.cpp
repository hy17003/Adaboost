#include "stdafx.h"
#include "TrainItem.h"
#include <iostream>


TrainItem::TrainItem(Feature _feature, int _label)
{
	feature = _feature;
	label = _label;
}


TrainItem::TrainItem()
{
	
}


TrainItem::~TrainItem()
{
}

void TrainItem::Print()
{
	std::cout << "label: " << label;
	feature.Print();
}