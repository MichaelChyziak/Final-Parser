#include "dataTypes.h"
#include <algorithm>

namespace dataTypes {
	vector<string> variablesAndTypes(0);
	vector<string> tableOfUserTypes(0);
	vector<string> bracesErrors(0);
	vector<string> unmatchedErrors(0);
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
   // returns true if string matches a string in the user defigned type vector, false otherwise;
	bool isUserType(const string& stringRep) {
		for (int i = 0; i < tableOfSetsUserTypesSize; i++) {
			if (stringRep == tableOfSetsUserTypes[i]) {
				return true;
			}
		}
		return false;
	}
	

//this parses through a token list and looks for user defigned types to add to tableoftypes vector
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
						variablesAndTypes.push_back(currentToken->getStringRep());
						variablesAndTypes.push_back(storedDataType);
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


	//this goes through a token list and assignes appropriate strings to the datatype membervariable of tokens
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

	//this makes a new list of all the funtion declarations in a list of tokens
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

	//this makes a count of the number of syntax incorrect unmatched types in a assignment statement 
	int numberOfUnmatchedTypes(Token* firstToken) {
		Token* assignmentToken = new Token;
		assignmentToken = firstToken;
		string varDataType = "";
		int errors = 0;
		long double assignmentNumber = 1;
		bool matchFound = false;
		while (assignmentToken) {
			
			for(int i = 0; i < variablesAndTypes.size(); i = i + 2) {
				if (assignmentToken->getStringRep() == variablesAndTypes[i]) {
					assignmentToken->setDataType(variablesAndTypes[i+1]);
				}
			}


			//if we don't find a equal sign
			while(assignmentToken->getStringRep().find('=') == -1) {
				for(int i = 0; i < variablesAndTypes.size(); i = i + 2) {
					if (assignmentToken->getStringRep() == variablesAndTypes[i]) {
						assignmentToken->setDataType(variablesAndTypes[i+1]);
					}
				}
				//if the tokens data type is not empty (ie. not an int, bool, char, etc)
				if (assignmentToken->getDataType() != "") {
					//if the varType is already asigned then we have an error
					if (varDataType != "") {
						varDataType = "";
						string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
						" error because there was another variable type trying to be assigned on the left side of the statement.";
						unmatchedErrors.push_back(temp);
						errors++;
						break;
					}
					varDataType = assignmentToken->getDataType();
				}
				assignmentToken = assignmentToken->getNext();
				//assignmentNumber++;
			}
			while (assignmentToken && assignmentToken->getStringRep() != ";") {
				for(int i = 0; i < variablesAndTypes.size(); i = i + 2) {
					if (assignmentToken->getStringRep() == variablesAndTypes[i]) {
						assignmentToken->setDataType(variablesAndTypes[i+1]);
					}
				}
				if (varDataType == "") {
					string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
					" error because there was no data type initialized on the left side of the statement.";
					unmatchedErrors.push_back(temp);
					errors++;
					break;
				}
				else if(assignmentToken->getDataType() != "") {
					if (assignmentToken->getDataType() != varDataType) {
						errors++;
						string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
						" error because there is a data type but it is not the same as the left side of the statement.";
						unmatchedErrors.push_back(temp);
						varDataType = "";
						break;
					}
					matchFound = true;
				}
				else if(assignmentToken->getStringType() == ensc251::T_Boolean) {
					if (varDataType != "bool") {
						errors++;
						string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
						" error because the left side variable is not of type bool, but " + assignmentToken->getStringRep() + " is.";
						unmatchedErrors.push_back(temp);
						varDataType = "";
						break;
					}
					matchFound = true;
				}
				else if(assignmentToken->getStringType() == ensc251::T_IntegerLiteral) {
					if (varDataType != "int") {
						errors++;
						string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
						" error because the left side variable is not of type integer, but " + assignmentToken->getStringRep() + " is.";
						unmatchedErrors.push_back(temp);
						varDataType = "";
						break;
					}
					matchFound = true;
				}
				else if(assignmentToken->getStringType() == ensc251::T_FloatLiteral) {
					if (varDataType != "float" || varDataType != "double") {
						string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
						" error because the left side variable is not of type float or double, but " + assignmentToken->getStringRep() + " is.";
						unmatchedErrors.push_back(temp);
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
							string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
								" error because the left side variable is not of type char, but " + assignmentToken->getStringRep() + " is.";
							unmatchedErrors.push_back(temp);
							errors++;
							varDataType = "";
							break;
						}
						matchFound = true;
				}
				else {
				}
				assignmentToken = assignmentToken->getNext();
				assignmentNumber++;
			}
			if (!matchFound && varDataType != "") {
				string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
				" error because no match was found to the for " + varDataType + " .";
				unmatchedErrors.push_back(temp);
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



	//this makes a count of the number of syntax incorrect unmatched braces
	int numberOfUnmatchedBraces(Token* firstToken) {
		Token* currentToken = firstToken;
		int counter = 0;
		int errors = 0;
		long double assignmentStatementNumber = 0;
		while (firstToken) {
			while(firstToken && counter >= 0 && firstToken->getStringRep() != ";") {
				if (firstToken->getStringRep() == "(") {
					counter++;
				}
				else if (firstToken->getStringRep() == ")") {
					counter--;
				}
				firstToken = firstToken->getNext();
			}
			assignmentStatementNumber++;
			if (counter != 0) {
				if (counter > 0) {
					string temp = "For assignment statement # " + to_string(assignmentStatementNumber) + ": There was an" +
						" error because there too many \" ( \" brackets.";
					bracesErrors.push_back(temp);
				}
				else if (counter < 0) {
					string temp = "For assignment statement # " + to_string(assignmentStatementNumber) + ": There was an" +
						" error because there too many \" ) \" brackets.";
					bracesErrors.push_back(temp);
				}
				counter = 0;
				errors++;
			}
			if (firstToken) {
				firstToken = firstToken->getNext();
			}
		}
		return errors;
	}

}
