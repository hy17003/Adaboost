#include "stdafx.h"
#include "TrainData.h"
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <fstream>
using namespace std;

bool CompareTrainItem(TRAIN_ITEM& ti1, TRAIN_ITEM& ti2)
{
	float v1 = 0.0, v2 = 0.0;
	for (int i = 0; i < ti1.first.size(); i++)
	{
		v1 += ti1.first[i];
	}
	for (int i = 0; i < ti2.first.size(); i++)
	{
		v2 += ti2.first[i];
	}
	return v1 < v2;
}

TrainData::TrainData()
{
	iPosNum = 0;
	iNegNum = 0;
	iDims = 1;
}

void TrainData::CreateTrainData(int featDims)
{
	iPosNum = 0;
	iNegNum = 0;
	iDims = featDims;
}

TrainData::TrainData(int featDims)
{
	iPosNum = 0;
	iNegNum = 0;
	iDims = featDims;
};



TrainData::~TrainData()
{

}



void TrainData::SortTrainItem()
{
	sort(trainData.begin(), trainData.end(), CompareTrainItem);
}

void TrainData::AddItem(TRAIN_ITEM item)
{
	assert(item.first.size() == iDims);
	trainData.push_back(item);
	weights.push_back(0);
	toBeTrain.push_back(1);
	
	if (item.second == 1)
	{
		iPosNum++;
	}
	else
	{
		iNegNum++;
	}
}

int TrainData::GetSampleNum()
{
	return trainData.size();
}
int TrainData::GetSampleDim()
{
	return iDims;
}


/*
std::vector<TRAIN_ITEM> trainData;
std::vector<float> weights;
int iPosNum;
int iNegNum;
int iDims
*/
int TrainData::Save(std::string filename)
{
	FILE *pFile = fopen(filename.c_str(), "wb");
	if (NULL == pFile)
	{
		cout << "Failed to open file to write!" << endl;
		return -1;
	}
	//iPosNum
	fwrite(&iPosNum, sizeof(int), 1, pFile);
	//iNegNum
	fwrite(&iNegNum, sizeof(int), 1, pFile);
	//iDims
	fwrite(&iDims, sizeof(int), 1, pFile);
	//trainData
	int iTrainSize = trainData.size();
	fwrite(&iTrainSize, sizeof(int), 1, pFile);
	for (int i = 0; i < iTrainSize; i++)
	{
		//保存特征
		for (int j = 0; j < iDims; j++)
		{
			fwrite(&(trainData[i].first[j]), sizeof(float), 1, pFile);
		}
		//保存标签
		fwrite(&(trainData[i].second), sizeof(int), 1, pFile);
		//保存权重
		fwrite(&(weights[i]), sizeof(float), 1, pFile);
		//保存fine标记
		fwrite(&(toBeTrain[i]), sizeof(int), 1, pFile);
	}
	fclose(pFile);
	return 0;
}

int TrainData::Load(std::string filename)
{
	FILE *pFile = fopen(filename.c_str(), "rb");
	if (NULL == pFile)
	{
		cout << "Failed to open file to read!" << endl;
		return -1;
	}
	//iPosNum
	fread(&iPosNum, sizeof(int), 1, pFile);
	//iNegNum
	fread(&iNegNum, sizeof(int), 1, pFile);
	//iDims
	fread(&iDims, sizeof(int), 1, pFile);
	//trainData
	int iTrainSize;
	fread(&iTrainSize, sizeof(int), 1, pFile);
	for (int i = 0; i < iTrainSize; i++)
	{
		TRAIN_ITEM item;
		//读取特征
		for (int j = 0; j < iDims; j++)
		{
			float value;
			fread(&value, sizeof(float), 1, pFile);
			item.first.push_back(value);
		}
		//读取标签
		int label = 0;
		fread(&label, sizeof(int), 1, pFile);
		item.second = label;

		trainData.push_back(item);
		//读取权重
		float weight = 0.0;
		fread(&weight, sizeof(float), 1, pFile);
		weights.push_back(weight);
		//读取fine标记
		int toTrain = 0;
		fread(&toTrain, sizeof(int), 1, pFile);
		toBeTrain.push_back(toTrain);
	}
	fclose(pFile);
	return 0;
}