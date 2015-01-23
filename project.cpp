//Use only the following three libraries:
#include "parserClasses.h"
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

	//asks the user what mode that they want
	char userResponse;
	cout << "Input the number corresponding to the type you want to run" << endl;
	cout << "Non-Verbose = 1" << endl << "Verbose = 2" << endl << "Exit = 3" << endl;
	cin >> userResponse;

	//enter non verbose mode
	if (userResponse == '1') {

		//Number of assignment Statements
		ensc251::setsUserDataTypes(tokens.getFirst());
		ensc251::setDataTypeList(tokens.getFirst());
		Token *aListPtr = getAssignmentStatements(tokens);
		Token* tempAList = aListPtr;
		int numOfAStatements = 0;
		while (tempAList) {
			if (tempAList->getStringRep() == ";") {
				numOfAStatements++;
			}
			tempAList = tempAList->getNext();
		}

		//prints out the assignment statements and its following errors
		cout << "---------------NON VERBOSE OUTPUT----------------------" << endl;
		cout << "The number of assignment statements: " << numOfAStatements << endl;
		cout << "The number of assignment statements with unmatched types: " << dataTypes::numberOfUnmatchedTypes(aListPtr) << endl;
		cout << "The number of assignment statements with unmatched braces: " << dataTypes::numberOfUnmatchedBraces(aListPtr) << endl;

		//number of function declarations
		Token* fDec = dataTypes::setFunctionDeclarations(tokens.getFirst());
		Token* tempFDec = fDec;
		int numOfFDecs = 0;
		while (tempFDec) {
			if (tempFDec->getStringRep() == ";") {
				numOfFDecs++;
			}
			tempFDec = tempFDec->getNext();
		}
		cout << "The number of function declarations: " << numOfFDecs << endl;

		// total number of tokens in the program ---------------------------------------------
		Token* tcount = tokens.getFirst();
		int numOftokens = 0;
		while (tcount) {
			numOftokens++;
			tcount = tcount->getNext();
		}
		cout << "The number of tokens: " << numOftokens << endl;



		// total # of token types in the program
		Token* tokentype = tokens.getFirst();
		int lexicaltypecount[9] = {};
		while (tokentype) {
			using namespace ensc251;

			//0
			if (tokentype->getStringType() == T_Identifier){
				lexicaltypecount[0]++;
			}
			//1
			if (tokentype->getStringType() == T_Operator){
				lexicaltypecount[1]++;
			}
			//2
			if (tokentype->getStringType() == T_Punctuator){
				lexicaltypecount[2]++;
			}
			//3
			if (tokentype->getStringType() == T_Keyword){
				lexicaltypecount[3]++;
			}
			//4
			if (tokentype->getStringType() == T_Boolean){
				lexicaltypecount[4]++;
			}
			//5
			if (tokentype->getStringType() == T_IntegerLiteral){
				lexicaltypecount[5]++;
			}
			//6
			if (tokentype->getStringType() == T_FloatLiteral){
				lexicaltypecount[6]++;
			}
			//7
			if (tokentype->getStringType() == T_StringLiteral){
				lexicaltypecount[7]++;
			}
			//8
			if (tokentype->getStringType() == T_Unknown){
				lexicaltypecount[8]++;
			}
			tokentype = tokentype->getNext();
		}

		//prints out the total number of each type of token
		cout << "-------LEXEMES TYPES-------" << endl;
		cout << "The number of identifiers " << lexicaltypecount[0] << endl;
		cout << "The number of operators " << lexicaltypecount[1] << endl;
		cout << "The number of keywords " << lexicaltypecount[2] << endl;
		cout << "The number of puntuators " << lexicaltypecount[3] << endl;
		cout << "The number of boolean values " << lexicaltypecount[4] << endl;
		cout << "The number of integer literals " << lexicaltypecount[5] << endl;
		cout << "The number of float literals " << lexicaltypecount[6] << endl;
		cout << "The number of string literals " << lexicaltypecount[7] << endl;
		cout << "The number of unknown tokens " << lexicaltypecount[8] << endl;
		cout << "------------------------------------------" << endl;

		//lets the user look at it, and presses a key to end the program
		cout << endl << endl;
		system("PAUSE");
	}


	//enters verbose mode
	else if (userResponse == '2') {
		dataTypes::setsUserDataTypes(tokens.getFirst());
		dataTypes::setDataTypeList(tokens.getFirst());
		//Number of assignment Statements
		Token *aListPtr = getAssignmentStatements(tokens);
		Token* tempAList = aListPtr;
		int numOfAStatements = 1;
		bool printNumOfStatements = true;

		cout << "---------------VERBOSE OUTPUT----------------------" << endl;
		cout << "The assignment statements: " << endl;

		//prints out the assignment statements from beginning to the ";"
		while (tempAList) {
			if (printNumOfStatements) {
				cout << numOfAStatements << " | ";
				printNumOfStatements = false;
			}
			cout << tempAList->getStringRep() << " ";
			if (tempAList->getStringRep() == ";") {
				cout << endl;
				printNumOfStatements = true;
				numOfAStatements++;
			}
			tempAList = tempAList->getNext();
		}
		dataTypes::numberOfUnmatchedTypes(aListPtr);
		dataTypes::numberOfUnmatchedBraces(aListPtr);


		cout << endl << "Assignment statements unmatched types errors: " << endl;
		for (int i = 0; i < dataTypes::unmatchedErrors.size(); i++) {
			cout << i + 1 << " | " << dataTypes::unmatchedErrors[i] << endl;
		}

		cout << endl << "Assignment statements unmatched braces errors: " << endl;
		for (int i = 0; i < dataTypes::bracesErrors.size(); i++) {
			cout << i + 1 << " | " << dataTypes::bracesErrors[i] << endl;
		}

		//number of function declarations
		//counts by finding the ";" which means that we have 1 function declaration,
		//print out everything to the ";"
		Token* fDec = dataTypes::setFunctionDeclarations(tokens.getFirst());
		Token* tempFDec = fDec;
		int numOfFDecs = 1;
		bool printNumOfFunction = true;
		cout << endl << "The function declarations: " << endl;
		while (tempFDec) {
			if (printNumOfFunction) {
				cout << numOfFDecs << " | ";
				printNumOfFunction = false;
			}
			cout << tempFDec->getStringRep() << " ";
			if (tempFDec->getStringRep() == ";") {
				cout << endl;
				printNumOfFunction = true;
				numOfFDecs++;
			}
			tempFDec = tempFDec->getNext();
		}
		//counts the number of tokens in our list
		Token* tcount = tokens.getFirst();
		int numOftokens = 0;
		while (tcount) {
			numOftokens++;
			tcount = tcount->getNext();
		}
		cout << "The number of tokens: " << numOftokens << endl;


		// total # of token types in the program---------------
		Token* tokentype = tokens.getFirst();
		int lexicaltypecount[9] = {};
		while (tokentype) {
			using namespace ensc251;

			//0
			if (tokentype->getStringType() == T_Identifier){
				lexicaltypecount[0]++;
			}
			//1
			if (tokentype->getStringType() == T_Operator){
				lexicaltypecount[1]++;
			}
			//2
			if (tokentype->getStringType() == T_Punctuator){
				lexicaltypecount[2]++;
			}
			//3
			if (tokentype->getStringType() == T_Keyword){
				lexicaltypecount[3]++;
			}
			//4
			if (tokentype->getStringType() == T_Boolean){
				lexicaltypecount[4]++;
			}
			//5
			if (tokentype->getStringType() == T_IntegerLiteral){
				lexicaltypecount[5]++;
			}
			//6
			if (tokentype->getStringType() == T_FloatLiteral){
				lexicaltypecount[6]++;
			}
			//7
			if (tokentype->getStringType() == T_StringLiteral){
				lexicaltypecount[7]++;
			}
			//8
			if (tokentype->getStringType() == T_Unknown){
				lexicaltypecount[8]++;
			}
			tokentype = tokentype->getNext();
		}

		//prints out the amount of each type
		cout << "-------LEXEMES TYPES-------" << endl;
		cout << "The number of identifiers " << lexicaltypecount[0] << endl;
		cout << "The number of operators " << lexicaltypecount[1] << endl;
		cout << "The number of keywords " << lexicaltypecount[2] << endl;
		cout << "The number of puntuators " << lexicaltypecount[3] << endl;
		cout << "The number of boolean values " << lexicaltypecount[4] << endl;
		cout << "The number of integer literals " << lexicaltypecount[5] << endl;
		cout << "The number of float literals " << lexicaltypecount[6] << endl;
		cout << "The number of string literals " << lexicaltypecount[7] << endl;
		cout << "The number of unknown tokens " << lexicaltypecount[8] << endl;
		cout << "------------------------------------------" << endl;


		// printing out lexemes types
		using namespace ensc251;
		{
			//all other examples follow the same procedure, so will only explain once
			//Go from the top of the list and check if it is identifier type (or other for other implementations) and if it is then print it out
			//go through the entire list until its done
			cout << "THE IDENTIFIERS:" << endl;
			Token* identifierparse = tokens.getFirst();
			while (identifierparse){
				if (identifierparse->getStringType() == T_Identifier)
					cout << identifierparse->getStringRep() << endl;
				identifierparse = identifierparse->getNext();
			}

			cout << "THE OPERATORS:" << endl;
			Token* operatorparse = tokens.getFirst();
			while (operatorparse){
				if (operatorparse->getStringType() == T_Operator)
					cout << operatorparse->getStringRep() << endl;
				operatorparse = operatorparse->getNext();
			}


			cout << "THE KEYWORDS:" << endl;
			Token* keywordparse = tokens.getFirst();
			while (keywordparse){
				if (keywordparse->getStringType() == T_Keyword)
					cout << keywordparse->getStringRep() << endl;
				keywordparse = keywordparse->getNext();
			}


			cout << "THE PUNCTUATOR:" << endl;
			Token* puntuatorparse = tokens.getFirst();
			while (puntuatorparse){
				if (puntuatorparse->getStringType() == T_Punctuator)
					cout << puntuatorparse->getStringRep() << endl;
				puntuatorparse = puntuatorparse->getNext();
			}/**/


			cout << "THE BOOLEAN VALUES:" << endl;
			Token* booleanparse = tokens.getFirst();
			while (booleanparse){
				if (booleanparse->getStringType() == T_Boolean) {
					cout << booleanparse->getStringRep() << endl;
				}
				booleanparse = booleanparse->getNext();
			}

			cout << "THE INTEGER LITERALS:" << endl;
			Token* integerparse = tokens.getFirst();
			while (integerparse){
				if (integerparse->getStringType() == T_IntegerLiteral) {
					cout << integerparse->getStringRep() << endl;
				}
				integerparse = integerparse->getNext();
			}

			cout << "THE FLOAT LITERALS:" << endl;
			Token* floatparse = tokens.getFirst();
			while (floatparse){
				if (floatparse->getStringType() == T_FloatLiteral) {
					cout << floatparse->getStringRep() << endl;
				}
				floatparse = floatparse->getNext();
			}

			cout << "THE STRING LITERALS:" << endl;
			Token* stringparse = tokens.getFirst();
			while (stringparse){
				if (stringparse->getStringType() == T_StringLiteral) {
					cout << stringparse->getStringRep() << endl;
				}
				stringparse = stringparse->getNext();
			}

			cout << "THE UNKNOWNS:" << endl;
			Token* unknownparse = tokens.getFirst();
			while (unknownparse){
				if (unknownparse->getStringType() == T_Unknown) {
					cout << unknownparse->getStringRep() << endl;
				}
				unknownparse = unknownparse->getNext();
			}
		}

		//waits for the user to press a button to end the program
		cout << endl << endl;
		system("PAUSE");
	}

	//says goodbye to the user
	else if (userResponse == '3') {
		cout << "Goodbye";
	}

	//user puts in invalid input, we end the program without a nice goodbye
	else {
		cout << "Wrong input, terminating program!";
	}/**/

	return 0;
}
