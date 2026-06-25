#include "../../Header/Debug/ClashHandler.h"

#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <stdio.h>
#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

#include "../../Header/Util/Utility.h"

#include "../resource.h"

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

	struct CallstackInfo
	{
		struct Stacktrace
		{
			char moduleStr[64] = "\0";
			char funcStr[256] = "\0";
			char fileStr[256] = "\0";
		}stacktrace[64];

		int tracedNum;
	};

	void GetCallstack(CallstackInfo* callstackInfo)
	{
		const int MAX_STACK_NUM = 64;
		void* stacktrace[MAX_STACK_NUM];
		callstackInfo->tracedNum = CaptureStackBackTrace(0, MAX_STACK_NUM, stacktrace, nullptr);

		HANDLE process = GetCurrentProcess();

		for (int i = 0; i < callstackInfo->tracedNum; ++i)
		{
			CallstackInfo::Stacktrace* pStacktrace = &callstackInfo->stacktrace[i];

			DWORD64 address = (DWORD64)stacktrace[i];

			// モジュール情報取得（カーネルが呼んでるのかとか
			IMAGEHLP_MODULE module = {};
			module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
			if (SymGetModuleInfo(process, address, &module))
			{
				sprintf_s(pStacktrace->moduleStr, "%s", module.ModuleName);
			}

			// 関数情報取得
			char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
			SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(buffer);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;

			DWORD64 displacement = 0;
			if (SymFromAddr(process, address, &displacement, symbol))
			{
				sprintf_s(pStacktrace->funcStr, "%s", symbol->Name);
			}

			// ファイル名情報取得
			IMAGEHLP_LINE lineInfo = {};
			lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE);

			DWORD lineDisplacement = 0;
			if (SymGetLineFromAddr(process, address, &lineDisplacement, &lineInfo))
			{
				sprintf_s(pStacktrace->fileStr, "%s(%lu)", lineInfo.FileName, lineInfo.LineNumber);
			}
		}
	}

	HWND g_hwnd = nullptr;
	HINSTANCE g_hInstance = nullptr;

	LRESULT EditControlProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR uid, DWORD_PTR refData)
	{
		Utility::Printf("msg:%d, %d, %d\n", msg, wp, lp);

		switch (msg)
		{
		case WM_KEYDOWN:
			if (wp == 'C' && GetKeyState(VK_CONTROL) & 0x8000)
			{
				// アサート文をコピー
				char buf[256];
				GetWindowText(hwnd, buf, 256);
			}

			if (wp == 'A' && GetKeyState(VK_CONTROL) & 0x8000)
			{
				// 全選択
				SendMessage(hwnd, EM_SETSEL, 0, -1);

				Utility::Printf("ctrl+a\n");
			}
			break;
		}

		return DefSubclassProc(hwnd, msg, wp, lp);
	}

	void AssertDialog()
	{
		// モジュール準備終わっているときのみ

		// コールスタックを文字列として取得
		CallstackInfo stackInfo;
		GetCallstack(&stackInfo);

		auto DialogProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)->INT_PTR
			{
				switch (msg)
				{
				case WM_INITDIALOG:
				{
					CallstackInfo* pStackInfo = reinterpret_cast<CallstackInfo*>(lp);

					//char buf[2560] = "\0";
					//char* pBuf = buf;
					//for (int i = 0; i < pStackInfo->tracedNum; ++i)
					//{
					//	int bufCount = 2560 - strlen(buf);
					//	sprintf_s(pBuf, bufCount, "%s %s %s\r\n", pStackInfo->stacktrace[i].moduleStr, pStackInfo->stacktrace[i].funcStr, pStackInfo->stacktrace[i].fileStr);
					//	//sprintf_s(buf, "%s %s %s\n", pStackInfo->stacktrace[i].moduleStr, pStackInfo->stacktrace[i].funcStr, pStackInfo->stacktrace[i].fileStr);
					//	pBuf = buf + strlen(buf);
					//}
					char buf[2560] = "\0";
					sprintf_s(buf, "revision:000000\r\nassert description\r\n");

					SetWindowSubclass(GetDlgItem(hwnd, ID_ASSERT_DESCRIPTION), EditControlProc, 1, 0);

					SetDlgItemText(hwnd, ID_ASSERT_DESCRIPTION, buf);

					return TRUE;
				}
				case WM_COMMAND:
				{
					switch (LOWORD(wp))
					{
					case ID_BUTTON_COPY_DESCRIPTION:
					{
						// アサートをクリップボードに保存する

						return TRUE;
					}
					case ID_BUTTON_COPY_INFOMATION:
					{
						// アサート＋コールスタックをクリップボードに保存する

						return TRUE;
					}
					case ID_BUTTON_COPY_ASSERT_LOG:
					{
						// 直近のアサート情報をクリップボードに保存する

						return TRUE;
					}
					case ID_BUTTON_EXIT:
					{
						// アプリケーションを終了させる

						return TRUE;
					}
					case ID_BUTTON_DUMP:
					{
						// ダンプを出力してアプリケーションを終了させる

						return TRUE;
					}
					case ID_BUTTON_OK:
					{
						// アサート文とコールスタック情報を保存する

						EndDialog(hwnd, 0);
						return TRUE;
					}
					}
				}
				}

				return FALSE;
			};

		DialogBoxParam(
			g_hInstance,
			MAKEINTRESOURCE(ID_ASSERT_DIALOG),
			g_hwnd,
			DialogProc,
			(LPARAM)&stackInfo
		);
	}

	void SetupWindowHandle(HWND hwnd, HINSTANCE hInstance)
	{
		g_hwnd = hwnd;
		g_hInstance = hInstance;
	}
}