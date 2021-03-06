/* src/vm/jit/mips/atomic.hpp - MIPS atomic instructions

   Copyright (C) 1996-2013
   CACAOVM - Verein zur Foerderung der freien virtuellen Maschine CACAO

   This file is part of CACAO.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

*/


#ifndef MD_ATOMIC_HPP_
#define MD_ATOMIC_HPP_ 1

#include "config.h"

#include <stdint.h>

#include "threads/atomic.hpp"

namespace Atomic_md {

/**
 * An atomic compare and swap for 32-bit integer values.
 *
 * @param p      Pointer to memory address.
 * @param oldval Old value to be expected.
 * @param newval New value to be stored.
 *
 * @return value of the memory location before the store
 */
inline uint32_t compare_and_swap(volatile uint32_t *p, uint32_t oldval, uint32_t newval)
{
	uint32_t result;
	uint32_t temp;

	__asm__ __volatile__ (
		"    .set  push      \n"
		"    .set  mips2     \n"
		"1:                  \n"
		"    ll    %0,%5     \n"
		"    bne   %0,%3,2f  \n"
		"    move  %1,%4     \n"
		"    sc    %1,%2     \n"
		"    .set  pop       \n"
		"    beqz  %1,1b     \n"
		"2:                  \n"
		: "=&r" (result), "=&r" (temp), "=m" (*p)
		: "r" (oldval), "r" (newval), "m" (*p)
		: "memory");

	return result;
}


/**
 * An atomic compare and swap for 64-bit integer values.
 *
 * @param p      Pointer to memory address.
 * @param oldval Old value to be expected.
 * @param newval New value to be stored.
 *
 * @return value of the memory location before the store
 */
inline uint64_t compare_and_swap(volatile uint64_t *p, uint64_t oldval, uint64_t newval)
{
#if SIZEOF_VOID_P == 8
	uint64_t result;
	uint64_t temp;

	__asm__ __volatile__ (
		"    .set  push      \n"
		"    .set  mips2     \n"
		"1:                  \n"
		"    lld   %0,%5     \n"
		"    bne   %0,%3,2f  \n"
		"    move  %1,%4     \n"
		"    scd   %1,%2     \n"
		"    .set  pop       \n"
		"    beqz  %1,1b     \n"
		"2:                  \n"
		: "=&r" (result), "=&r" (temp), "=m" (*p)
		: "r" (oldval), "r" (newval), "m" (*p)
		: "memory");

	return result;
#else
	return Atomic::generic_compare_and_swap(p, oldval, newval);
#endif
}


/**
 * A memory barrier.
 */
inline void memory_barrier(void)
{
	__asm__ __volatile__ ("" : : : "memory");
}


/**
 * A write memory barrier.
 */
inline void write_memory_barrier(void)
{
	__asm__ __volatile__ ("" : : : "memory");
}


/**
 * An instruction barrier.
 */
inline void instruction_barrier(void)
{
	__asm__ __volatile__ ("" : : : "memory");
}

}

#endif // MD_ATOMIC_HPP_


/*
 * These are local overrides for various environment variables in Emacs.
 * Please do not remove this and leave it at the end of the file, where
 * Emacs will automagically detect them.
 * ---------------------------------------------------------------------
 * Local variables:
 * mode: c++
 * indent-tabs-mode: t
 * c-basic-offset: 4
 * tab-width: 4
 * End:
 * vim:noexpandtab:sw=4:ts=4:
 */
