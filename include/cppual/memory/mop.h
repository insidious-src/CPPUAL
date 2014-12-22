/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_MEMORY_OPERATIONS_H_
#define CPPUAL_MEMORY_OPERATIONS_H_
#ifdef __cplusplus

#include <cstddef>
#include <cppual/decl.h>

namespace cppual { namespace Memory {

// Memory operations for processors with either pre-increment only
// or pre-increment + post-increment instruction set.
// Manual increment/decrement is NOT supported.

/****************************** byte operations ******************************/

inline void* directCopy (void* pDest, cvoid* pSrc, std::size_t uSize) noexcept
{
	if (!pDest or !pSrc or !uSize) return nullptr;

	byte* pDest8 = static_cast<byte*> (pDest);
	byte* pSrc8  = static_cast<byte*> (pSrc);

	// for pre-increment/decrement only
	--pDest8;
	--pSrc8;

	do *++pDest8 = *++pSrc8;
	while (--uSize);
	return pDest;
}

inline void* directSafeCopy (void* pDest, cvoid* pSrc, std::size_t uSize) noexcept
{
	if (!pDest or !pSrc or !uSize) return nullptr;

	byte* pDest8 = static_cast<byte*> (pDest);
	byte* pSrc8  = static_cast<byte*> (pSrc);

	// for pre-increment/decrement only
	--pDest8;
	--pSrc8;

	// origin overlap check
	if (static_cast<std::size_t> (pDest8 - pSrc8) < uSize) pSrc8 += uSize;

	do *++pDest8 = *++pSrc8;
	while (--uSize);
	return pDest;
}

inline void directZero (void* pSrc, std::size_t uSize) noexcept
{
	if (!pSrc or !uSize) return;
	byte* pSrc8 = static_cast<byte*> (pSrc);

	// for pre-increment/decrement only
	--pSrc8;
	do *++pSrc8 = 0;
	while (--uSize);
}

// =========================================================

template <typename T>
inline T* arrayCopy (T* pDest, const T* pSrc, std::size_t uElemNum) noexcept
{
	if (!pDest or !pSrc or !uElemNum) return nullptr;

	do
	{
		// for pre-increment/decrement only
		--uElemNum;
		*(pDest + (sizeof (T) * uElemNum)) = *(pSrc + (sizeof (T) * uElemNum));
	}
	while (uElemNum);

	return pDest;
}

template <typename T>
inline T* safeArrayCopy (T* pDest, const T* pSrc, std::size_t uElemNum) noexcept
{
	if (!pDest or !pSrc or !uElemNum) return nullptr;
	const std::size_t uSize	= sizeof (T) * uElemNum;

	// origin overlap check
	if (static_cast<std::size_t> (pDest - pSrc) < uSize)
		pSrc += uSize;

	do
	{
		// for pre-increment/decrement only
		--uElemNum;
		*(pDest + (sizeof (T) * uElemNum)) = *(pSrc + (sizeof (T) * uElemNum));
	}
	while (uElemNum);

	return pDest;
}

template <typename T>
inline void arrayZero (T* pSrc, std::size_t uElemNum) noexcept
{
	if (!pSrc or !uElemNum) return;

	do *(pSrc += sizeof (T)) = 0;
	while (--uElemNum);
}

/****************************** bit operations ******************************/

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_OPERATIONS_H_
