#pragma once
#include "HaarExtractor.h"
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
	void InitCascade(int iWidth, int iHeight, float _dMaxFalseAlarm = 0.6, int _iMaxStageNum = 40);
	void ExtractHaarFeature(vector<string> imagePath, vector<int> labels, TrainData& trainData);
	void Train(TrainData& trainData);
	void Train(vector<string> imagePath, vector<int> labels);
	int Predict(FEATURE feature);
	int Predict(Mat image);
	void SetTerminateCriteria(int _iMaxStageNum, float _dMinAccracy, float _dMaxFalseAlarm);
private:
	void TrainWeakClassifier(TrainData& data, vector<WeakClassifier>& weaks);
	vector<pair<int, float>> SortWeakClassifier(vector<WeakClassifier>& wfs);
	bool bShouldStop(TrainData& data);
	void LabelTrainData(TrainData& data);
private:
	TrainData trainData;
	HaarExtractor haarExtractor;
	vector<WeakClassifier> weakClassifiers;
	vector<StrongClassifier> strongClassifiers;
	float dMinAccuracy;
	float dMaxFalseAlarm;
	int iMaxStageNum;
	float ConfusionMatrix[2][2];
	vector<int> wrongIdx;
};

