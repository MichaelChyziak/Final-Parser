/*
* lexemesTypes.cpp
*
* Created on: Oct 27, 2014
*
*/

#include "lexemesTypes.h"

namespace ensc251{

	//***Complete the implementation of the following functions: ***

	bool isCommented = false;

	//This function returns TRUE if the input is identifier else FALSE
	bool isIdentifier(const string &lexeme) {
		//http://msdn.microsoft.com/en-us/library/565w213d.aspx	
		//create an array of possible identifier characters and natural numbers, as well as the arrays sizes
		const char identifierChars[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
										'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_' };
		int sizeOfIdentifierChars = 53;
		int sizeOfNaturalNumbers = sizeof(tableOfNaturalNumbers) / sizeof(tableOfNaturalNumbers[0]);
		
		int offset = 0;


		//if the string is a keyword or boolean value, return false (since is not identifier)
		if (isKeyword(lexeme) || isBooleanValue(lexeme)) {
			return false;
		}

		//Checks that the first characters is an identifier Character and not a number or other character
		//if its a identifier character, increase offset by 1
		for (int i = 0; i < sizeOfIdentifierChars; i++) {
			if (lexeme.at(0) == identifierChars[i]) {
				offset++;
				break;
			}
		}

		//checks that the rest of our string is all filled with an allowed character (identifierChars), if it is then increase offset by 1
		for (int i = 1; i < lexeme.length(); i++) {
			for (int j = 0; j < sizeOfIdentifierChars; j++) {
				if (lexeme.at(i) == identifierChars[j]) {
					offset++;
					break;
				}
			}
			for (int j = 0; j < sizeOfNaturalNumbers; j++) {
				if (lexeme.at(i) == tableOfNaturalNumbers[j]) {
					offset++;
					break;
				}
			}
		}
		
		
		//returns true if the offset equals the length of the string
		if (offset > 0 && offset == lexeme.length()) {
			return true;
		}
		return false;
	}

	//This function returns TRUE if the input is one of the operators defined in the variable
	//"tableOfOperators" in "lexemesTypes.h" else it returns FALSE
	bool isOperator(const string &lexeme) { 
		string temp = lexeme; //creates a temporary string variable to hold the value of lexeme
		int sizeOfOperators = sizeof(tableOfOperators) / sizeof(tableOfOperators[0]);

		//compares the temporary string to all of the strings from "tableOfOperators" in "lexemesTypes.h"
		//returns true if the string matches to one of the strings in the array, false otherwise
		for (int i = 0; i < sizeOfOperators; i++) {
			if (temp == tableOfOperators[i]) {
				return true;
			}
		}
		return false;
	}

	//This function returns TRUE if the input is one of the punctuators defined in the variable
	//"tableOfPunctuators" in "lexemesTypes.h" else it returns FALSE
	bool isPunctuator(char lexeme) {
		char temp = lexeme; //creates a temporary char variable to hold the value of lexeme
		int sizeOfPunctuators = sizeof(tableOfPunctuators) / sizeof(tableOfPunctuators[0]);

		//compares the temporary character to all of the characters from "tableOfPunctuators" in "lexemesTypes.h"
		//returns true if the character matches to one of the characters in the array, false otherwise
		for (int i = 0; i < sizeOfPunctuators; i++) {
			if (temp == tableOfPunctuators[i]) {
				return true;
			}
		}
		return false;

	}

	//This function returns TRUE if the input is one of the keywords defined in the variable
	//"tableOfKeywords" in "lexemesTypes.h" else it returns FALSE
	bool isKeyword(const string &lexeme) {
		string temp = lexeme; //creates a temporary string variable to hold the value of lexeme
		int sizeOfKeywords = sizeof(tableOfKeywords) / sizeof(tableOfKeywords[0]);

		//compares the temporary string to all of the strings from "tableOfKeywords" in "lexemesTypes.h"
		//returns true if the string matches to one of the strings in the array, false otherwise
		for (int i = 0; i < sizeOfKeywords; i++) {
			if (temp == tableOfKeywords[i]) {
				return true;
			}
		}
		return false;
	}

