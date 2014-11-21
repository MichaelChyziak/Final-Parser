#include "dataTypes.h"

namespace dataTypes {

	vector<string> tableOfUserTypes;
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
	}

	Token* setDataTypeList(Token* firstToken) {
		TokenList* dataTypeList = new TokenList;
		Token* currentType;
		currentType = firstToken;
		while (currentType) {
			if (isDataType(currentType->getStringRep())) {
				if ((currentType->getNext() != NULL)  && (currentType->getNext()->getStringType() == ensc251::T_Identifier)) {
					currentType = currentType->getNext();
					currentType->setDataType(currentType->getPrev()->getStringRep());
					Token* temp;
					temp = currentType;
					currentType = currentType->getNext();
					dataTypeList->append(temp);
				}
			}
			if (isSetsUserType(currentType->getStringRep())) {
				tableOfUserTypes.push_back(currentType->getStringRep());
				currentType = currentType->getNext();
			}
		}
		return dataTypeList->getFirst();
	}
	
}
