#pragma once
#include <vector>
#include "WeakClassifier.h"
#include "Classifier.h"
class StrongClassifier :
	public Classifier
{
public:
	StrongClassifier(std::vector<WeakClassifier>* pWcs, std::vector<std::pair<int, float>>& weakMap, 
		double minAccuracy = 0.95, double maxFalseAlarm = 0.6);
	void SetTerminateCriteria(float _dMinAccuracy, float _dMaxFalseAlarm);
	~StrongClassifier();
	void InitWeight(TrainData& trainData);
	void Train(TrainData& trainData);
	int Predict(FEATURE featrue);
	void CalculateConfusionMatrix(TrainData& trainData);
	void PrintConfusionMatrix();
private:
	bool bShouldStop(TrainData& trainData);
public:
	std::vector<WeakClassifier>* pWeakClassifiers;
	std::vector<std::pair<int, float>> weakMap;
	std::vector<std::pair<int, float>> weakClassiferStream;
	int ConfusionMatrix[2][2];
	int dThreshold;
	float dMinAccuracy;
	float dMaxFalseAlarm;
	int iLayerNum;
};

