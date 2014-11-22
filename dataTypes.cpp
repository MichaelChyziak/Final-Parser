#include "dataTypes.h"

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
		return NULL;
	}


	void setsUserDataTypes(Token* firstToken) {
		Token* currentToken;
		currentToken = firstToken;
		while (currentToken) {
			if (isSetsUserType(currentToken->getStringRep())) {
				while (currentToken->getNext()) {
					if ((currentToken->getNext()->getStringRep().find(';') != -1) || (currentToken->getNext()->getStringRep().find('=') != -1) ||
						(currentToken->getNext()->getStringRep().find('{') != -1)) {
						tableOfUserTypes.push_back(currentToken->getStringRep());
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
