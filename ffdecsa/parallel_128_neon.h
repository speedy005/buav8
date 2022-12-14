/* FFdecsa -- fast decsa algorithm
 *
 * Copyright (C) 2007 Dark Avenger
 *               2003-2004  fatih89r
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

# include <arm_neon.h>

#define MEMALIGN_VAL 16

union __u128i {
	unsigned int u[4];
	uint64x2_t v;
};

static const union __u128i ff0 = {{0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U}};
static const union __u128i ff1 = {{0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU}};

typedef uint64x2_t group;
#define GROUP_PARALLELISM 128
#define FF0() ff0.v
#define FF1() ff1.v
#define FFAND(a,b) vandq_u64((a),(b))
#define FFOR(a,b)  vorrq_u64((a),(b))
#define FFXOR(a,b) veorq_u64((a),(b))
#define FFNOT(a)   vreinterpretq_u64_u8(vmvnq_u8(vreinterpretq_u8_u64(a)))


/* BATCH */

static const union __u128i ff29 = {{0x29292929U, 0x29292929U, 0x29292929U, 0x29292929U}};
static const union __u128i ff02 = {{0x02020202U, 0x02020202U, 0x02020202U, 0x02020202U}};
static const union __u128i ff04 = {{0x04040404U, 0x04040404U, 0x04040404U, 0x04040404U}};
static const union __u128i ff10 = {{0x10101010U, 0x10101010U, 0x10101010U, 0x10101010U}};
static const union __u128i ff40 = {{0x40404040U, 0x40404040U, 0x40404040U, 0x40404040U}};
static const union __u128i ff80 = {{0x80808080U, 0x80808080U, 0x80808080U, 0x80808080U}};

typedef uint64x2_t batch;
#define BYTES_PER_BATCH 16
#define B_FFN_ALL_29() ff29.v
#define B_FFN_ALL_02() ff02.v
#define B_FFN_ALL_04() ff04.v
#define B_FFN_ALL_10() ff10.v
#define B_FFN_ALL_40() ff40.v
#define B_FFN_ALL_80() ff80.v

#define B_FFAND(a,b) FFAND(a,b)
#define B_FFOR(a,b)  FFOR(a,b)
#define B_FFXOR(a,b) FFXOR(a,b)
#define B_FFSH8L(a,n) vshlq_n_u64((a),(n))
#define B_FFSH8R(a,n) vshrq_n_u64((a),(n))

#define M_EMPTY()

#undef BEST_SPAN
#define BEST_SPAN            16

#undef XOR_BEST_BY
inline static void XOR_BEST_BY(unsigned char *d, unsigned char *s1, unsigned char *s2)
{
	uint64x2_t vs1 = vld1q_u64((uint64_t*)s1);
	uint64x2_t vs2 = vld1q_u64((uint64_t*)s2);
	vs1 = veorq_u64(vs1, vs2);
	vst1q_u64((uint64_t*)d, vs1);
}

#include "fftable.h"
