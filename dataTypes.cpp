#include "dataTypes.h"
#include <algorithm>

namespace dataTypes {

	vector<string> tableOfUserTypes(0);
	int tableOfSetsUserTypesSize = sizeof(tableOfSetsUserTypes) / sizeof(tableOfSetsUserTypes[0]);
	int tableOfTypesSize = sizeof(tableOfTypes) / sizeof(tableOfTypes[0]);



	//returns true if the given string is a data type of c++ or user defined data type, false otherwise
	bool isDataType(const string& stringRep) {
		for (int i = 0; i < tableOfTypesSize; i++) {
			if (stringRep == tableOfTypes[i]) {
				return true;
			}
		}
		for (int i = 0; i < tableOfUserTypes.size(); i++) {
			if (stringRep == tableOfUserTypes[i]) {
				return true;
			}
		}
		return false;
	}




	bool isSetsUserType(const string& stringRep) {
		for (int i = 0; i < tableOfSetsUserTypesSize; i++) {
			if (stringRep == tableOfSetsUserTypes[i]) {
				return true;
			}
		}
		return false;
	}



	Token* setDataTypeList(Token* firstToken) {
		TokenList* dataTypeList = new TokenList;
		Token* currentToken;
		currentToken = firstToken;
		string storedDataType = "";
		string storedObjectType = "";
		bool found;
		while (currentToken) {
			found = false;
			while (currentToken->getNext() && !found) {
				if (isDataType(currentToken->getStringRep())) {
					storedDataType = currentToken->getStringRep();
					currentToken = currentToken->getNext();
				}
				else {
					if ((currentToken->getNext()->getStringRep().find('{') != -1) || (currentToken->getNext()->getStringRep().find('(') != -1)) {
						currentToken = currentToken->getNext();
						storedDataType = "";
						break;
					}
					else if ((currentToken->getStringType() == ensc251::T_Identifier) && (!storedDataType.empty())) {
						if ((currentToken->getNext() != NULL) && ((currentToken->getNext()->getStringRep().find('=') != -1) || (currentToken->getNext()->getStringRep().find(';') != -1) 
							|| (currentToken->getNext()->getStringRep().find(')') != -1))) {
							Token* temp = new Token;
							temp->setDataType(storedDataType);
							storedObjectType += currentToken->getStringRep();
							temp->setStringRep(storedObjectType);
							dataTypeList->append(temp);
							storedDataType = "";
							storedObjectType = "";
							found = true;
							break;
						}
						storedObjectType += currentToken->getStringRep();
						currentToken = currentToken->getNext();
					}
					else {
						currentToken = currentToken->getNext();
					}
				}
			}
			currentToken = currentToken->getNext();
		}
		return dataTypeList->getFirst();
	}



	void setsUserDataTypes(Token* firstToken) {
		Token* currentToken;
		currentToken = firstToken;
		while (currentToken) {
			if (isSetsUserType(currentToken->getStringRep())) {
				while (currentToken->getNext()) {
					if ((currentToken->getNext()->getStringRep().find(';') != -1) || (currentToken->getNext()->getStringRep().find('=') != -1) ||
						(currentToken->getNext()->getStringRep().find('{') != -1)) {
						
						//if table of user types contains the string in current token, don't add it (aka do nothing), otherwise add it
						if (std::find(tableOfUserTypes.begin(), tableOfUserTypes.end(), currentToken->getStringRep()) != tableOfUserTypes.end()) {
							//DO NOTHING
						}
						else {
							tableOfUserTypes.push_back(currentToken->getStringRep());
						}
						break;
					}
					else {
						currentToken = currentToken->getNext();
					}

				}
			}
			else {
				currentToken = currentToken->getNext();
			}
		}

	}


}
