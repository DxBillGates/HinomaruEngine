#include "Test.h"

#include "../../Header/Debug/ClashHandler.h"

#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include "../../Header/Util/Utility.h"

namespace GE
{
	LONG ClashHandler(EXCEPTION_POINTERS* pExceptInfo)
	{
		Utility::Printf("application clashed\n");
		Utility::Printf("output log...\n");
		Utility::Printf("[call stack]\n");

		CONTEXT context = *pExceptInfo->ContextRecord;

		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		STACKFRAME64 stackFrame = {};

		DWORD machineType = 0;

#if defined(_M_X64)
		machineType = IMAGE_FILE_MACHINE_AMD64;
		stackFrame.AddrPC.Offset = context.Rip;
		stackFrame.AddrStack.Offset = context.Rsp;
		stackFrame.AddrFrame.Offset = context.Rbp;
#else defined(_M_IX86)
#endif

		stackFrame.AddrPC.Mode = AddrModeFlat;
		stackFrame.AddrStack.Mode = AddrModeFlat;
		stackFrame.AddrFrame.Mode = AddrModeFlat;

		// 最大64スタック、取得出来なくなるまでループ
		const int MAX_STACK_NUM = 64;
		for (int i = 0; i < MAX_STACK_NUM; ++i)
		{
			DWORD64 address = 0;

			bool isGet = StackWalk(
				machineType,
				process,
				thread,
				&stackFrame,
				&context,
				nullptr,
				SymFunctionTableAccess,
				SymGetModuleBase,
				nullptr
			);

			if (!isGet || stackFrame.AddrPC.Offset == 0)
			{
				break;
			}

			address = stackFrame.AddrPC.Offset;

			// モジュール読み込めてるか確認する
			{
				IMAGEHLP_MODULE module = {};
				module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
				if (SymGetModuleInfo(process, address, &module))
				{
					Utility::Printf("module:%s, type:%lu", module.ModuleName, module.SymType);
				}
			}

			// 関数名、アドレス、ファイル名、行番号を出力する
			char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
			SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(buffer);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;

			DWORD64 displacement = 0;
			if (SymFromAddr(process, address, &displacement, symbol))
			{
				Utility::Printf("%24s + 0x%08x", symbol->Name, displacement);
			}

			IMAGEHLP_LINE lineInfo = {};
			lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE);

			DWORD lineDisplacement = 0;
			if (SymGetLineFromAddr64(process, address, &lineDisplacement, &lineInfo))
			{
				Utility::Printf("    %s(%lu)\n", lineInfo.FileName, lineInfo.LineNumber);
			}
			else
			{
				Utility::Printf("%\n");
			}
		}

		Utility::Printf("exit application...\n");

		return EXCEPTION_EXECUTE_FAULT;
	}

	void EnableClashHandler()
	{
		SymInitialize(GetCurrentProcess(), nullptr, true);
		SetUnhandledExceptionFilter(ClashHandler);
	}

	void DisableClashHandler()
	{
		SymCleanup(GetCurrentProcess());
		SetUnhandledExceptionFilter(nullptr);
	}
}

Void he::TestRun()
{
}
