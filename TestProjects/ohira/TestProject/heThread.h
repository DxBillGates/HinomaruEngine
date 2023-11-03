#ifndef __HE_THREAD_H__
#define __HE_THREAD_H__
#include "./heTypes.h"

namespace he
{
	// セマフォ
	// NOTE: N個のスレッドまで同じデータを共有できるmutex
	template<he::u32 N>
	class Sem
	{
	public:
		Sem() :m_tCnt{ N }{}
		~Sem() {}

		// セマフォを使用する。できないときはスレッドをスリープ
		void take()
		{
			// m_tCntのためのロック
			std::unique_lock<he::thread_mutex> lk(m_mutex);

			// カーネルが誤ってスレッドを起こす可能性があるためループ
			while (m_tCnt == 0)
			{
				// NOTE: wait中はアンロックされ、waitが解除されるとロックが再開するのでデッドロックの心配はない
				m_cond.wait(lk);
			}

			// セマフォを1つ使用
			--m_tCnt;
		}

		// セマフォを解放する
		// NOTE: take()を実行しないままに実行されることを想定しない
		void give()
		{
			// m_tContのためのロック
			std::unique_lock<he::thread_mutex> lk(m_mutex);

			// カウンタが0ならロック中のスレッドを1つ起こす
			if (m_tCnt == 0)
			{
				m_cond.notify_one();
			}

			// セマフォを1つ解放
			++m_tCnt;
		}
	private:
		he::u32 m_tCnt;
		he::thread_mutex m_mutex;
		he::thread_cond m_cond;
		// TODO: padding
	};

	// バイナリセマフォ
	using BinSem = Sem<1>;

}
#endif // __HE_THREAD_H__
