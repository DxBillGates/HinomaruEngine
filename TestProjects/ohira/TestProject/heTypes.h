#ifndef __HE_TYPES_H__
#define __HE_TYPES_H__
#include <stdint.h>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace he
{
	// integer types definition
	using u8	= uint8_t;
	using u16	= uint16_t;
	using u32	= uint32_t;
	using u64	= uint64_t;
	using i8	= int8_t;
	using i16	= int16_t;
	using i32	= int32_t;
	using i64	= int64_t;

	// floating point number types definition
	using f32	= float;
	using f64	= double;

	// boolean
	using bl	= bool;

	// byte
	using bt	= uint8_t;

	// thread
	using thread		= std::thread;
	using thread_mutex	= std::mutex;
	using thread_cond	= std::condition_variable;

#define COROUTINE struct
}
#endif // __HE_TYPES_H__
