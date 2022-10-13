#pragma once
#include <map>

#include "String"

enum SPL_CONSOLE_SUBOP
{
	CNS_SETPOS,
	CNS_CLEAR,
	CNS_SETCOLOR,
	CNS_TITLE,
	CNS_CURSOR,
};

static std::map<const SPL_CONSOLE_SUBOP, std::string> ConsoleSubOp
{
	{CNS_SETPOS, "setpos"},
	{CNS_CLEAR, "clear"},
	{CNS_SETCOLOR, "setcolor"},
	{CNS_TITLE, "title"},
	{CNS_CURSOR, "cursor"},
};