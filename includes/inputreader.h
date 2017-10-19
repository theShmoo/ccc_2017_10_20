#pragma once

#include <vector>
using namespace std;

class InputReader
{
public:
	struct Input
	{
		int x;
		int y;
	};

	InputReader();
	~InputReader() = default;

	void Load(vector<Input> &rvInput);
private:
	static vector<string> m_vInputFiles;

};