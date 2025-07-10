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

#include <cppual/concept/concepts.h>

// =========================================================

namespace cppual {

// =========================================================

/**
 * @brief Iterator for frope
 * Provides bidirectional iteration over rope characters
 */
template <class_t Rope>
class frope_iterator
{
public:
    // Standard iterator type definitions
    typedef frope_iterator<Rope>            self_type          ;
    typedef std::remove_reference_t<Rope>   buf_type           ;
    typedef std::add_const_t<buf_type>      const_buf          ;
    typedef buf_type *                      buf_pointer        ;
    typedef buf_type const*                 buf_const_pointer  ;
    typedef buf_type &                      buf_reference      ;
    typedef buf_type const&                 buf_const_reference;
    typedef remove_cref_t<buf_type>         buf_clean_type     ;
    typedef typename Rope::pointer          pointer            ;
    typedef typename Rope::const_pointer    const_pointer      ;
    typedef typename Rope::reference        reference          ;
    typedef typename Rope::const_reference  const_reference    ;
    typedef typename Rope::difference_type  difference_type    ;
    typedef typename Rope::size_type        size_type          ;
    typedef size_type const                 const_size         ;
    typedef typename Rope::value_type       value_type         ;
    typedef value_type const                const_value        ;
    typedef std::bidirectional_iterator_tag iterator_category  ;
    typedef buf_clean_type::list_node       list_node          ;
    typedef buf_clean_type::tree_node       tree_node          ;

    template <typename U>
    using self_type_t = frope_iterator<U>;

    typedef std::conditional_t<std::is_const_v<buf_type>, const_reference, reference> elem_ref;
    typedef std::conditional_t<std::is_const_v<buf_type>, const_pointer  , pointer  > elem_ptr;

    union node_union
    {
        struct { list_node* node { }; } list;
        struct { tree_node* node { }; } tree;
    };

    friend class frope_iterator<buf_type const>;
    friend class frope_iterator<buf_clean_type>;

private:

    buf_pointer _M_pBuf     { };    // Parent rope
    size_type   _M_uPos     { };    // Position in entire rope
    node_union  _M_current  { };
    size_type   _M_node_pos { };    // Position within current node

public:
    /**
     * @brief Construct iterator at specific position
     */
    constexpr frope_iterator (buf_reference rope, const_size pos = size_type ())
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
    constexpr frope_iterator (self_type_t<const_buf> const& other) noexcept
    : _M_pBuf     (const_cast<buf_pointer> (other._M_pBuf))
    , _M_uPos     (other._M_uPos    )
    , _M_current  (other._M_current )
    , _M_node_pos (other._M_node_pos)
    { }

    //! converting a non-const iterator to a const iterator
    constexpr frope_iterator (self_type_t<buf_clean_type> const& other) noexcept
    : _M_pBuf     (const_cast<buf_pointer> (other._M_pBuf))
    , _M_uPos     (other._M_uPos    )
    , _M_current  (other._M_current )
    , _M_node_pos (other._M_node_pos)
    { }

    //! converting a non-const iterator to a const iterator
    constexpr self_type& operator = (self_type_t<buf_clean_type> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_pBuf     = other._M_pBuf    ;
        _M_uPos     = other._M_uPos    ;
        _M_current  = other._M_current ;
        _M_node_pos = other._M_node_pos;

        return *this;
    }

    /**
     * @brief Dereference operator
     */
    constexpr elem_ref operator * () const
    {
        return _M_pBuf->is_tree () ? _M_current.tree.node->data[_M_node_pos]
                                   : _M_current.list.node->data[_M_node_pos];
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
        if (!_M_pBuf || _M_uPos >= _M_pBuf->size()) return *this;

        ++_M_uPos;
        ++_M_node_pos;

        if (_M_pBuf->is_tree ())
        {
            if (_M_node_pos >= _M_current.tree.node->data.size ())
            {
                advance_tree_node ();
            }
        }
        else
        {
            if (_M_node_pos >= _M_current.list.node->data.size ())
            {
                _M_current.list.node = _M_current.list.node->next;
                _M_node_pos          = 0;
            }
        }

        return *this;
    }

    /**
     * @brief Post-increment
     */
    constexpr self_type operator ++ (int)
    {
        frope_iterator temp (*this);

        ++(*this);
        return temp;
    }

