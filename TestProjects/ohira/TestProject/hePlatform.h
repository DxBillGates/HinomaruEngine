#ifndef __HE_PLATFORM_H__
#define __HE_PLATFORM_H__
#include "heTypes.h"

namespace he
{
	// endian conversion
	he::u16 swapU16(he::u16 value)
	{
		return ((value & 0x00FF) << 8) | 
			((value & 0xFF00) >> 8);
	}
	he::u32 swapU32(he::u32 value)
	{
		return ((value & 0x000000FF) << 24) |
			((value & 0x0000FF00) << 8) |
			((value & 0x00FF0000) >> 8) |
			((value & 0xFF000000) >> 24);
	}
	he::u64 swapU64(he::u64 value)
	{
		return ((value & 0x00000000000000FF) << 56) |
			((value & 0x000000000000FF00) << 40) |
			((value & 0x0000000000FF0000) << 24) |
			((value & 0x00000000FF000000) << 8) |
			((value & 0x000000FF00000000) >> 8) |
			((value & 0x0000FF0000000000) >> 24) |
			((value & 0x00FF000000000000) >> 40) |
			((value & 0xFF00000000000000) >> 56);
	}
	he::i16 swapI16(he::i16 value)
	{
		he::u16 cvalue = swapU16(static_cast<he::u16>(value));
		return static_cast<he::i16>(cvalue);
	}
	he::i32 swapI32(he::i32 value)
	{
		he::u32 cvalue = swapU32(static_cast<he::u32>(value));
		return static_cast<he::i32>(cvalue);
	}
	he::i64 swapI64(he::i64 value)
	{
		he::u64 cvalue = swapU64(static_cast<he::u64>(value));
		return static_cast<he::i64>(cvalue);
	}
	he::f32 swapF32(he::f32 value)
	{
		union U32F32
		{
			he::u32 _u32;
			he::f32 _f32;
		};
		U32F32 u;
		u._f32 = value;
		u._u32 = swapU32(u._u32);

		return u._f32;
	}
	he::f64 swapF64(he::f64 value)
	{
		union U64F64
		{
			he::u64 _u64;
			he::f64 _f64;
		};
		U64F64 u;
		u._f64 = value;
		u._u64 = swapU32(u._u64);

		return u._f64;
	}
}

#endif // __HE_PLATFORM_H__