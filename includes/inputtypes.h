#pragma once
#include <string>
#include <vector>

using namespace std;

class Person
{
public:
	string sName;

	string sAccountNo;
	int iBalance;
	int iOverdraftLimit;

	bool IsAccountNoValid();
};

class TransactionRequest
{
public:
	string sTransactionID;
	string sOwnerFrom;
	string sOwnerTo;
	int iAmount;
	int iAmountLeft;
	long long int iTime;
};

class InputElement
{
public:
	string sInputTransactionID;
	string sInputTransactionOwner;
	int iInputTransactionAmount;
};

class Transaction;
class OutputElement
{
public:
	string sOutputTransactionOwner;
	int iOutputTransactionAmount;
	int iAmountLeft;

	bool UseForInput(InputElement * pInput);

	int ProcessRequest(TransactionRequest * pRequest);

	Transaction * pTransaction;
};

class Transaction
{
public:
	string sID;
	vector<InputElement*> vInputs;
	vector<OutputElement*> vOutputs;

	long long int iTime;

	bool DoTransaction(Person * pFrom, Person * pTo);
	bool DoSumInputMatchSumOutput();
};

class Block
{
public:

	string sBlockID;
	string sPreviousBlockID;

	vector<string> vTransactionIDS;

	long long int iTime;
};

class BlockLink
{
public:
	BlockLink()
	{
		pIn = nullptr;
		pOut = nullptr;
	}
	Block * pIn;
	Block * pOut;
};

class BlockChain
{
public:
	BlockChain()
	{
		Block * pRoot = new Block;
		pRoot->sBlockID = "0b00000000";
		pRoot->sPreviousBlockID = "";
		pRoot->iTime = 0;

		pStart = new BlockLink;
		pStart->pIn = pRoot;
	}
	BlockLink * pStart;

};

