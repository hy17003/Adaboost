#pragma once
#include "Classifier.h"
class WeakClassifier :
	public Classifier
{
public:
	WeakClassifier();
	~WeakClassifier();
	void Train(TrainData& trainData);
	int Predict(FEATURE feature);
	void TestBunch(TrainData trainData);
	void SetThreshold(float _threshold);
	float GetThreshold();
	void InvertPolarity();
public:
	//��ֵ
	float dThreshold;
	//������
	float dError;
	//����
	bool bPolarity;
	//�Ƿ��ѱ�ĳ��ǿ������ʹ����
	bool bUsed;
	//��Ӧ������ά����ţ���һ��������10��������
	//iFeatIdx = 5�������������ֻ�Ե�5�����������б�
	int iFeatIdx;
};

