#pragma once
#include "WeakClassifier.h"
#include "StrongClassifier.h"
#include <vector>
#include "Classifier.h"
using namespace std;

class Cascade : public Classifier
{
public:
	Cascade();
	~Cascade();
	void Train(TrainData& trainData);
	void Train(vector<string> imagePath, vector<int> labels);
	int Predict(FEATURE feature);
	void SetTerminateCriteria(int _iMaxStageNum, double _dMinAccracy, double _dMaxFalseAlarm);

private:
	void SortTrainData(TrainData& trainData);
	void SortWeakClassifier(vector<WeakClassifier>& wfs);
private:
	vector<WeakClassifier> weakClassifiers;
	vector<StrongClassifier> strongClassifiers;
	double dMinAccuracy;
	double dMaxFalseAlarm;
	int iMaxStageNum;

};

