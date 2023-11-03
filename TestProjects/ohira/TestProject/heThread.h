#ifndef __HE_THREAD_H__
#define __HE_THREAD_H__
#include "./heTypes.h"

namespace he
{
	// �Z�}�t�H
	// NOTE: N�̃X���b�h�܂œ����f�[�^�����L�ł���mutex
	template<he::u32 N>
	class Sem
	{
	public:
		Sem() :m_tCnt{ N }{}
		~Sem() {}

		// �Z�}�t�H���g�p����B�ł��Ȃ��Ƃ��̓X���b�h���X���[�v
		void take()
		{
			// m_tCnt�̂��߂̃��b�N
			std::unique_lock<he::thread_mutex> lk(m_mutex);

			// �J�[�l��������ăX���b�h���N�����\�������邽�߃��[�v
			while (m_tCnt == 0)
			{
				// NOTE: wait���̓A�����b�N����Await�����������ƃ��b�N���ĊJ����̂Ńf�b�h���b�N�̐S�z�͂Ȃ�
				m_cond.wait(lk);
			}

			// �Z�}�t�H��1�g�p
			--m_tCnt;
		}

		// �Z�}�t�H���������
		// NOTE: take()�����s���Ȃ��܂܂Ɏ��s����邱�Ƃ�z�肵�Ȃ�
		void give()
		{
			// m_tCont�̂��߂̃��b�N
			std::unique_lock<he::thread_mutex> lk(m_mutex);

			// �J�E���^��0�Ȃ烍�b�N���̃X���b�h��1�N����
			if (m_tCnt == 0)
			{
				m_cond.notify_one();
			}

			// �Z�}�t�H��1���
			++m_tCnt;
		}
	private:
		he::u32 m_tCnt;
		he::thread_mutex m_mutex;
		he::thread_cond m_cond;
		// TODO: padding
	};

	// �o�C�i���Z�}�t�H
	using BinSem = Sem<1>;

}
#endif // __HE_THREAD_H__
