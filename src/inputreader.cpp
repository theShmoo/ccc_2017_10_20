// inputreader.cpp
// Read the input from the input directory

#include "inputreader.h"
#include "stdafx.h"

vector<string> InputReader::m_vInputFiles = {
	"input/level4-eg.txt", "input/level4-1.txt",	"input/level4-2.txt",	"input/level4-3.txt",	"input/level4-4.txt"
};

InputReader::InputReader()
{
}

void InputReader::Load(string sInputFile, vector<Person*> &rvPersons, vector<Transaction*> &rvTransactions, vector<Block*> &rvBlocks)
{
	cout << "Read: " << sInputFile << endl;
	// open file stream
	ifstream infile(sInputFile);

	// read in every line
	string line;

	/*int iPeople = 0;
	getline(infile, line);
	istringstream iss(line);

	iss >> iPeople;

	for (int i = 0; i < iPeople; ++i)
	{
		getline(infile, line);
		istringstream issp(line);

		Person * pPerson = new Person();
		issp >> pPerson->sName;
		issp >> pPerson->sAccountNo;
		issp >> pPerson->iBalance;
		issp >> pPerson->iOverdraftLimit;

		if(pPerson->IsAccountNoValid())
			rvPersons.push_back(pPerson);
	}*/

	getline(infile, line);
	istringstream iss2(line);

	int iTransactions;

	iss2 >> iTransactions;

	for (int i = 0; i < iTransactions; ++i)
	{
		getline(infile, line);
		istringstream isst(line);

		Transaction * pTrans = new Transaction;

		isst >> pTrans->sID;

		int iNumInputs = 0;
		isst >> iNumInputs;

		for (int j = 0; j < iNumInputs; ++j)
		{
			InputElement * pInput = new InputElement;
			isst >> pInput->sInputTransactionID;
			isst >> pInput->sInputTransactionOwner;
			isst >> pInput->iInputTransactionAmount;

			pTrans->vInputs.push_back(pInput);
		}

		int iNumOutputs = 0;
		isst >> iNumOutputs;

		for (int j = 0; j < iNumOutputs; ++j)
		{
			OutputElement * pOutput = new OutputElement;
			isst >> pOutput->sOutputTransactionOwner;
			isst >> pOutput->iOutputTransactionAmount;
			pOutput->iAmountLeft = pOutput->iOutputTransactionAmount;

			pOutput->pTransaction = pTrans;

			pTrans->vOutputs.push_back(pOutput);
		}

		isst >> pTrans->iTime;

		//cout << pTrans->sFrom << pTrans->sTo << pTrans->iBalance << " " << pTrans->iTime << endl;

		rvTransactions.push_back(pTrans);
	}

	/*getline(infile, line);
	istringstream iss3(line);

	int iTransactionsRequests = 0;

	iss3 >> iTransactionsRequests;

	for(int i = 0; i < iTransactionsRequests; ++i)
	{ 
		TransactionRequest * pReq = new TransactionRequest;

		getline(infile, line);
		istringstream issr(line);

		issr >> pReq->sTransactionID;
		issr >> pReq->sOwnerFrom;
		issr >> pReq->sOwnerTo;
		issr >> pReq->iAmount;
		issr >> pReq->iTime;

		pReq->iAmountLeft = pReq->iAmount;

		rvRequests.push_back(pReq);
	}*/

	getline(infile, line);
	istringstream iss3(line);

	int iNumBlocks = 0;

	iss3 >> iNumBlocks;

	for (int i = 0; i < iNumBlocks; ++i)
	{
		Block * b = new Block;

		getline(infile, line);
		istringstream issr(line);

		issr >> b->sBlockID;
		issr >> b->sPreviousBlockID;
		
		int iNumTrans = 0;
		issr >> iNumTrans;

		for (int a = 0; a < iNumTrans; ++a)
		{
			string sID;
			issr >> sID;

			b->vTransactionIDS.push_back(sID);
		}
		issr >> b->iTime;

		rvBlocks.push_back(b);
	}
}

