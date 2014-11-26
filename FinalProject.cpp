//Use only the following three libraries:
#include "parserClasses.h"
#include "dataTypes.h"
#include <iostream>
#include <fstream>

using namespace std;

//Removes all inline comments from the tokenList including the // marker
//Returns the number of comments removed
int removeInlineComments(TokenList &tokenList) {
	int count = 0;
	Token *t = tokenList.getFirst();
	while (t) {
		Token *c1, *c2;
		if (t->getStringRep() == "//") {
			c1 = t;
			c2 = t->getNext();
			if (c2->getStringRep() != "\n") {
				t = c2->getNext();
				tokenList.deleteToken(c2);
				tokenList.deleteToken(c1);
				count++;
			}
			else {
				t = c2;
				tokenList.deleteToken(c1);
				count++;
			}
		}
		else {
			t = t->getNext();
		}
	}
	return count;
}

//Remove all block comments from the tokenList including /* and */ markers
//Returns the number of block comments removed (where each pair of /* and */ count as one comment)
int removeBlockComments(TokenList &tokenList) {
	int count = 0;
	Token *t = tokenList.getFirst();
	Token *prev;
	bool deleteNext = false;

	while (t) {
		if (t->getStringRep() == "/*") {
			deleteNext = true;
		}

		prev = t;
		t = t->getNext();

		if (prev->getStringRep() == "*/") {
			tokenList.deleteToken(prev);
			deleteNext = false;
			count++;
		}

		if (deleteNext) {
			tokenList.deleteToken(prev);
		}
	}

	return count;
}

//Input: a list of tokens
//Output: head pointer to the list of assignment statements, returns NULL if there are no assignment statements
//NOTE: Assignment statement must end with a semi-colon
//@ description: extract all the assignment statements from input token list, prepare a new token list (assignment list)
//using extracted statements and return the head pointer to it


Token* getAssignmentStatements(TokenList &tokenList) {
	Token *t = tokenList.getFirst(); //current position in our token of the passed in token list
	TokenList *assignmentList;
	assignmentList = new TokenList;
	Token * temp = new Token; //temporary position of tokens
	bool isIdentifier = false;
	bool found = false;
	{
		using namespace ensc251;
		while (t) {
			if (t->getStringRep() == "=" || t->getStringRep() == "+=" || t->getStringRep() == "-=" || t->getStringRep() == "*=" ||
				t->getStringRep() == "/=" || t->getStringRep() == "<<=" || t->getStringRep() == ">>=" || t->getStringRep() == "|=" ||
				t->getStringRep() == "&=" || t->getStringRep() == "^=") { //check if it is one of these equals
				temp = t;
				while (!found) {
					//if we find a keyword, we found the end of the expression
					if (temp->getStringType() == T_Keyword) {
						found = true;
					}
					//checks if identifier, if it is then if the last one was also an identifier then we found the end, otherwise keep searching
					else if (temp->getStringType() == T_Identifier) {
						if (isIdentifier) {
							found = true;
							isIdentifier = false;
						}
						else {
							isIdentifier = true;
							found = false;
							if (temp->getPrev() != NULL) {
								temp = temp->getPrev();
							}
						}
					}
					//if we find a ; or a new line
					else if (temp->getStringRep() == ";" || temp->getStringRep() == "\n") {
						found = true;
						isIdentifier = false;
					}
					//otherwise it is all other cases which we do not have to cover
					else {
						if (temp->getPrev() != NULL) {
							temp = temp->getPrev();
							isIdentifier = false;
						}
						else {
							found = true;
						}
					}
				}
				//create the string that will hold the entire expression, up till the ;
				isIdentifier = false;
				found = false;
				if (temp->getPrev() == NULL) {
					//don't change the position of temp
				}
				else {
					temp = temp->getNext();
				}
				while (temp->getStringRep() != ";") {
					Token* temp2 = new Token;
					temp2->setDataType(temp->getDataType());
					temp2->setStringRep(temp->getStringRep());
					temp2->setStringType(temp->getStringType());
					temp = temp->getNext();
					assignmentList->append(temp2);
				}
				Token* temp2 = new Token;
				temp2->setDataType(temp->getDataType());
				temp2->setStringRep(temp->getStringRep());
				temp2->setStringType(temp->getStringType());
				temp = temp->getNext();
				assignmentList->append(temp2);
				//create the token and append it to the assignment list with the given string
				t = temp;
			}
			else {
				t = t->getNext();
			}
		}
	}
	return assignmentList->getFirst();
}


