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

#include <cppual/gfx/matrix.h>

namespace cppual { namespace Graphics {

UniformMatrix<2>& UniformMatrix<2>::transpose () noexcept
{
	std::swap (m_matrix[1], m_matrix[2]);
	return *this;
}

// =========================================================

UniformMatrix<3>& UniformMatrix<3>::transpose () noexcept
{
	std::swap (m_matrix[1],  m_matrix[3]);
	std::swap (m_matrix[2],  m_matrix[6]);
	std::swap (m_matrix[5],  m_matrix[7]);

	return *this;
}

// =========================================================

UniformMatrix<4>& UniformMatrix<4>::transpose () noexcept
{
	std::swap (m_matrix[1],  m_matrix[4]);
	std::swap (m_matrix[2],  m_matrix[8]);
	std::swap (m_matrix[3],  m_matrix[12]);
	std::swap (m_matrix[6],  m_matrix[9]);
	std::swap (m_matrix[7],  m_matrix[13]);
	std::swap (m_matrix[11], m_matrix[14]);

	return *this;
}

} } // namespace Graphics
