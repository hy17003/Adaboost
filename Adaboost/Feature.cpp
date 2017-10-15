#include "stdafx.h"
#include "Feature.h"
#include <iostream>

static int n = 0;

Feature::Feature() :
	iDims(0),
	pValue(0)
{
	n++;
	id = n;
	std::cout << this << " create... id = " << id  << std::endl;
}


Feature::~Feature()
{
	if (pValue != 0)
	{
		delete[] pValue;
		pValue = 0;
	}
	std::cout << this << " destroy... id = "<< id << std::endl;
}

Feature::Feature(Feature& f)
{
	n++;
	id = n;
	iDims = f.iDims;
	pValue = new double[iDims];
	memcpy(pValue, f.pValue, sizeof(double) * iDims);
	std::cout << this << " create...id = " << id << std::endl;
}

Feature& Feature::operator=(const Feature& f)
{
	if (pValue != 0)
	{
		delete[] pValue;
		pValue = 0;
	}
	iDims = f.iDims;
	pValue = new double[iDims];
	memcpy(pValue, f.pValue, sizeof(double) * iDims);
	return *this;
}

Feature::Feature(std::vector<double> feature)
{
	n++;
	id = n;
	if (pValue != 0)
	{
		delete[] pValue;
		pValue = 0;
	}
	iDims = feature.size();
	pValue = new double[iDims];
	for (int i = 0; i < iDims; i++)
	{
		pValue[i] = feature[i];
	}
	std::cout << this << " create... id = " << id << std::endl;
}

void Feature::Print()
{
	std::cout << "feature num: " << iDims << " value: ";
	for (int i = 0; i < iDims; i++)
	{
		std::cout << pValue[i] << " ";
	}
	std::cout << std::endl;
}