	//This function returns TRUE if the input is one of the boolean defined in the variable
	//"booleanValues" in "lexemesTypes.h" else it returns FALSE
	bool isBooleanValue(const string &lexeme) { 
		string temp = lexeme; //creates a temporary string variable to hold the value of lexeme
		int sizeOfBoolValues = sizeof(booleanValues) / sizeof(booleanValues[0]);

		//compares the temporary string to all of the strings from "booleanValues" in "lexemesTypes.h"
		//returns true if the string matches to one of the strings in the array, false otherwise
		for (int i = 0; i < sizeOfBoolValues; i++) {
			if (temp == booleanValues[i]) {
				return true;
			}
		}
		return false;
	}

	//This function returns TRUE if the input is integer literal else FALSE
	bool isIntegerLiteral(const string &lexeme) {
		string temp = lexeme; //creates a temporary string variable to hold the value of lexeme
		//http://en.cppreference.com/w/cpp/language/integer_literal
		bool isInt = false;
		int offset = 0;
		const char hexLetters[] = { 'x', 'X', 'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F' };
		const char integerSuffix[] = { 'u', 'U', 'l', 'L' };
		int sizeOfNaturalNumbers = sizeof(tableOfNaturalNumbers) / sizeof(tableOfNaturalNumbers[0]);
		int sizeOfHexLetters = 14;
		int sizeOfIntegerSuffix = 4;

		//Checks every character of the string to see if it is an IntegerLiteral, if it is then increase offset by 1
		for (int i = 0; i < temp.length(); i++) {
			for (int j = 0; j < sizeOfNaturalNumbers; j++) {
				if (temp.at(i) == tableOfNaturalNumbers[j]) {
					offset++;
					break;
				}
			}
			if (temp.at(0) == '0') {
				for (int j = 0; j < sizeOfHexLetters; j++) {
					if (temp.at(i) == hexLetters[j]) {
						offset++;
						break;
					}
				}
			}
			for (int j = 0; j < sizeOfIntegerSuffix; j++) {
				if (temp.at(i) == integerSuffix[j]) {
					offset++;
					break;
				}
			}
		}

		//make sure that the string is not empty and that the amount of offset integersLiteral cases is equal to the length of the string
		//if so return true, otherwise false
		if (offset > 0 && offset == temp.length()) {
			return true;
		}
		return false;
	}
	//This function returns TRUE if the input is float literal else FALSE
	bool isFloatLiteral(const string &lexeme) {
		string temp = lexeme; //creates a temporary string variable to hold the value of lexeme
		//http://msdn.microsoft.com/en-us/library/tfh6f0w2.aspx
		int offset = 0;
		int sizeOfNaturalNumbers = sizeof(tableOfNaturalNumbers) / sizeof(tableOfNaturalNumbers[0]);
		const char floatingPointCharacters[] = { '.', 'e', 'E', 'l', 'L', 'f', 'F', '+', '-' };
		int floatingPointCharactersSize = 9;

		//Checks every character of the string to see if it is a Float Literal, if it is then increase offset by 1
		for (int i = 0; i < temp.length(); i++) {
			for (int j = 0; j < sizeOfNaturalNumbers; j++) {
				if (temp.at(i) == tableOfNaturalNumbers[j]) {
					offset++;
					break;
				}
			}
			for (int j = 0; j < floatingPointCharactersSize; j++) {
				if (temp.at(i) == floatingPointCharacters[j]) {
					offset++;
					break;
				}
			}
		}
		
		//make sure that the string is not empty and that the amount of offset floatLiteral cases is equal to the length of the string
		//if so return true, otherwise false
		if (offset > 0 && offset == temp.length()) {
			return true;
		}
		return false;
	}
	//This function returns TRUE if the input is string literal else FALSE
	bool isStringLiteral(const string &lexeme) {
		string temp = lexeme; //creates a temporary string variable to hold the value of lexeme
		//http://msdn.microsoft.com/en-us/library/69ze775t.aspx <- this lists the different types of string literals
		if (temp.length() > 1){
			if (temp.at(0) == '"' && temp.at(temp.length() - 1) == '"'){
				return true;
			}
		}
		// if the token is a narrow string variable the token length must be atleast 2 and have a quotation mark at the begining or end of the string
		

		return false;
	}
}
