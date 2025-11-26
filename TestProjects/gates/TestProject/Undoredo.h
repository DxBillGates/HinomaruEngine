#pragma once
#include "CoreTypes.h"
#include <stack>

namespace he
{
	class ICommand
	{
	public:
		virtual Void Do() = 0;
		virtual Void Undo() = 0;
	};

	using DoFunc = Void (ICommand*);

	class Undoredo
	{
	private:
		std::stack<ICommand*> undoList;
		std::stack<ICommand*> redoList;
	private:
		Void Do(std::stack<ICommand*>& popList, std::stack<ICommand*>& pushList, DoFunc* doFunc)
		{
			if (popList.empty())return;

			pushList.push(popList.top());
			doFunc(popList.top());
			popList.pop();
		}
	public:
		// コマンドを発行する
		// param0 pCommand:コマンドのインスタンス
		// param1 isInvokeDo:コマンド発行時にDoを実行するか
		Void IssueCommand(ICommand* pCommand, Bool isInvokeDo)
		{
			while (!redoList.empty())
			{
				delete redoList.top();
				redoList.pop();
			}
			undoList.push(pCommand);

			if (pCommand)
			{
				pCommand->Do();
			}
		}

		// undoを実行
		Void Undo()
		{
			auto func = [](ICommand* pCommand)
			{
				if (!pCommand)return;

				pCommand->Undo();
			};
			Do(undoList, redoList, func);
		}

		// redoを実行
		Void Redo()
		{
			auto func = [](ICommand* pCommand)
			{
				if (!pCommand)return;

				pCommand->Do();
			};
			Do(redoList, undoList, func);
		}
	};
}