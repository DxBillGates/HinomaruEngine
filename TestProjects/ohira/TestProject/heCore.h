#ifndef __HE_CORE_H__
#define __HE_CORE_H__
#include "heTypes.h"
#include <cstddef>

// if�����g�p���Ȃ�3�����Z�q�̃e�X�g
// NOTE: CPU�̃p�C�v���C�������������̂��߂ɕ���ˑ�������������
template <typename T>
T terneryOperator8(he::bl cond, T a, T b)
{
	static_assert(sizeof(T) == 1);

	// cond=1�̎�mask=0xff, cond=0�̎�mask=0x00
	he::u8 mask = 0U - static_cast<he::u8>(cond);
	
	// T��float�̏ꍇ�̂��߂�
	union U8T8
	{
		he::u8 _u8;
		T _t;
	};
	U8T8 ua, ub, ur;
	ua._t = a;
	ub._t = b;
	ur._u8 = (ua._u8 & mask) | (ub._u8 & (~mask));
	return ur._t;
}

template <typename T>
T terneryOperator16(he::bl cond, T a, T b)
{
	static_assert(sizeof(T) == 2);

	// cond=1�̎�mask=0xffff, cond=0�̎�mask=0x0000
	he::u16 mask = 0U - static_cast<he::u16>(cond);

	// T��float�̏ꍇ�̂��߂�
	union U16T16
	{
		he::u16 _u16;
		T _t;
	};
	U16T16 ua, ub, ur;
	ua._t = a;
	ub._t = b;
	ur._u16 = (ua._u16 & mask) | (ub._u16 & (~mask));
	return ur._t;
}

template <typename T>
T terneryOperator32(he::bl cond, T a, T b)
{
	static_assert(sizeof(T) == 4);

	// cond=1�̎�mask=0xffffffff, cond=0�̎�mask=0x00000000
	he::u32 mask = 0U - static_cast<he::u32>(cond);

	// T��float�̏ꍇ�̂��߂�
	union U32T32
	{
		he::u32 _u32;
		T _t;
	};
	U32T32 ua, ub, ur;
	ua._t = a;
	ub._t = b;
	ur._u32 = (ua._u32 & mask) | (ub._u32 & (~mask));
	return ur._t;
}

template <typename T>
T terneryOperator64(he::bl cond, T a, T b)
{
	static_assert(sizeof(T) == 8);

	// cond=1�̎�mask=0xffffffffffffffff, 
	// cond=0�̎�mask=0x0000000000000000
	he::u64 mask = 0U - static_cast<he::u64>(cond);

	// T��float�̏ꍇ�̂��߂�
	union U64T64
	{
		he::u64 _u64;
		T _t;
	};
	U64T64 ua, ub, ur;
	ua._t = a;
	ub._t = b;
	ur._u64 = (ua._u64 & mask) | (ua._u64 & (~mask));
	return ur._t;
}
#endif // __HE_CORE_H__