// ccc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "inputreader.h"
#include "inputtypes.h"
#include <set>


int main()
{
	InputReader reader;
	cout << "Read input files: " << reader.m_vInputFiles.size() << endl;
	for (auto inputFile : reader.m_vInputFiles)
	{
		vector<Person *> vPeople;
		vector<Transaction*> vTransactions;
		vector<TransactionRequest*> vRequests;
		vector<Block*> vBlocks;
		reader.Load(inputFile, vPeople, vTransactions, vBlocks);

		map<string, Person*> mapPersons;
		for (auto iter = vPeople.begin(); iter != vPeople.end(); ++iter)
			mapPersons[(*iter)->sAccountNo] = (*iter);

		map<long long int, Transaction*> mapTransactions;
		for (auto pTrans : vTransactions)
			mapTransactions[pTrans->iTime] = pTrans;

		vector<OutputElement*> vPreviousOutputs;

		vector<Transaction*> correctTransactions;

		for (auto transaction : mapTransactions)
		{
			bool bSumsMatch = transaction.second->DoSumInputMatchSumOutput();
			if (bSumsMatch)
			{
				set<OutputElement*> setUsedOutputsForThisTransaction;
				bool bInputsOk = true;
				for (auto pinput : transaction.second->vInputs)
				{
					if (bInputsOk)
					{
						if (pinput->sInputTransactionOwner.compare("origin") == 0)
						{
							bInputsOk = true;
						}
						else {
							bool bPreviousOutputFound = false;
							for (auto previousiter = vPreviousOutputs.begin(); (!bPreviousOutputFound) && previousiter != vPreviousOutputs.end(); ++previousiter)
							{
								bPreviousOutputFound = pinput->sInputTransactionOwner.compare((*previousiter)->sOutputTransactionOwner) == 0;
								bPreviousOutputFound = bPreviousOutputFound && (pinput->iInputTransactionAmount == (*previousiter)->iOutputTransactionAmount);

								bPreviousOutputFound = bPreviousOutputFound && (setUsedOutputsForThisTransaction.find(*previousiter) == setUsedOutputsForThisTransaction.end());

								if (bPreviousOutputFound)
								{
									setUsedOutputsForThisTransaction.insert((*previousiter));
								}
							}
							
							
							bInputsOk = bPreviousOutputFound;
						}
					}

					bInputsOk = bInputsOk && (pinput->iInputTransactionAmount > 0);
				}

				if (bInputsOk)
				{
					set<string> vUniqueOutputs;
					bool bOutputsOk = true;
					for (auto poutput : transaction.second->vOutputs)
					{
						vUniqueOutputs.insert(poutput->sOutputTransactionOwner);
						bOutputsOk = bOutputsOk && (poutput->iOutputTransactionAmount > 0);
					}

					bOutputsOk = bOutputsOk && vUniqueOutputs.size() == transaction.second->vOutputs.size();
					if (bOutputsOk)
					{
						correctTransactions.push_back(transaction.second);

						for (auto pinput : transaction.second->vInputs)
						{
							auto previousoutput = vPreviousOutputs.begin();

							bool bFound = false;
							while (!bFound && previousoutput != vPreviousOutputs.end())
							{
								bFound = pinput->sInputTransactionOwner.compare((*previousoutput)->sOutputTransactionOwner) == 0;
								bFound = bFound && pinput->iInputTransactionAmount == (*previousoutput)->iOutputTransactionAmount;

								if (!bFound)
									++previousoutput;

							}

							if (bFound)
							{
								(*previousoutput)->UseForInput(pinput);
								vPreviousOutputs.erase(previousoutput);
							}
						}

						for (auto poutput : transaction.second->vOutputs)
						{
							vPreviousOutputs.push_back(poutput);
						}
					}
				}
			}
		}

		map<long long int, TransactionRequest*> mapRequests;
		for (auto r : vRequests)
			mapRequests[r->iTime] = r;

		for (auto r : mapRequests)
		{
			int iWhatGoes = 0;
			for (auto o : vPreviousOutputs)
			{
				if (o->sOutputTransactionOwner.compare(r.second->sOwnerFrom) == 0)
				{
					iWhatGoes += o->iAmountLeft;
				}
			}

			if (iWhatGoes >= r.second->iAmount)
			{
				Transaction * pResultingTransaction = new Transaction;
				pResultingTransaction->sID = r.second->sTransactionID;
				pResultingTransaction->iTime = r.second->iTime;

				for (auto o : vPreviousOutputs)
				{
					if (r.second->iAmountLeft > 0)
					{
						if (o->sOutputTransactionOwner.compare(r.second->sOwnerFrom) == 0)
						{
							if (o->iAmountLeft > 0)
							{
								int iProcessed = o->ProcessRequest(r.second);
								if (iProcessed > 0)
								{
									InputElement * pInputElement = new InputElement;
									pInputElement->sInputTransactionID = o->pTransaction->sID;
									pInputElement->iInputTransactionAmount = iProcessed;
									pInputElement->sInputTransactionOwner = r.second->sOwnerFrom;

									pResultingTransaction->vInputs.push_back(pInputElement);
								}
							}
						}
					}
				}

				int iInInput = 0;
				for (auto i : pResultingTransaction->vInputs)
				{
					iInInput += i->iInputTransactionAmount;
				}

				if (iInInput > 0)
				{
					OutputElement * pOutput1 = new OutputElement;
					pOutput1->sOutputTransactionOwner = r.second->sOwnerTo;
					pOutput1->iOutputTransactionAmount = r.second->iAmount;
					pOutput1->iAmountLeft = pOutput1->iOutputTransactionAmount;
					pOutput1->pTransaction = pResultingTransaction;

					pResultingTransaction->vOutputs.push_back(pOutput1);
					vPreviousOutputs.push_back(pOutput1);

					int iRest = iInInput - r.second->iAmount;

					if (iRest > 0)
					{
						OutputElement * pOutput2 = new OutputElement;
						pOutput2->sOutputTransactionOwner = r.second->sOwnerFrom;
						pOutput2->iOutputTransactionAmount = iRest;
						pOutput2->iAmountLeft = pOutput2->iOutputTransactionAmount;
						pOutput2->pTransaction = pResultingTransaction;

						pResultingTransaction->vOutputs.push_back(pOutput2);
						vPreviousOutputs.push_back(pOutput2);
					}

					vTransactions.push_back(pResultingTransaction);
					correctTransactions.push_back(pResultingTransaction);
				}
			}
		}

		vector<Block*> vValidBlocks;

		
		

		/*for (auto iter = mapTransactions.begin(); iter != mapTransactions.end(); ++iter)
		{
			auto iterfrom = mapPersons.find(iter->second->sAccountFrom);
			auto iterto = mapPersons.find(iter->second->sAccountTo);

			if (iterfrom != mapPersons.end() && iterto != mapPersons.end())
			{
				Person * pFrom = iterfrom->second;
				Person * pTo = iterto->second;

				iter->second->DoTransaction(pFrom, pTo);
			}
		}*/


		//todo

		ofstream outputFile(inputFile + ".out");
		cout << inputFile << " output:" << endl;
		if (outputFile.is_open())
		{
			outputFile << correctTransactions.size() << "\n";

			for (auto transaction : vTransactions)
			{
				auto iter = find(correctTransactions.begin(), correctTransactions.end(), transaction);
				if (iter != correctTransactions.end())
				{
					outputFile << transaction->sID << " " << transaction->vInputs.size() << " ";

					for (auto pinput : transaction->vInputs)
					{
						outputFile << pinput->sInputTransactionID << " " << pinput->sInputTransactionOwner << " " << pinput->iInputTransactionAmount << " ";
					}

					outputFile << transaction->vOutputs.size() << " ";

					for (auto poutput : transaction->vOutputs)
					{
						outputFile << poutput->sOutputTransactionOwner << " " << poutput->iOutputTransactionAmount << " ";
					}

					outputFile << transaction->iTime << "\n";
				}
			}

			/*outputFile << vPeople.size() << "\n";

			for (auto person : vPeople)
			{
				outputFile << person->sName << " " << person->iBalance << "\n";
			}

			outputFile << "\n";*/
			//cout << endl;
			outputFile.close();
		}
		else cout << "Unable to open file";

	}
	
    return 0;
}

