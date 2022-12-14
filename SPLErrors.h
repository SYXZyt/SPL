#pragma once
#include <map>
#include <ostream>

#include "String.h"

enum SPL_ERROR_CODE : unsigned short
{
	//Note that the W prefix means warning
	//Generic errors
	SPL_STACKOVERFLOW = 9001,
	SPL_ERROR_INVALID = 9002,
	SPL_USER_ERROR = 9003,

	//Lexer errors
	SPL_UNTERMINATED_STRING = 1001,
	SPL_UNRECOGNISED_ESCAPE = 1002,
	SPL_MALFORMED_NUMERIC = 1003,
	SPL_ILLEGAL_CHARACTER = 1004,

	//Preprocessor errors
	SPL_LABEL_NAME_MISSING = 1101,
	SPL_MISSING_NEGATIVE_NUMBER = 1102,
	SPL_NONEXISTANT_LABEL = 1103,
	SPL_CALL_INVALID_ADDRESS = 1104,
	SPL_GOTO_INVALID_ADDRESS = 1105,
	SPL_LABEL_CANNOT_BE_KEYWORD = 1106,
	SPL_LABEL_NO_START = 1107,

	//Parser errors
	SPL_MISSING_ACCESS_MODIFIER = 1201,
	SPL_VARNAME_NOT_IDEN = 1202,
	SPL_VARNAME_IS_KEY = 1203,
	SPL_FREE_NOT_VAR = 1204,
	SPL_EXPRESSION = 1205,
	SPL_UNEXPECTED_KEYWORD = 1206,
	SPL_UNEXPECTED_TOKEN = 1207,
	SPL_CONST_NOT_LITERAL = 1208,
	SPL_INVALID_CONSOLE_OP = 1209,
	SPL_SLEEP_NO_INT = 1210,
	SPL_RANDOM_NO_INT = 1211,
	SPL_EXIT_NO_INT = 1212,
	SPL_UNEXPECTED_IDEN = 1213,

	//Assembler
	SPL_NO_CODE = 1301,
	SPL_POP_EMPTY = 1302,
	SPL_RET_EMPTY = 1303,
	SPL_CONST_OVERWRITE = 1304,
	WSPL_FREEING_CONST = 1305,
	SPL_IDENTIFIER_NOT_FOUND = 1306,
	SPL_UNKNOWN_CONSOLE_OP = 1307,

	//VM
	SPL_UNKNOWN_VAR = 1401,
	SPL_STRING_UNEXPECTED = 1402,
	SPL_DIV_ZERO = 1404,
	SPL_CONVERSION_ERROR = 1405,
	SPL_UNKNOWN_OPCODE = 1406,
	SPL_NOT_ENOUGH_ITEMS = 1407,
	SPL_SETPOS_EXPECTED_INT = 1408,
	SPL_COLOUR_EXPECTED_INT = 1409,
	SPL_CONDITION_WRONG_PARAMS = 1410,
	SPL_CURSOR_MODE_NO_INT = 1411,
	SPL_NO_ITEM = 1412,
};

static std::ostream& operator<<(std::ostream& os, SPL_ERROR_CODE code)
{
	os << "SPL-" << static_cast<int>(code);

	return os;
}

static std::map<const SPL_ERROR_CODE, std::string> ErrorMessages
{
	{SPL_STACKOVERFLOW, "Stack overflow!"},
	{SPL_ERROR_INVALID, "Error code &SPL_0 does not exist"},
	{SPL_USER_ERROR, "User threw error '&SPL_0'"},

	{SPL_UNTERMINATED_STRING, "Unterminated string. Did you forget an end quote?"},
	{SPL_UNRECOGNISED_ESCAPE, "'&SPL_0' is not a recognised escape sequence"},
	{SPL_MALFORMED_NUMERIC, "Number was not a valid numeric format"},
	{SPL_ILLEGAL_CHARACTER, "Illegal character '&SPL_0' found"},

	{SPL_LABEL_NAME_MISSING, "Expected an identifier after a label definition"},
	{SPL_MISSING_NEGATIVE_NUMBER, "Expected a numerical value to follow '-'"},
	{SPL_NONEXISTANT_LABEL, "The label '&SPL_0' does not exist"},
	{SPL_CALL_INVALID_ADDRESS, "'call' expects a label name"},
	{SPL_GOTO_INVALID_ADDRESS, "'&SPL_0' expects a line number or label name"},
	{SPL_LABEL_CANNOT_BE_KEYWORD, "Label name cannot be a keyword"},
	{SPL_LABEL_NO_START, "Label must come first on a line"},

	{SPL_MISSING_ACCESS_MODIFIER, "Expected 'const' or 'mut' keyword"},
	{SPL_VARNAME_NOT_IDEN, "Expected a variable name"},
	{SPL_VARNAME_IS_KEY, "Variable name cannot be a keyword"},
	{SPL_FREE_NOT_VAR, "'free' keyword expected a variable name to follow"},
	{SPL_EXPRESSION, "Expected an expression"},
	{SPL_UNEXPECTED_KEYWORD, "Keyword '&SPL_0' was unexpected at this time"},
	{SPL_UNEXPECTED_TOKEN, "Token '&SPL_0' was unexpected at this time"},
	{SPL_CONST_NOT_LITERAL, "Constant value could not be parsed at compile time"},
	{SPL_INVALID_CONSOLE_OP, "Console sub-operation must be a valid keyword"},
	{SPL_SLEEP_NO_INT, "'sleep' expected an integer to follow"},
	{SPL_RANDOM_NO_INT, "'random' expected an integer to follow"},
	{SPL_EXIT_NO_INT, "The provided expression cannot be used to exit"},
	{SPL_UNEXPECTED_IDEN, "The identifier '&SPL_0' was unexpected"},

	{SPL_NO_CODE, "Cannot jump to line &SPL_0 as no code was found"},
	{SPL_POP_EMPTY, "Tried to pop while the stack was empty"},
	{SPL_RET_EMPTY, "Tried to return while the callstack was empty"},
	{SPL_CONST_OVERWRITE, "Tried to overwrite a constant value"},
	{WSPL_FREEING_CONST, "Freeing a constant will only open its name up for a variable, and not another constant"},
	{SPL_IDENTIFIER_NOT_FOUND, "Could not find variable name '&SPL_0'"},
	{SPL_UNKNOWN_CONSOLE_OP, "Console sub-operation '&SPL_0' is unknown"},

	{SPL_UNKNOWN_VAR, "Variable '&SPL_0' does not exist"},
	{SPL_STRING_UNEXPECTED, "String cannot be used in '&SPL_0' calculation"},
	{SPL_DIV_ZERO, "Tried to divide by zero"},
	{SPL_CONVERSION_ERROR, "Cannot convert value to &SPL_0"},
	{SPL_UNKNOWN_OPCODE, "Unknown opcode '0x&SPL_0'"},
	{SPL_NOT_ENOUGH_ITEMS, "Calculation '&SPL_0' expected items on the stack"},
	{SPL_NO_ITEM, "Calculation '&SPL_0' expected an item on the stack"},
	{SPL_SETPOS_EXPECTED_INT, "Console sub-operation 'setpos' expected two int values"},
	{SPL_COLOUR_EXPECTED_INT, "Console sub-operation 'setcolor' expected an int values"},
	{SPL_CONDITION_WRONG_PARAMS, "Conditional '&SPL_0' expected two values on the stack"},
	{SPL_CURSOR_MODE_NO_INT, "Console sub-operation 'cursor' expected an int value on the stack"},
};