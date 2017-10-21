#include "inputtypes.h"
#include <map>
#include <set>


bool Person::IsAccountNoValid()
{
	string sCAT = sAccountNo.substr(0, 3);
	string sChecksum = sAccountNo.substr(3, 2);
	string sAccountID = sAccountNo.substr(5, 10);

	bool bOk = (sCAT.compare("CAT") == 0);
	if (bOk)
	{
		int iCheckSum = stoi(sChecksum);
		
		string sAccountIDCAT00 = sAccountID + "CAT00";

		int iLen = sAccountIDCAT00.length();

		int iSum = 0;

		for (int i = 0; i < iLen; ++i)
		{
			char c = sAccountIDCAT00.at(i);

			int ascii = c;
			iSum += ascii;
		}

		int iRest = iSum % 97;
		int iCheck = 98 - iRest;

		bOk = (iCheckSum == iCheck);
	}

	if (bOk)
	{
		map<char, int> countmap;

		int iLen = sAccountID.length();
		for (int i = 0; i < iLen; ++i)
		{
			char c = sAccountID.at(i);

			if(countmap.find(c) == countmap.end())
				countmap[c] = 0;

			if (c >= 65 && c <= 90)
				countmap[c]++;
			else if (c >= 97 && c <= 122)
				countmap[c - 32]--;
		}

		for (auto iter : countmap)
			bOk = bOk && iter.second == 0;
	}

	return bOk;
}

bool Transaction::DoTransaction(Person * pFrom, Person * pTo)
{
	/*bool bOk = (pFrom->iBalance - iAmount >= (-1*pFrom->iOverdraftLimit));

	if (bOk)
	{
		pFrom->iBalance -= iAmount;
		pTo->iBalance += iAmount;
	}

	return bOk;*/ return true;
}

bool Transaction::DoSumInputMatchSumOutput()
{
	int iSumInput = 0;
	int iSumOutput = 0;


	for (auto poutput : vOutputs)
		iSumOutput += poutput->iOutputTransactionAmount;

	for (auto pinput : vInputs)
		iSumInput += pinput->iInputTransactionAmount;

	return (iSumInput == iSumOutput);
}

bool OutputElement::UseForInput(InputElement * pInput)
{
	bool bOk = (pInput->iInputTransactionAmount == iOutputTransactionAmount);

	if (bOk)
		iAmountLeft = 0;

	return bOk;
}

int OutputElement::ProcessRequest(TransactionRequest * pRequest)
{
	int iProcessed = 0;
	bool bOk = (iAmountLeft > 0);
	if (bOk)
	{
		iProcessed = iAmountLeft;
		iAmountLeft = 0;

		if (iProcessed < pRequest->iAmountLeft)
		{
			pRequest->iAmountLeft -= iProcessed;
		}
		else
		{
			pRequest->iAmountLeft = 0;
		}
	}

	return iProcessed;
}
