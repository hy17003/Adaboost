#pragma once
#include <vector>

class Feature
{
public:
	Feature();
	Feature(std::vector<double> feature);
	Feature(Feature& f);
	void Print();
	~Feature();
	Feature& operator=(const Feature& f);
public:
	double* pValue;
	int iDims;
	int id;
};

