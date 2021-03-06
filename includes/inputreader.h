#pragma once

#include <vector>
#include <memory>
#include "inputtypes.h"
using namespace std;

class InputReader
{
public:
	InputReader();
	~InputReader() = default;

	void Load(string sInputFile, vector<Person*> &rvPersons, vector<Transaction*> &rvTransactions, vector<Block*> &rvRequests);

	static vector<string> m_vInputFiles;
};