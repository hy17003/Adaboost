#pragma once
#include <vector>
#include "WeakClassifier.h"
#include "Classifier.h"
class StrongClassifier :
	public Classifier
{
public:
	StrongClassifier(std::vector<WeakClassifier>* pWcs);
	void SetTerminateCriteria(double _dMinAccuracy, double _dMaxFalseAlarm);
	~StrongClassifier();
	void Train(TrainData& trainData);
	int Predict(FEATURE featrue);
private:
	bool bShouldStop(TrainData& trainData);
public:
	std::vector<WeakClassifier>* pWeakClassifiers;
	std::vector<std::pair<int, double>> weakClassiferStream;
	int ConfusionMatrix[2][2];
	int dThreshold;
	double dMinAccuracy;
	double dMaxFalseAlarm;
};

