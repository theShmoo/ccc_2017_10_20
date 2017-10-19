// inputreader.cpp
// Read the input from the input directory

#include "inputreader.h"
#include "stdafx.h"

vector<string> InputReader::m_vInputFiles = {
	"input/inputfile.txt"
};

InputReader::InputReader()
{
}

void InputReader::Load(vector<Input> &rvInput)
{
	cout << "Read input files: " << m_vInputFiles.size() << endl;
	for (auto input : m_vInputFiles)
	{
		cout << "Read: " << input << endl;
		// open file stream
		ifstream infile(input);

		// read in every line
		string line;
		while (std::getline(infile, line))
		{
			istringstream iss(line);
			int a, b;
			if (!(iss >> a >> b))
			{
				// error loading two ints
				cout << "error loading input" << endl;
				break;
			}

			Input i;
			i.x = a;
			i.y = b;
			rvInput.push_back(i);
		}
	}
}