//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main() {
	ifstream sourceFile;
	TokenList tokens;
	Tokenizer tokenizer;

	//Read in a file line-by-line and tokenize each line
	sourceFile.open("test.cpp");
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		return 1;
	}

	while (!sourceFile.eof()) {
		string lineA, lineB;

		getline(sourceFile, lineA);

		//while the current line ends with a line-continuation \ append the next line to the current line
		while (lineA.length() > 0 && lineA[lineA.length() - 1] == '\\') {
			lineA.erase(lineA.length() - 1, 1);
			getline(sourceFile, lineB);
			lineA += lineB;
		}

		tokenizer.setString(&lineA);
		while (!tokenizer.isComplete()) {
			tokens.append(tokenizer.getNextToken());
		}
		//Re-insert newline that was removed by the getline function
		tokens.append("\n");
	}
	removeInlineComments(tokens);
	removeBlockComments(tokens);


	//Test your assignment statements
/*	Token *aListPtr = getAssignmentStatements(tokens);
	Token* temp = aListPtr;
	while (temp) {
		cout << temp->getStringRep() << endl;
		temp = temp->getNext();
	}/**/

	dataTypes::setsUserDataTypes(tokens.getFirst());
	dataTypes::setDataTypeList(tokens.getFirst()); 
	Token *aListPtr = getAssignmentStatements(tokens);
	cout << dataTypes::numberOfUnmatchedTypes(aListPtr);
/*	while (t) {
		cout << t->getStringRep() << ": " << t->getDataType() << endl;
		t = t->getNext();
	}/**/

/*	for (int i = 0; i < dataTypes::tableOfUserTypes.size(); i++) {
		cout << dataTypes::tableOfUserTypes[i] << endl;
	}/**/


/*	while (aListPtr) {
		cout << aListPtr->getStringRep() << aListPtr->getDataType() << endl;
		aListPtr = aListPtr->getNext();
	}/**/

	//FUNCTION DECLARATION TEST CASE
/*	Token* fDec = dataTypes::setFunctionDeclarations(tokens.getFirst());
	while (fDec) {
		cout << fDec->getStringRep() << " ";
		fDec = fDec->getNext();
	}/**/

	

	//FINAL PROJECT TEST
/*	dataTypes::setsUserDataTypes(tokens.getFirst()); //THIS HAS TO BE DONE
	Token* t = dataTypes::setDataTypeList(tokens.getFirst()); //BEFORE THIS
	while (t) {
		cout << t->getStringRep() << ": " << t->getDataType() << endl;
		t = t->getNext();
	}
	cout << endl;
	for (int i = 0; i < dataTypes::tableOfUserTypes.size(); i++) {
		cout << dataTypes::tableOfUserTypes[i] << endl;
	}/**/


	//MY TEST CASE
/*	Token *t = tokens.getFirst();
	int k =	0;
	while (t) {
		cout << "token" << k << ": " << t->getStringRep() << ": " << t->getDataType()<< "\n";
		t = t->getNext();
		k++;
	}/**/

	//Test your tokenization of the file by traversing the tokens list and printing out the tokens and the tokens type */
/*	Token *t = tokens.getFirst();
	while (t) {
	cout << t->getStringRep() << " ";
	t = t->getNext();
	}
	t = tokens.getFirst();
	while (t) {
	cout << t->getStringType() << " ";
	t = t->getNext();
	}/**/

	return 0;
}
