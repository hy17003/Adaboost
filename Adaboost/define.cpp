#include "stdafx.h"
#include "define.h"


void readTxtToVector(string filename, vector<string>* pImagePath, vector<int>* pLabel)
{
	pImagePath->clear();
	if (pLabel)
	{
		pLabel->clear();
	}
	fstream file(filename, std::fstream::in);
	if (!file.is_open())
	{
		std::cout << "Can not open file!" << std::endl;
		return;
	}
	string line;
	while (getline(file, line))
	{
		int pos = line.find_last_of(' ');
		string path = line.substr(0, pos);
		string strlabel = line.substr(pos + 1, line.length() - pos);
		int label = atoi(strlabel.c_str());
		pImagePath->push_back(path);
		if (pLabel)
		{
			pLabel->push_back(label);
		}
	}
	file.close();
}
