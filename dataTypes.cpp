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

	bool isUserType(const string& stringRep) {
		for (int i = 0; i < tableOfSetsUserTypesSize; i++) {
			if (stringRep == tableOfSetsUserTypes[i]) {
				return true;
			}
		}
		return false;
	}


/*	Token* setDataTypeList(Token* firstToken) {
		TokenList* dataTypeList = new TokenList;
		Token* currentToken = new Token;
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
						storedObjectType = "";
						break;
					}
					else if (!storedDataType.empty()) {
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
	}/**/

	
	void setDataTypeList(Token* firstToken) {
		Token* currentToken;
		currentToken = firstToken;
		string storedDataType = "";
		bool found;
		while (currentToken) {
			found = false;
			while (currentToken->getNext() && !found) {
				if (isDataType(currentToken->getStringRep())) {
					storedDataType = currentToken->getStringRep();
					currentToken = currentToken->getNext();
				}
				else {
					if ((currentToken->getNext()->getStringRep().find('{') != -1)) {
						currentToken = currentToken->getNext();
						storedDataType = "";
						break;
					}
					else if (currentToken->getStringRep() == "*" || currentToken->getStringRep() == ")" ||currentToken->getStringRep() == "(") {
						currentToken = currentToken->getNext();
					}

					else if ((currentToken->getStringType() == ensc251::T_Identifier) && !(storedDataType.empty())) {
						currentToken->setDataType(storedDataType);
						storedDataType = "";
						found = true;
						break;
						currentToken = currentToken->getNext();
					}
					else {
						currentToken = currentToken->getNext();
					}
				}
			}
			currentToken = currentToken->getNext();
		}
	}



	void setsUserDataTypes(Token* firstToken) {
		Token* currentToken = new Token;
		currentToken = firstToken;
		while (currentToken) {
			if (isUserType(currentToken->getStringRep())) {
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

	//This function returns TRUE if the input is one of the keywords defined in the variable
	//"tableOfBetterKeywords" in "dataTypes.h" else it returns FALSE
	bool isBetterKeyword(const string &lexeme) {
		string temp = lexeme; //creates a temporary string variable to hold the value of lexeme
		int sizeOfBetterKeywords = sizeof(tableOfBetterKeywords) / sizeof(tableOfBetterKeywords[0]);

		//compares the temporary string to all of the strings from "tableOfKeywords" in "lexemesTypes.h"
		//returns true if the string matches to one of the strings in the array, false otherwise
		for (int i = 0; i < sizeOfBetterKeywords; i++) {
			if (temp == tableOfBetterKeywords[i]) {
				return true;
			}
		}
		return false;
	}


	Token* setFunctionDeclarations(Token* firstToken) {
		Token* startToken = new Token;
		Token* endToken = new Token;
		Token* currentToken = new Token;
		currentToken = firstToken;
		TokenList* functionList = new TokenList;
		bool found = false;
		while(currentToken) {
			while (true) {
				//If the current token has the string value of "(", then test for function declaraction further, otherwise go to the next token
				if (currentToken->getStringRep() == "(") {
				
					//checks if the previous token exists, otherwise go to the next token
					if (currentToken->getPrev()) {
					
						//checks if the current token is an identifier and assigns the current token to the previous token if it exists,
						//if it isn't then it isn't a function declaration
						if (ensc251::isIdentifier(currentToken->getPrev()->getStringRep())) {
							startToken = currentToken;
							startToken = startToken->getPrev();
						
							//if it is a constructor it is a special case and set firstToken to the startToken, otherwise it is a 
							//normal function declaration
							if (isUserType(startToken->getStringRep())) {
								break;
							}
							else {
								//general function identifier case
								//checks if the previous function is not null and is either a data type or a keyword from ensc::251 namespace,
								//otherwise go to the next token
								if ((startToken->getPrev()) && ((isDataType(startToken->getPrev()->getStringRep())) || 
									(isBetterKeyword(startToken->getPrev()->getStringRep())))) {
										startToken = startToken->getPrev();
										while ((startToken->getPrev()) && ((isDataType(startToken->getPrev()->getStringRep())) || 
											(isBetterKeyword(startToken->getPrev()->getStringRep()))  || 
											(startToken->getPrev()->getStringRep() == "*") || startToken->getPrev()->getStringRep() == "&")) {
												startToken = startToken->getPrev();
										}
										break;
								}
								else {
									startToken = NULL;
								
									break;
								}
							}
						}
						else {
							startToken = NULL;
					
							break;
						}
					}
					else {
						startToken = NULL;
						
						break;
					}
				}
				else {
					startToken = NULL;
					
					break;
				}
			}
			if (startToken) {
				endToken = currentToken;
				int bracketCounter = 0;
				while (bracketCounter >= 0) {
					if (endToken->getNext()) {
						if (endToken->getNext()->getStringRep() == "(") {
							bracketCounter++;
						}
						else if (endToken->getNext()->getStringRep() == ")") {
							bracketCounter--;
						}
					}
					else {
						bracketCounter = -1;
					}
					endToken = endToken->getNext();
				}
			}
			if (endToken && endToken->getNext() && endToken->getNext()->getStringRep() == ";") {
				endToken = endToken->getNext();
			}
			else {
				endToken = NULL;
			}

			while (endToken && startToken && startToken->getStringRep() != ";") {
				Token* temp = new Token;
				temp->setDataType(startToken->getDataType());
				temp->setStringRep(startToken->getStringRep());
				temp->setStringType(startToken->getStringType());
				startToken = startToken->getNext();
				functionList->append(temp);
			}
			if (startToken && functionList->getLast() && functionList->getLast()->getStringRep() != ";") {
				Token* temp = new Token;
				temp->setDataType(startToken->getDataType());
				temp->setStringRep(startToken->getStringRep());
				temp->setStringType(startToken->getStringType());
				startToken = startToken->getNext();
				functionList->append(temp);
			}
			startToken = NULL;
			endToken = NULL;
			currentToken = currentToken->getNext();
		}
		return functionList->getFirst();
	}

/*	int numberOfUnmatchedTypes(Token* firstToken) {
		Token* assignmentList = new Token;
		assignmentList = firstToken;
		bool searching = true; //searching for if we encounter a semicolon or not
		string listDataType = ""; //the type that everything else should match
		int errors = 0; //how many errors there are (1 max for each assignment statement)

		while (assignmentList ) {
			while (searching) {
				if (assignmentList->getDataType() != "") {
					if (listDataType.empty()) {
						listDataType = assignmentList->getDataType();
							if (assignmentList->getNext()) {
								assignmentList->getNext();
							}
					}
					else {
						searching = false;
						errors++;
						break;
					}
				}
				else if (assignmentList->getStringRep() == "=") {
					if (listDataType == "") {
						errors++;
						searching = false;
					}
					else {
						while (assignmentList->getStringRep() != ";") {
							if (assignmentList->getStringType() == ensc251::T_Boolean) {
								if (listDataType == "bool") {
									//it is, so continue
								}
								else {
									errors++;
									searching = false;
									break;
								}
							}
							else if (assignmentList->getStringType() == ensc251::T_IntegerLiteral) {
								if (listDataType == "int") {
									//it is, so continue
								}
								else {
									errors++;
									searching = false;
									break;
								}
							}
							if (assignmentList->getStringType() == ensc251::T_FloatLiteral) {
								if (listDataType == "float" && listDataType == "double") {
									//it is, so continue
								}
								else {
									errors++;
									searching = false;
									break;
								}
							}
						}
					}
				}
				else {
					assignmentList = assignmentList->getNext();
				}
			}
			if (assignmentList->getStringRep() == ";") {
				searching = true;
			}
			else if (!searching) {
				while (assignmentList && assignmentList->getStringRep() != ";") {
					assignmentList = assignmentList->getNext();
				}
				searching = true;
			}
			assignmentList = assignmentList->getNext();
		}
		return errors;
	}*/


	int numberOfUnmatchedTypes(Token* firstToken) {
		Token* assignmentToken = new Token;
		assignmentToken = firstToken;
		string varDataType = "";
		int errors = 0;
		bool matchFound = false;
		while (assignmentToken) {
			while(assignmentToken->getStringRep().find('=') == -1) {
				if (assignmentToken->getDataType() != "") {
					if (varDataType != "") {
						varDataType = "";
						errors++;
						break;
					}
					varDataType = assignmentToken->getDataType();
				}
				assignmentToken = assignmentToken->getNext();
			}
			while (assignmentToken && assignmentToken->getStringRep() != ";") {
				if (varDataType == "") {
					errors++;
					break;
				}
				else if(assignmentToken->getDataType() != "") {
					if (assignmentToken->getDataType() != varDataType) {
						errors++;
						varDataType = "";
						break;
					}
					matchFound = true;
				}
				else if(assignmentToken->getStringType() == ensc251::T_Boolean) {
					if (varDataType != "bool") {
						errors++;
						varDataType = "";
						break;
					}
					matchFound = true;
				}
				else if(assignmentToken->getStringType() == ensc251::T_IntegerLiteral) {
					if (varDataType != "int") {
						errors++;
						varDataType = "";
						break;
					}
					matchFound = true;
				}
				else if(assignmentToken->getStringType() == ensc251::T_FloatLiteral) {
					if (varDataType != "float" || varDataType != "double") {
						errors++;
						varDataType = "";
						break;
					}
					matchFound = true;
				}
				else if((assignmentToken->getStringRep().size() == 3 || assignmentToken->getStringRep().size() == 4)
					&& (assignmentToken->getStringRep().at(0) == '\'') 
					&& (assignmentToken->getStringRep().at(assignmentToken->getStringRep().size() -1 ) == '\'')) {
						if (varDataType != "char") {
							errors++;
							varDataType = "";
							break;
						}
						matchFound = true;
				}
				else {
				}
				assignmentToken = assignmentToken->getNext();
			}
			if (!matchFound && varDataType != "") {
				errors++;
				varDataType = "";
			}					
			varDataType = "";
			matchFound = false;
			while (assignmentToken) {
				if (assignmentToken->getStringRep() == ";") {
					assignmentToken = assignmentToken->getNext();
					break;
				}
				else {
					assignmentToken = assignmentToken->getNext();
				}
			}
		}
		return errors;
	}




	int numberOfUnmatchedBraces(Token* firstToken) {
		int errors = 0;
		return errors;
	}
}
