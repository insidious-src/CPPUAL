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

#ifndef CPPUAL_GFX_GL_QUERY_H_
#define CPPUAL_GFX_GL_QUERY_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/gfx/gl/glbase.h>

namespace cppual { namespace Graphics { namespace GL {

class Query : public Object
{
public:
	enum class Type : unsigned char
	{
		Undefined = 0,
		SamplesPassed,
		AnySamplesPassed,
		AnySamplesPassedConservative,
		PrimitivesGenerated,
		TransformFeedbackPrimitivesWritten,
		TimeElapsed,
		Timestamp
	};

	Query () noexcept;
	~Query () noexcept;
	u64  get64 () noexcept;
	uint get () noexcept;
	bool isReady () noexcept;
	void beginQuery (Type type) noexcept;
	void beginQueryIndexed (Type type, uint index) noexcept;
	void endQuery () noexcept;
	void queryCounter (Type type) noexcept;

	inline Type queryType () const noexcept
	{ return m_eType; }

private:
	Type m_eType;
	bool m_bHasQuery;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_QUERY_H_
