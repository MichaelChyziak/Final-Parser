
#include "dataTypes.h"
#include <algorithm>
namespace dataTypes {
	//initializing variables used in the data type funtions 
	vector<string> variablesAndTypes(0);
	vector<string> tableOfUserTypes(0);
	vector<string> bracesErrors(0);
	vector<string> unmatchedErrors(0);
	int tableOfSetsUserTypesSize = sizeof(tableOfSetsUserTypes) / sizeof(tableOfSetsUserTypes[0]);
	int tableOfTypesSize = sizeof(tableOfTypes) / sizeof(tableOfTypes[0]);
	//returns true if the given string is a data type of c++ or user defined data type, false otherwise
	//to do this it passes in a string and compares it with tables of defined data types and user defigned types
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
	//to do this it passes in string values and compares it with the vector that is created after preparsing a c++ file for user defined types 
	bool isUserType(const string& stringRep) {
		for (int i = 0; i < tableOfSetsUserTypesSize; i++) {
			if (stringRep == tableOfSetsUserTypes[i]) {
				return true;
			}
		}
		return false;
	}
	//this parses through a token list and assigns appropriate data types
	void setDataTypeList(Token* firstToken) {
		Token* currentToken;
		currentToken = firstToken;
		string storedDataType = "";
		bool found;
		//This loop keeps moving through the entire token list looking for  types
		while (currentToken) {
			found = false;
			while (currentToken->getNext() && !found) {
				//this discovers if the read in value is a data type if it is it records the value 
				if (isDataType(currentToken->getStringRep())) {
					storedDataType = currentToken->getStringRep();
					currentToken = currentToken->getNext();
				}
				else {
					//if the value after the next value is anything but a bracket then  move forward and make the stored value empty
					if ((currentToken->getNext()->getStringRep().find('{') != -1)) {
						currentToken = currentToken->getNext();
						storedDataType = "";
						break;
					}
					
					//if you read in *(pointer symbol) or useless brackets move forward looking for an identifier to pin a type too
					else if (currentToken->getStringRep() == "*" || currentToken->getStringRep() == ")" ||currentToken->getStringRep() == "(") {
						currentToken = currentToken->getNext();
					}
					//case where you find a user type definition -  you find a identifier with its data type not empty 
					//then you pop it into a list with a identifier and the type
					//you also set the type in the list 
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
	//this goes through a token list and assignes appropriate strings to the datatype member variable of tokens
	void setsUserDataTypes(Token* firstToken) {
		Token* currentToken = new Token;
		currentToken = firstToken;
		
		while (currentToken) {
			//you go in this loop if you get a token that is a user type 
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
											(isBetterKeyword(startToken->getPrev()->getStringRep())) ||
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
			//start token does not equal zero start moving 
			//end token through list using a bracket counter to make sure you are at the end of funtion statement
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
			//checks that your end token is and the end of the tokenlist 
			if (endToken && endToken->getNext() && endToken->getNext()->getStringRep() == ";") {
				endToken = endToken->getNext();
			}
			else {
				endToken = NULL;
			}
			//now you know where your end token lies and your start token lies start making a new token and append it to a new token list
			while (endToken && startToken && startToken->getStringRep() != ";") {
				Token* temp = new Token;
				temp->setDataType(startToken->getDataType());
				temp->setStringRep(startToken->getStringRep());
				temp->setStringType(startToken->getStringType());
				startToken = startToken->getNext();
				functionList->append(temp);
			}
			//it is in this while that you just keep on appending tokens between start token and end token to the new list 
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
			
			//this matches appropriate identifiers to types if they were given types in the code 
			//this is needed for unmatched identification of identifiers
			for(int i = 0; i < variablesAndTypes.size(); i = i + 2) {
				if (assignmentToken->getStringRep() == variablesAndTypes[i]) {
					assignmentToken->setDataType(variablesAndTypes[i+1]);
				}
			}
			//keep running forward and changing identifiers to proper types till you hit a assignment statement 
			//and assignment statement is identified by a = sign in a token
			while(assignmentToken->getStringRep().find('=') == -1) {
			
			//this matches appropriate identifiers to types if they were given types in the code 
			//this is needed for unmatched identification of identifiers
				for(int i = 0; i < variablesAndTypes.size(); i = i + 2) {
					if (assignmentToken->getStringRep() == variablesAndTypes[i]) {
						assignmentToken->setDataType(variablesAndTypes[i+1]);
					}
				}
				//this is the case where you have multiple variables with different types on the left side of an assignment 
				//if the tokens data type is not empty (ie. not an int, bool, char, etc)
				if (assignmentToken->getDataType() != "") {
					//if the varType is already assigned then we have an error
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
			
			//once you hit this you have passed the = sign and are looking for mismatches till the ;
			while (assignmentToken && assignmentToken->getStringRep() != ";") {
				for(int i = 0; i < variablesAndTypes.size(); i = i + 2) {
					if (assignmentToken->getStringRep() == variablesAndTypes[i]) {
						assignmentToken->setDataType(variablesAndTypes[i+1]);
					}
				}
				
				//this error occurs when you have an assignment like " = 5 ;"
			    //you can see it is not matched with anything
				if (varDataType == "") {
					string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
						" error because there was no data type initialized on the left side of the statement.";
					unmatchedErrors.push_back(temp);
					errors++;
					break;
				}

				//this case occurs when you have an assignment with two identifiers of mismatching types
				//we detect this by having a identifier read in on the right side that is not the same type
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

				//this case occurs when you have a assignment with a indentifier and a unmatching boolean value. i.e int a = true;
				//we detect this by reading in a boolean value on the right side of a assignment and not having a boolean identifier on the left side
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
				//this case occurs when you have a assignment with a indentifier and a unmatching integer 
				//i.e char a = 5;
				//we detect this case by reading in a integer on the right side but not having a integer identifier on the left side
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
				
				//this case occurs when you have a assignment with a indentifier and a unmatching float or double on the right side
				//to simplify some difficult cases we have combined double and floats together as identified as they are combined as a float literal
				//i.e char a = 2.0;
				//we detect this case by reading in a float on the right side but not having a float or double identifier on the left side
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


				//this case occurs when you have a assignment with a indentifier and a unmatching char on the right side
				//we detect this case by reading in a char on the right sided but not having a char identifier on the left side 
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
				//if non of these cases keep moving forward till you hit a ';'
				else {
				}
				assignmentToken = assignmentToken->getNext();
				assignmentNumber++;
			}
			// this is the case when you have nothing on the right of the assignment 
			// i.e "a = ;"
			if (!matchFound && varDataType != "") {
				string temp = "For assignment statement # " + to_string(assignmentNumber) + ": There was an" +
					" error because no match was found to the for " + varDataType + " .";
				unmatchedErrors.push_back(temp);
				errors++;
				varDataType = "";
			}
			varDataType = "";
			matchFound = false;
			//when you exit the while loop you are still on the semicolon or null so you need to move forward to parse the next assignment 
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
			//this counts brackets pointing left and right in single statements 
			while(firstToken && counter >= 0 && firstToken->getStringRep() != ";") {
				if (firstToken->getStringRep() == "(") {
					counter++;
				}
				else if (firstToken->getStringRep() == ")") {
					counter--;
				}
				firstToken = firstToken->getNext();
			}
			//this value is incremented to keep track of statement location in code where errors might occur
			assignmentStatementNumber++;

			//this loop is entered to push error statements in a brace error vector that records the line the error occured 
			//to be displayed later and reinitialize the counter to look at the new statements
			if (counter != 0) {

				//counter being too positive means too many "('
				if (counter > 0) {
					string temp = "For assignment statement # " + to_string(assignmentStatementNumber) + ": There was an" +
						" error because there too many \" ( \" brackets.";
					bracesErrors.push_back(temp);
				}
				//counter being to negative means to many ")"
				else if (counter < 0) {
					string temp = "For assignment statement # " + to_string(assignmentStatementNumber) + ": There was an" +
						" error because there too many \" ) \" brackets.";
					bracesErrors.push_back(temp);
				}
				// if you have entered the loop you have obviously encountered an error and that is too be counted
				counter = 0;
				errors++;
			}
			// move forward past the semicolon and continue
			if (firstToken) {
				firstToken = firstToken->getNext();
			}
		}
		return errors;
	}
}
