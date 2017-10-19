// ccc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "inputreader.h"

int main()
{
	InputReader reader;
	vector<InputReader::Input> input;
	reader.Load(input);
	for (auto i : input)
	{
		cout << i.x << " " << i.y << endl;
	}
	
    return 0;
}

