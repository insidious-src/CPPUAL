/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef CPPUAL_RESOURCE_H_
#define CPPUAL_RESOURCE_H_
#ifdef __cplusplus

namespace cppual {

template <typename> class Disposable { };
class PrivateDisposable { };

enum class ResourceType : unsigned char
{
	Null = 0,
	Device,
	Image,
	Font,
	Macro,
	Texture,
	Shader,
	Program,
	Query,
	Surface,
	Buffer,
	Pixmap,
	Colormap,
	Cursor,
	GlyphCursor,
	Context
};

// =========================================================

template <class Controller, class ID, class DisposableType = PrivateDisposable>
class Resource : public DisposableType
{
public:
	typedef ID         value_type;
	typedef Controller controller;

	constexpr Resource () noexcept = default;
	Resource (Resource const&) = delete;
	Resource& operator = (Resource const&) = delete;

	constexpr controller   connection () const noexcept { return m_pCon; }
	constexpr value_type   id () const noexcept { return m_id; }
	constexpr ResourceType resType () const noexcept { return m_eResType; }
	constexpr bool         isValid () const noexcept { return m_id; }

	constexpr Resource (Controller pCon, value_type id, ResourceType eType) noexcept
	: m_pCon (pCon),
	  m_id (id),
	  m_eResType (eType)
	{ }

	template <class Controller_, class ID_>
	friend constexpr bool operator == (Resource<Controller_, ID_, DisposableType> const&,
									   Resource<Controller_, ID_, DisposableType> const&);

private:
	controller   m_pCon;
	value_type   m_id;
	ResourceType m_eResType;
};

// =========================================================

template <class ID, class DisposableType>
class Resource < void, ID, DisposableType > : public DisposableType
{
public:
	typedef ID   value_type;
	typedef void controller;

	constexpr Resource () noexcept = default;
	Resource (Resource const&) = delete;
	Resource& operator = (Resource const&) = delete;

	constexpr value_type   id      () const noexcept { return m_id;       }
	constexpr ResourceType resType () const noexcept { return m_eResType; }
	constexpr bool         isValid () const noexcept { return m_id;       }

	template <class ID_>
	friend constexpr bool operator == (Resource<void, ID_, DisposableType> const&,
									   Resource<void, ID_, DisposableType> const&);

protected:
	constexpr Resource (value_type id, ResourceType eType) noexcept
	: m_id (id),
	  m_eResType (eType)
	{ }

private:
	value_type   m_id;
	ResourceType m_eResType;
};

// =========================================================

template <class ID, class DisposableType>
constexpr bool operator == (Resource<void, ID, DisposableType> const& gObj1,
							Resource<void, ID, DisposableType> const& gObj2)
{ return gObj1.m_id == gObj2.m_id and gObj1.m_eResType == gObj2.m_eResType; }

template <class ID, class DisposableType>
constexpr bool operator != (Resource<void, ID, DisposableType> const& gObj1,
							Resource<void, ID, DisposableType> const& gObj2)
{ return !(gObj1 == gObj2); }

template <class Controller, class ID>
constexpr bool operator == (Resource<Controller, ID> const& gObj1,
							Resource<Controller, ID> const& gObj2)
{
	return gObj1.m_pCon == gObj2.m_pCon and gObj1.m_id == gObj2.m_id and
			gObj1.m_eResType == gObj2.m_eResType;
}

template <class Controller, class ID>
constexpr bool operator != (Resource<Controller, ID> const& gObj1,
							Resource<Controller, ID> const& gObj2)
{ return !(gObj1 == gObj2); }

} // cppual

#endif // __cplusplus
#endif // CPPUAL_GFX_RESOURCE_H_
