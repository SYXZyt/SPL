#pragma once
#include <map>
#include <ostream>

#include "String.h"
#include "Constants.h"

enum SPL_ERROR_CODE : unsigned short
{
	//Note that the W prefix means warning
	//Generic errors
	SPL_STACKOVERFLOW = 9001,
	SPL_ERROR_INVALID = 9002,

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

	//Parser errors
	SPL_MISSING_ACCESS_MODIFIER = 1201,
	SPL_VARNAME_NOT_IDEN = 1202,
	SPL_VARNAME_IS_KEY = 1203,
	SPL_FREE_NOT_VAR = 1204,
	SPL_EXPRESSION = 1205,
	SPL_UNEXPECTED_KEYWORD = 1206,
	SPL_UNEXPECTED_TOKEN = 1207,
	SPL_INVALID_CONDITIONAL = 1208,
	SPL_EMPTY_BLOCK = 1209,

	//Semantic analyser
	SPL_OUT_OF_FILE = 1301,
	WSPL_UNUSED_VAR = 1302,
	SPL_CONST_OVERWRITE = 1304,
	SPL_REF_VAR = 1305,
	SPL_POP_EMPTY = 1306,
	SPL_SETPOP_STACK_EMPTY = 1307,
	SPL_RET_EMPTY = 1308,
	WSPL_UNREACHABLE = 1309,

	//VM
	SPL_UNKNOWN_VAR = 1401,
	SPL_STRING_UNEXPECTED = 1402,
	SPL_DIV_ZERO = 1404,
	SPL_CONVERSION_ERROR = 1405,
	SPL_UNKNOWN_OPCODE = 1406,
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

	{SPL_UNTERMINATED_STRING, "Unterminated string. Did you forget an end quote?"},
	{SPL_UNRECOGNISED_ESCAPE, "'&SPL_0' is not a recognised escape sequence"},
	{SPL_MALFORMED_NUMERIC, "Number was not a valid numeric format"},
	{SPL_ILLEGAL_CHARACTER, "Illegal character '&SPL_0' found"},

	{SPL_LABEL_NAME_MISSING, "Expected an identifier after a label definition"},
	{SPL_MISSING_NEGATIVE_NUMBER, "Expected a numerical value to follow '-'"},
	{SPL_NONEXISTANT_LABEL, "The label '&SPL_0' does not exist"},
	{SPL_CALL_INVALID_ADDRESS, "'call' expects a label name"},
	{SPL_GOTO_INVALID_ADDRESS, "'goto' expects a line number or label name"},

	{SPL_MISSING_ACCESS_MODIFIER, "Expected 'const' or 'mut' keyword"},
	{SPL_VARNAME_NOT_IDEN, "Expected a variable name"},
	{SPL_VARNAME_IS_KEY, "Variable name cannot be a keyword"},
	{SPL_FREE_NOT_VAR, "'free' keyword expected a variable name to follow"},
	{SPL_EXPRESSION, "Expected an expression"},
	{SPL_UNEXPECTED_KEYWORD, "Keyword '&SPL_0' was unexpected at this time"},
	{SPL_UNEXPECTED_TOKEN, "'&SPL_0' was unexpected at this time"},
	{SPL_INVALID_CONDITIONAL, "'&SPL_0' cannot be used in a condition"},
	{SPL_EMPTY_BLOCK, "SPL does not support empty code blocks. Please use the 'pass' keyword instead"},

	{SPL_OUT_OF_FILE, "Tried to find code that is outside the file"},
	{WSPL_UNUSED_VAR, "Variable has been reassigned, despite not being reference. Try removing the original assignment"},
	{SPL_CONST_OVERWRITE, "Tried to overwrite a constant value"},
	{SPL_REF_VAR, "Tried to reference a variable that does not exist"},
	{SPL_POP_EMPTY, "Tried to pop an empty stack"},
	{SPL_SETPOP_STACK_EMPTY, "Tried to set a value off of the stack while it was empty"},
	{SPL_RET_EMPTY, "Cannot return when the callstack is empty. Are you using 'goto' and not 'call'?"},
	{WSPL_UNREACHABLE, "Unreachable code detected"},

	{SPL_UNKNOWN_VAR, "Variable '&SPL_0' does not exist"},
	{SPL_STRING_UNEXPECTED, "String cannot be used in '&SPL_0' calculation"},
	{SPL_DIV_ZERO, "Tried to divide by zero"},
	{SPL_CONVERSION_ERROR, "Cannot convert value to &SPL_0"},
	{SPL_UNKNOWN_OPCODE, "Unknown opcode '0x&SPL_0'"}
};