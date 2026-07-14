/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#ifndef CPPUAL_ROPE_ITERATOR_H_
#define CPPUAL_ROPE_ITERATOR_H_
#ifdef __cplusplus

#include <cppual/concepts>

// =========================================================

namespace cppual {

// =========================================================

/**
 * @brief Iterator for frope
 * Provides bidirectional iteration over rope characters
 */
template <structure T>
class seq_list_iterator
{
public:
    //! standard iterator type definitions
    typedef seq_list_iterator<T>            self_type          ;
    typedef remove_ref_t<T>                 buf_type           ;
    typedef std::add_const_t<buf_type>      const_buf          ;
    typedef remove_const_t<buf_type>        clean_buf          ;
    typedef buf_type *                      buf_pointer        ;
    typedef buf_type const*                 buf_const_pointer  ;
    typedef buf_type &                      buf_reference      ;
    typedef buf_type const&                 buf_const_reference;
    typedef buf_type::pointer               pointer            ;
    typedef buf_type::const_pointer         const_pointer      ;
    typedef buf_type::reference             reference          ;
    typedef buf_type::const_reference       const_reference    ;
    typedef buf_type::difference_type       difference_type    ;
    typedef buf_type::size_type             size_type          ;
    typedef size_type const                 const_size         ;
    typedef buf_type::value_type            value_type         ;
    typedef value_type const                const_value        ;
    typedef std::bidirectional_iterator_tag iterator_category  ;

    static_assert (sequence_container<buf_type>, "seq_list_iterator requires a sequence type");

    template <typename U>
    using self_type_t = seq_list_iterator<U>;

    typedef std::conditional_t<std::is_const_v<buf_type>, const_reference, reference> elem_ref;
    typedef std::conditional_t<std::is_const_v<buf_type>, const_pointer  , pointer  > elem_ptr;

    friend class seq_list_iterator<const_buf>;
    friend class seq_list_iterator<clean_buf>;

private:
    buf_pointer _M_pBuf     { };    // Parent rope
    size_type   _M_uPos     { };    // Position in entire rope
    size_type   _M_node_pos { };    // Position within current node

public:
    /**
     * @brief Construct iterator at specific position
     */
    constexpr seq_list_iterator (buf_reference rope, const_size pos = size_type ())
    : _M_pBuf (&rope)
    , _M_uPos ( pos )
    {
        if (_M_pBuf && pos < _M_pBuf->size ())
        {
            if (_M_pBuf->is_tree ()) find_tree_position (pos);
            else find_list_position (pos);
        }
    }

    //! converting a const iterator to a non-const iterator
    constexpr seq_list_iterator (self_type_t<const_buf> const& other) noexcept
    : _M_pBuf     (const_cast<buf_pointer> (other._M_pBuf))
    , _M_uPos     (other._M_uPos    )
    , _M_node_pos (other._M_node_pos)
    { }

    //! converting a non-const iterator to a const iterator
    constexpr seq_list_iterator (self_type_t<clean_buf> const& other) noexcept
    : _M_pBuf     (other._M_pBuf    )
    , _M_uPos     (other._M_uPos    )
    , _M_node_pos (other._M_node_pos)
    { }

    //! converting a non-const iterator to a const iterator
    constexpr self_type& operator = (self_type_t<clean_buf> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_pBuf     = other._M_pBuf    ;
        _M_uPos     = other._M_uPos    ;
        _M_node_pos = other._M_node_pos;

        return *this;
    }

    /**
     * @brief Dereference operator
     */
    constexpr elem_ref operator * () const
    {
        return (*_M_pBuf->begin () + _M_uPos)[_M_node_pos];
    }

    /**
     * @brief Arrow operator
     */
    constexpr elem_ptr operator -> () const
    {
        return &(operator * ());
    }

    /**
     * @brief Pre-increment
     */
    constexpr self_type& operator ++ ()
    {


        return *this;
    }

    /**
     * @brief Post-increment
     */
    constexpr self_type operator ++ (int)
    {
        self_type temp (*this);

        ++(*this);
        return temp;
    }

    /**
     * @brief Pre-decrement
     */
    constexpr self_type& operator -- ()
    {


        return *this;
    }

    /**
     * @brief Post-decrement
     */
    constexpr self_type operator -- (int)
    {
        seq_list_iterator temp (*this);

        --(*this);
        return temp;
    }

    /**
     * @brief Equality comparison
     */
    constexpr bool operator == (self_type const& other) const noexcept
    {
        return _M_uPos == other._M_uPos;
    }

    /**
     * @brief Inequality comparison
     */
    constexpr bool operator != (self_type const& other) const noexcept
    {
        return !(*this == other);
    }

private:
    consteval seq_list_iterator () = default;
};

// =========================================================

} // namespace cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_ROPE_ITERATOR_H_