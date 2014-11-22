
#ifndef DATATYPES_H_
#define DATATYPES_H_

//Use only the following libraries:
#include "parserClasses.h"
#include <string>
#include <cctype>
#include <vector>


using namespace std;

namespace dataTypes {

	//tables
	const string tableOfTypes[] = { "char", "char16_t", "char32_t", "wchar_t", "signed", "short", "int", "long", "float", "double", "unsigned", "float", "double", "bool", "void", "decltype",
		"nullptr", "nullptr_t", "string" };

	const string tableOfSetsUserTypes[] = { "typedef", "class", "enum", "union", "struct" };

	extern int tableOfSetsUserTypesSize;

	extern int tableOfTypesSize;

	extern vector<string> tableOfUserTypes;

	bool isDataType(const string&); //returns true if the given string is a data type of c++ or user defined data type, false otherwise
	bool isSetsUserType(const string&);
	Token* setDataTypeList(Token* firstToken);
	void setsUserDataTypes(Token* firstToken);

}

#endif /* DATA_TYPES */
