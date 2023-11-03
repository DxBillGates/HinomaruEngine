
#include "heThread.h"
#include <iostream>

int main()
{
	const he::u32 THREAD_CNT = std::thread::hardware_concurrency();

	he::BinSem sem;
	he::i32 x = 0;
	auto compute = [&]() {
		sem.take();
		++x; 
		std::cout << x << std::endl; 
		sem.give();
	};

	he::thread t1(compute);
	he::thread t2(compute);
	he::thread t3(compute);
	he::thread t4(compute);
	he::thread t5(compute);

	// 呼び出したスレッドが終了するまでmainスレッド待機
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
}