    /**
     * @brief Pre-decrement
     */
    constexpr self_type& operator -- ()
    {
        if (!_M_pBuf || _M_uPos <= 0) return *this;

        --_M_uPos;

        if (_M_node_pos > 0)
        {
            --_M_node_pos;
        }
        else
        {
            if (_M_pBuf->is_tree ())
            {
                retreat_tree_node ();
            }
            else
            {
                _M_current.list.node = _M_current.list.node->prev;
                _M_node_pos = _M_current.list.node->data.size () - 1;
            }
        }

        return *this;
    }

    /**
     * @brief Post-decrement
     */
    constexpr self_type operator -- (int)
    {
        frope_iterator temp (*this);

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
    consteval frope_iterator () = default;

    /**
     * @brief Find node containing position in list mode
     */
    constexpr void find_list_position (const_size target_pos)
    {
        _M_current.list.node = _M_pBuf->_M_gBuffer.list.head;
        size_type curr_pos   = size_type ();

        while (_M_current.list.node)
        {
            size_type node_size = _M_current.list.node->data.size ();

            if (curr_pos <= target_pos && target_pos < curr_pos + node_size)
            {
                _M_node_pos = target_pos - curr_pos;
                return;
            }

            curr_pos += node_size;
            _M_current.list.node = _M_current.list.node->next;
        }
    }

    /**
     * @brief Find node containing position in tree mode
     */
    constexpr void find_tree_position (const_size target_pos)
    {
        _M_current.tree.node = _M_pBuf->_M_gBuffer.tree.root;
        size_type curr_pos   = size_type ();

        while (_M_current.tree.node)
        {
            size_type left_size = get_subtree_size (_M_current.tree.node->left);

            if (target_pos < curr_pos + left_size)
            {
                _M_current.tree.node = _M_current.tree.node->left;
            }
            else if (target_pos < curr_pos + left_size + _M_current.tree.node->data.size ())
            {
                _M_node_pos = target_pos - (curr_pos + left_size);
                return;
            }
            else
            {
                curr_pos += left_size + _M_current.tree.node->data.size ();
                _M_current.tree.node = _M_current.tree.node->right;
            }
        }
    }

    /**
     * @brief Move to next node in tree mode
     */
    constexpr void advance_tree_node ()
    {
        if (!_M_current.tree.node->right)
        {
            // No right subtree, go up until we find a parent we haven't visited
            tree_node* prev = _M_current.tree.node;
            _M_current.tree.node = _M_current.tree.node->parent;

            while (_M_current.tree.node && _M_current.tree.node->right == prev)
            {
                prev = _M_current.tree.node;
                _M_current.tree.node = _M_current.tree.node->parent;
            }
        }
        else
        {
            // Find leftmost node in right subtree
            _M_current.tree.node = _M_current.tree.node->right;

            while (_M_current.tree.node->left)
            {
                _M_current.tree.node = _M_current.tree.node->left;
            }
        }

        _M_node_pos = 0;
    }

    /**
     * @brief Move to previous node in tree mode
     */
    constexpr void retreat_tree_node ()
    {
        if (!_M_current.tree.node->left)
        {
            // No left subtree, go up until we find a parent whose right child we came from
            tree_node* prev = _M_current.tree.node;
            _M_current.tree.node = _M_current.tree.node->parent;

            while (_M_current.tree.node && _M_current.tree.node->left == prev)
            {
                prev = _M_current.tree.node;
                _M_current.tree.node = _M_current.tree.node->parent;
            }
        }
        else
        {
            // Find rightmost node in left subtree
            _M_current.tree.node = _M_current.tree.node->left;

            while (_M_current.tree.node->right)
            {
                _M_current.tree.node = _M_current.tree.node->right;
            }
        }
        _M_node_pos = _M_current.tree.node->data.size () - 1;
    }

    /**
     * @brief Calculate size of subtree
     */
    constexpr size_type get_subtree_size (tree_node* node) const
    {
        if (!node) return size_type ();

        return get_subtree_size (node->left) + node->data.size () +
               get_subtree_size (node->right);
    }
};

// =========================================================

} // namespace cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_ROPE_ITERATOR_H_