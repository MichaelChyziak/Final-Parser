#ifndef DATATYPES_H_
#define DATATYPES_H_

//Use only the following libraries:
#include <string>
#include <cctype>

using namespace std;

namespace ensc251{

	//tables
	extern bool isCommented;
	const char tableOfNaturalNumbers[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	const string tableOfOperators[] = { "+", "-", "/", "*", "=", "<", ">", "++", "--", "<<", ">>", "!=", "&", "&&", "|", "||", "[", "]", "==" };
	const char tableOfPunctuators[] = { ';', ',', '.', '(', ')', '{', '}' };
	const string tableOfKeywords[] = { "auto", "bool", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
		"enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
		"struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "asm", "_ss", "interrupt", "_cs", "cdecl",
		"near", "_ds", "far", "pascal", "_es", "huge", "catch", "class", "delete", "friend", "inline", "new", "operator", "overload",
		"private", "protected", "public", "template", "this", "throw", "try", "virtual" };
	const string booleanValues[] = { "true", "false" };

	//custom data type:
	enum DataType {

	};

#endif /* DATATYPES_H_ */
