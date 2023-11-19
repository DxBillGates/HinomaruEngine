#pragma once
#include <Windows.h>
#include <stdio.h>

namespace he
{
	static void PrintLog(const char* format, ...)
	{
		char buf[256];
		va_list  args;
		va_start(args, format);
		vsprintf_s(buf, format, args);
		OutputDebugString(buf);
		va_end(args);
	}
}
