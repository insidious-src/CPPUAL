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

#ifndef CPPUAL_TEXT_ROPE_H_
#define CPPUAL_TEXT_ROPE_H_
#ifdef __cplusplus

#include <cppual/casts>
#include <cppual/string>
#include <cppual/containers>
#include <cppual/memory/allocator.h>
#include <cppual/concepts>
#include <cppual/iterators/rope_iterator.h>

#include <string_view>
#include <iterator>
#include <memory>

// ====================================================

namespace cppual {

// ====================================================

//! Node policy interface
template <char_t T = char, allocator_t A = memory::allocator<T>>
struct default_node_policy
{
    typedef default_node_policy<T, A>                   self_type     ;
    typedef remove_cref_t<T>                            value_type    ;
    typedef std::size_t                                 size_type     ;
    typedef size_type const                             const_size    ;
    typedef memory::allocator<T> allocator_type;
    typedef fstring<value_type, allocator_type>         string_type   ;
    typedef std::basic_string_view<value_type>          string_view   ;

    //! Optimal chunk size for cache line alignment (typically 64 bytes)
    inline constexpr static const_size cache_line_size   =      64;
    inline constexpr static const_size optimal_node_size =    4096;  // Default chunk size
    inline constexpr static const_size list_threshold    =  262144;  // 256KB
    inline constexpr static const_size tree_threshold    = 1048576;  // 1MB

    /**
     * @brief Base node class aligned to cache line
     * Contains common data for both list and tree nodes
     */
    struct alignas (cache_line_size) node
    {
        string_type str;     // Actual string data

        constexpr explicit node (string_type const& data,
                                 allocator_type const& ator = allocator_type ())
        : str (data, ator)
        { }

        template <str_view_like_t U = string_view>
        constexpr explicit node (U const& sv = U (),
                                 allocator_type const& ator = allocator_type ())
        : str (sv, ator)
        { }
    };

    //! Node splitting strategy
    template <typename Node>
    constexpr static bool should_split (Node const* node) noexcept
    {
        return node->str.size () > optimal_node_size;
    }

    //! Node merging strategy
    template <typename Node>
    constexpr static bool should_merge (Node const* node1, Node const* node2) noexcept
    {
        return (node1->str.size () + node2->str.size ()) <= optimal_node_size;
    }

    //! Balance checking
    template <typename TreeNode>
    constexpr static bool needs_rebalance (TreeNode const* root) noexcept
    {
        // Default AVL-style balance check
        return std::abs (get_height (root->left) - get_height (root->right)) > 1;
    }

    static node* create_node(const string_type& data) {
        if (data.size() <= list_threshold) {
            return new list_node{data};
        } else if (data.size() <= tree_threshold) {
            tree_node* node = new tree_node{data};
            return node;
        } else {
            btree_node* node = new btree_node();
            node->data[0] = data;
            node->num_keys = 1;
            return node;
        }
    }

    static void delete_node(node* n) {
        if (auto ln = dynamic_cast<list_node*>(n)) {
            delete ln;
        } else if (auto tn = dynamic_cast<tree_node*>(n)) {
            delete_tree(tn);
        } else if (auto bn = dynamic_cast<btree_node*>(n)) {
            delete_btree(bn);
        }
    }

private:
    //! Mode tracking
    enum class mode { list, tree, btree };

    /**
     * @brief List node optimized for cache access
     * Most frequently accessed members placed at start
     */
    struct alignas (cache_line_size) list_node : node
    {
        using node::node;

        list_node* next { };       // Next node pointer (hot field)
        list_node* prev { };       // Previous node pointer (hot field)
    };

    /**
     * @brief Tree node for large text handling
     * Used only when text size exceeds list_threshold
     */
    struct alignas (cache_line_size) tree_node : node
    {
        using node::node;

        tree_node* next   { };
        tree_node* prev   { };
        tree_node* parent { };
    };

    struct alignas (cache_line_size) btree_node : node
    {
        using node::node;

        btree_node* next        { };
        btree_node* prev        { };
        btree_node* parent      { };
        btree_node* first_child { };
    };

    /**
     * @brief Union of storage for list/tree modes
     * Saves memory by sharing space between modes
     */
    union buffer final
    {
        struct
        {
            list_node* head;   // First node in list
            list_node* tail;   // Last node in list
        }
        list;

        struct
        {
            tree_node* root;   // Root of tree
        }
        tree;

        struct
        {
            btree_node* root;   // Root of btree
        }
        btree;

        consteval buffer () noexcept : list { } { }
    };

    typedef allocator_type::template rebind<list_node >::other list_node_allocator ;
    typedef allocator_type::template rebind<tree_node >::other tree_node_allocator ;
    typedef allocator_type::template rebind<btree_node>::other btree_node_allocator;

    //! Current mode tracking
    constexpr static mode get_mode (const_size total_size) noexcept
    {
        return total_size <= list_threshold ? mode::list :
               total_size <= tree_threshold ? mode::tree :
                                              mode::btree;
    }

    //! Mode transition helpers
    constexpr static node* convert_list_to_tree (list_node const* head)
    {
        if (!head) return nullptr;

        auto alloc = list_node_allocator ();

        // Collect nodes for balanced tree creation
        std::vector<tree_node, tree_node_allocator> nodes;
        list_node* curr = head;

        while (curr)
        {
            list_node* to_delete = curr;

            nodes.emplace_back (curr->str);
            curr = curr->next;

            alloc.deallocate (to_delete, 1);
        }

        return build_balanced_tree (nodes.data (), nodes.size ());
    }

    constexpr static node* convert_tree_to_btree (tree_node* root)
    {
        if (!root) return nullptr;

        // Collect nodes in-order
        std::vector<string_type, allocator_type> values;

        collect_inorder (root, values);

        // Clean up old tree
        delete_tree (root);

        // Create B-tree from sorted values
        return build_btree (values);
    }

    // Tree helpers
    constexpr static tree_node* build_balanced_tree (tree_node** nodes, const_size count)
    {
        if (count == 0) return nullptr;

        size_type  mid  = count / 2;
        tree_node* root = nodes[mid];

        root->left = build_balanced_tree (nodes, mid);
        if (root->left) root->left->parent = root;

        root->right = build_balanced_tree (nodes + mid + 1, count - mid - 1);
        if (root->right) root->right->parent = root;

        return root;
    }

    // B-tree helpers
    constexpr static btree_node* build_btree (std::vector<string_type, allocator_type> const& values)
    {
        auto alloc = btree_node_allocator ( );
        btree_node* root = alloc.allocate (1);

        btree_node_allocator::construct (root);

        for (auto const& val : values) insert_into_btree (root, val);
        return root;
    }

    constexpr static void insert_into_btree (btree_node* node, string_type const& value)
    {
        if (node->num_keys < btree_node::b_order)
        {
            // Insert into current node
            const_size pos = find_insert_pos (node, value);
            shift_right (node, pos);
            node->str[pos] = value;
            node->num_keys++;
        }
        else
        {
            // Split node and propagate up
            split_btree_node (node, value);
        }
    }
};

// ====================================================

/**
 ** @brief frope (fast rope) implements a rope data structure using a double-linked list of fstrings
 ** Maintains interface compatibility with std::basic_string and fstring
 **/
template <char_t       T = char,
          allocator_t  A = memory::allocator<T>,
          class_t Policy = default_node_policy<T, A>
          >
class SHARED_API frope : private A
{
public:
    struct node     ;
    struct tree_node;
    struct list_node;

    typedef frope<T, A>                                       self_type             ;
    typedef fstring<T, A>                                     string_type           ;
    typedef std::char_traits<T>                               char_traits           ;
    typedef list<string_type>                                 list_type             ;
    typedef std::allocator_traits<A>                          alloc_traits          ;
    typedef alloc_traits::allocator_type                      allocator_type        ;
    typedef allocator_type::template rebind<list_node>::other list_node_allocator   ;
    typedef allocator_type::template rebind<tree_node>::other tree_node_allocator   ;
    typedef alloc_traits::value_type                          value_type            ;
    typedef alloc_traits::pointer                             pointer               ;
    typedef alloc_traits::const_pointer                       const_pointer         ;
    typedef alloc_traits::reference                           reference             ;
    typedef alloc_traits::const_reference                     const_reference       ;
    typedef alloc_traits::size_type                           size_type             ;
    typedef size_type const                                   const_size            ;
    typedef frope_iterator<self_type>                         iterator              ;
    typedef frope_iterator<self_type const>                   const_iterator        ;
    typedef std::reverse_iterator<iterator>                   reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>             const_reverse_iterator;
    typedef std::basic_string_view<value_type>                string_view           ;
    typedef string_type::difference_type                      difference_type       ;

    static_assert (std::is_same_v<T, value_type>, "T & value_type are NOT the same!");

    inline constexpr static const_size npos = size_type (-1);
    //! 256KB threshold for list/tree mode switch
    inline constexpr static const_size list_threshold = 262144;
    //! Optimal chunk size for cache line alignment (typically 64 bytes)
    inline constexpr static const_size cache_line_size = 64;
    //! Node chunk size for batch allocation
    //inline constexpr static const_size node_chunk_size = 16;

    /**
     * @brief Base node class aligned to cache line
     * Contains common data for both list and tree nodes
     */
    struct alignas (cache_line_size) node
    {
        string_type str;     // Actual string data

        constexpr explicit node (string_type const& data,
                                 allocator_type const& ator = allocator_type ())
        : str (data, ator)
        { }

        template <str_view_like_t U = string_view>
        constexpr explicit node (U const& sv = U (),
                                 allocator_type const& ator = allocator_type ())
        : str (sv, ator)
        { }
    };

    /**
     * @brief List node optimized for cache access
     * Most frequently accessed members placed at start
     */
    struct alignas (cache_line_size) list_node : node
    {
        using node::node;

        list_node* next { };       // Next node pointer (hot field)
        list_node* prev { };       // Previous node pointer (hot field)
    };

    /**
     * @brief Tree node for large text handling
     * Used only when text size exceeds list_threshold
     */
    struct alignas (cache_line_size) tree_node : node
    {
        using node::node;

        tree_node* next   { };
        tree_node* prev   { };
        tree_node* parent { };
    };

    struct alignas (cache_line_size) btree_node : node
    {
        using node::node;

        btree_node* next        { };
        btree_node* prev        { };
        btree_node* parent      { };
        btree_node* first_child { };
    };

    /**
     * @brief Union of storage for list/tree modes
     * Saves memory by sharing space between modes
     */
    union buffer final
    {
        struct
        {
            list_node* head;   // First node in list
            list_node* tail;   // Last node in list
        }
        list;

        struct
        {
            tree_node* root;   // Root of tree
        }
        tree;

        struct
        {
            btree_node* root;   // Root of btree
        }
        btree;

        consteval buffer () noexcept : list { } { }
    };

    constexpr frope () noexcept = default;

    constexpr frope (self_type const& other)
    : allocator_type (other)
    , _M_uLength     ()
    {
        if (!other.empty ())
        {
            if (other.is_tree ()) copy_tree_from (other._M_gBuffer.tree.root);
            else copy_list_from (other._M_gBuffer.list.head);
        }
    }

    constexpr frope (self_type&& other) noexcept
    : allocator_type (std::move (other))
    , _M_gBuffer (other._M_gBuffer)
    , _M_uLength (other._M_uLength)
    {
        other._M_gBuffer.list = { nullptr, nullptr };
        other._M_uLength      = size_type ();
    }

    constexpr explicit frope (string_type const& str, allocator_type const& ator = allocator_type ())
    : allocator_type (ator)
    , _M_uLength (str.size ())
    {
        if (!str.empty ())
        {
            if (is_tree ()) _M_gBuffer.tree.root = allocate_tree_node (str);
            else _M_gBuffer.list.head = _M_gBuffer.list.tail = allocate_list_node (str);
        }
    }

    template <str_view_like_t U>
    constexpr frope (U const& sv, allocator_type const& ator = allocator_type ()) noexcept
    : allocator_type (ator)
    , _M_uLength (sv.size ())
    {
        if (!sv.empty ())
        {
            if (is_tree ()) _M_gBuffer.tree.root = allocate_tree_node (sv);
            else _M_gBuffer.list.head = _M_gBuffer.list.tail = allocate_list_node (sv);
        }
    }

    constexpr explicit frope (const_pointer pchar, allocator_type const& ator = allocator_type ()) noexcept
    : self_type (string_view (pchar), ator)
    { }

    template <iterator_t Iter>
    constexpr frope (Iter first, Iter last, allocator_type const& ator = allocator_type ()) noexcept
    : allocator_type (ator)
    , self_type (string_view (first, last))
    { }

    constexpr frope (std::initializer_list<value_type> list,
                     allocator_type const& ator = allocator_type ()) noexcept
    : self_type (list.begin (), list.end (), ator)
    { }

    constexpr frope (allocator_type const& ator) noexcept
    : allocator_type (ator), self_type ()
    { }

    constexpr frope (std::nullptr_t) noexcept
    : allocator_type (), self_type ()
    { }

    inline ~frope ()
    { clear (); }

    // copy assignment operator
    constexpr self_type& operator = (self_type const& other)
    {
        if (this != &other)
        {
            self_type temp (other);
            swap (temp);
        }

        return *this;
    }

    // move assignment operator
    constexpr self_type& operator = (self_type&& other) noexcept
    {
        if (this != &other)
        {
            clear ();

            allocator_type::operator = (std::move (other));

            _M_gBuffer = other._M_gBuffer;
            _M_uLength = other._M_uLength;

            other._M_gBuffer.list = { nullptr, nullptr };
            other._M_uLength      = size_type ();
        }

        return *this;
    }

    self_type& operator = (const_pointer pText) noexcept;

    //self_type substr  (size_type begin_pos, size_type end_pos = npos);


    constexpr const_iterator c_str         () const noexcept { return cbegin ()                       ; }
    constexpr const_iterator data          () const noexcept { return cbegin ()                       ; }
    constexpr iterator       data          ()       noexcept { return  begin ()                       ; }
    constexpr size_type      length        () const noexcept { return  _M_uLength                     ; }
    constexpr size_type      size          () const noexcept { return  _M_uLength                     ; }
    constexpr bool           empty         () const noexcept { return !_M_uLength                     ; }
    constexpr allocator_type get_allocator () const noexcept { return *this                           ; }
    constexpr size_type      max_size      () const noexcept { return allocator_type::max_size ()     ; }
    constexpr size_type      size_bytes    () const noexcept { return _M_uLength * sizeof (value_type); }
    constexpr size_type      capacity      () const noexcept { return _M_uLength                      ; }

    /**
     * @brief Copy substring to character array
     * @param dest Destination buffer
     * @param count Number of characters to copy
     * @param pos Starting position in rope
     * @return Number of characters copied
     */
    constexpr size_type copy (pointer dest, size_type count, size_type pos = 0) const
    {
        if (pos >= size ()) return 0;

        count = std::min (count, _M_uLength - pos);
        if (count == 0) return 0;

        size_type copied = 0;

        if (is_tree ())
        {
            // Find starting node in tree
            tree_node* curr = find_tree_node_at_pos(pos);

            if (!curr) return 0;

            size_type node_offset = get_tree_node_offset (curr);
            size_type pos_in_node = pos - node_offset;

            // Copy from current node
            size_type available = curr->data.size () - pos_in_node;
            size_type to_copy   = std::min (count, available);

            std::copy_n (curr->data.begin () + pos_in_node, to_copy, dest);
            copied += to_copy;

            // Continue with remaining nodes if needed
            while (copied < count && (curr = find_next_tree_node (curr)))
            {
                to_copy = std::min (count - copied, curr->data.size ());
                std::copy_n (curr->data.begin (), to_copy, dest + copied);
                copied += to_copy;
            }
        }
        else
        {
            // Find starting node in list
            list_node* curr = find_list_node_at_pos (pos);
            if (!curr) return 0;

            size_type node_offset = get_node_offset (curr);
            size_type pos_in_node = pos - node_offset;

            // Copy from current node
            size_type available = curr->data.size () - pos_in_node;
            size_type to_copy   = std::min (count, available);

            std::copy_n (curr->data.begin () + pos_in_node, to_copy, dest);
            copied += to_copy;

            // Continue with remaining nodes if needed
            curr = curr->next;

            while (copied < count && curr)
            {
                to_copy = std::min (count - copied, curr->data.size ());
                std::copy_n (curr->data.begin (), to_copy, dest + copied);
                copied += to_copy;
                curr = curr->next;
            }
        }

        return copied;
    }

    /**
     * @brief Reserve capacity for future growth
     * For ropes, we optimize node capacities in the current mode
     */
    inline void reserve (size_type new_cap)
    {
        if (new_cap <= size ()) return;

        if (is_tree ())
        {
            // In tree mode, we balance the tree and ensure node capacities
            tree_node* curr = _M_gBuffer.tree.root;

            while (curr)
            {
                curr->data.reserve (std::min (new_cap - size (), string_type::max_size ()));
                curr = find_next_tree_node (curr);
            }
        }
        else
        {
            // In list mode, we optimize node distribution
            // Target a reasonable node size for good cache behavior
            constexpr static const_size target_node_size = 4096;  // Typical page size

            list_node* curr = _M_gBuffer.list.head;

            while (curr)
            {
                // Reserve space in current node if it's smaller than target
                if (curr->data.capacity () < target_node_size)
                {
                    curr->data.reserve (std::min (target_node_size, string_type::max_size ()));
                }
                curr = curr->next;
            }
        }
    }

    /**
     * @brief Add character to end
     */
    inline void push_back (value_type ch)
    {
        if (is_tree ())
        {
            // Find rightmost node
            tree_node* last = _M_gBuffer.tree.root;

            while (last && last->right) last = last->right;

            if (last)
            {
                last->data.push_back (ch);
            }
            else
            {
                // Empty tree case
                _M_gBuffer.tree.root = allocate_tree_node (string_type (1, ch));
            }
        }
        else
        {
            // List mode
            if (!_M_gBuffer.list.tail)
            {
                // Empty list case
                _M_gBuffer.list.head = _M_gBuffer.list.tail = allocate_list_node (string_type (1, ch));
            }
            else
            {
                // Check if last node has room for efficient appending
                constexpr static const_size target_node_size = 4096;  // Typical page size

                if (_M_gBuffer.list.tail->data.size () < target_node_size)
                {
                    _M_gBuffer.list.tail->data.push_back (ch);
                }
                else
                {
                    // Create new node
                    list_node* new_node = allocate_list_node (string_type (1, ch));

                    new_node->prev = _M_gBuffer.list.tail;
                    _M_gBuffer.list.tail->next = new_node;
                    _M_gBuffer.list.tail = new_node;
                }
            }
        }

        ++_M_uLength;

        // Check if we need to switch modes
        if (!is_tree ()) convert_to_tree ();
    }

    /**
     * @brief Clear all content
     */
    constexpr void clear () noexcept
    {
        // Store mode before clearing
        bool was_tree = is_tree ();

        if (was_tree)
        {
            // Tree mode cleanup
            tree_node* root = _M_gBuffer.tree.root;

            if (root) clear_tree_recursive (root);
        }
        else
        {
            // List mode cleanup
            list_node* curr = _M_gBuffer.list.head;

            while (curr)
            {
                list_node* next = curr->next;
                deallocate_list_node (curr);
                curr = next;
            }
        }

        // Reset to empty state
        _M_gBuffer.list = { nullptr, nullptr };  // Safe to use list mode for empty state
        _M_uLength = size_type ();
    }

    /**
     * @brief Core insert operation
     * Handles mode switching based on size
     */
    constexpr self_type& insert (size_type pos, const string_type& str)
    {
        if (str.empty ()) return *this;
        if (pos >= size ()) throw std::out_of_range ("Insert position out of range");

        // Check if operation will cause mode switch
        size_type new_size = size () + str.size ();
        bool  will_be_tree = (new_size * sizeof(value_type)) > list_threshold;

        if (will_be_tree && !is_tree ()) convert_to_tree ();

        if (is_tree ()) tree_insert (pos, str);
        else list_insert (pos, str);

        _M_uLength = new_size;
        return *this;
    }

    /**
     * @brief Core erase operation
     * Handles mode switching if size drops below threshold
     */
    constexpr self_type& erase (size_type pos = size_type (), size_type len = npos)
    {
        if (pos >= size ()) throw std::out_of_range ("Erase position out of range");

        len = std::min (len, size () - pos);

        if (len == 0) return *this;

        size_type new_size = size () - len;
        bool will_be_list  = (new_size * sizeof(value_type)) <= list_threshold;

        if (is_tree ())
        {
            if (will_be_list)
            {
                // Convert to list first, then erase
                convert_to_list ();
                list_erase (pos, len);
            }
            else
            {
                tree_erase (pos, len);
            }
        }
        else
        {
            list_erase (pos, len);
        }

        _M_uLength = new_size;
        return *this;
    }

    /**
     * @brief Append operation with mode switching
     */
    constexpr self_type& append (string_type const& str)
    {
        return insert (size (), str);
    }

    /**
     * @brief Get substring
     * Creates new rope in appropriate mode based on substring size
     */
    constexpr self_type substr (size_type pos = size_type (), size_type len = npos) const
    {
        if (pos >= size ()) throw std::out_of_range ("Substring position out of range");

        len = std::min (len, size () - pos);
        self_type result;  // Will automatically be in correct mode based on size

        if (is_tree()) collect_substring_tree (_M_gBuffer.tree.root, pos, len, result);
        else collect_substring_list (pos, len, result);

        return result;
    }

    /**
     * @brief Find substring
     * Optimized for both modes
     */
    constexpr size_type find (string_type const& str, size_type pos = 0, size_type /*count*/ = 0) const
    {
        if (pos >= size () || str.empty ()) return npos;
        if (is_tree ()) return find_in_tree (str, pos);
        else return find_in_list (str, pos);
    }

    constexpr size_type find (const_pointer str, size_type pos = 0, size_type count = 0) const noexcept
    {
        return find (string_view (str), pos, count);
    }

    constexpr size_type find (value_type ch, size_type pos = 0) const
    {
        if (pos >= size ()) return npos;
        for (auto i = pos; i < size (); ++i) if ((*this)[i] == ch) return i;
        return npos;
    }

    template <str_view_like_t U>
    constexpr size_type find (U const& sv, size_type pos = 0, size_type count = 0) const
    {
        if (pos >= size () || sv.empty ()) return npos;
        return find (string_type (sv), pos, count);
    }

    constexpr size_type rfind (string_type const& str, size_type pos = npos, size_type count = 0) const
    {
        if ((pos != npos && pos >= size ()) || str.empty () || (count != 0 && str.size () < count))
            return npos;

        auto i = std::min (pos, size () - 1);

        for (auto it = (rend () -= 1) -= i, end = count ? rbegin () + count : rend ();
             it != end;
             ++it, --i)
        {
            if (std::equal (str.rbegin (), count ? str.rbegin () + count : str.rend (), it)) return i;
        }

        return npos;
    }

    constexpr size_type rfind (value_type ch, size_type pos = npos)
    {
        if (pos >= size ()) return npos;
        return rfind (&ch, pos, 1);
    }

    template <str_view_like_t U>
    size_type rfind (U const& sv, size_type pos = npos, size_type count = 0) const
    {
        if ((pos != npos && pos >= size ()) || sv.empty ()) return npos;
        return rfind (string_type (sv), pos, count);
    }

    constexpr iterator begin () noexcept
    {  return iterator (*this, size_type ()); }

    constexpr const_iterator begin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr iterator end () noexcept
    {  return iterator (*this, size ()); }

    constexpr const_iterator end () const noexcept
    {  return const_iterator (*this, size ()); }

    constexpr const_iterator cend () noexcept
    {  return const_iterator (*this, size ()); }

    constexpr const_iterator cend () const noexcept
    {  return const_iterator (*this, size ()); }

    constexpr reverse_iterator rbegin () noexcept
    {  return reverse_iterator (*this, size () - 1); }

    constexpr const_reverse_iterator rbegin () const noexcept
    {  return const_reverse_iterator (*this, size () - 1); }

    constexpr const_reverse_iterator crbegin () noexcept
    {  return const_reverse_iterator (*this, size () - 1); }

    constexpr const_reverse_iterator crbegin () const noexcept
    {  return const_reverse_iterator (*this, size () - 1); }

    constexpr reverse_iterator rend () noexcept
    {  return reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator rend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    /**
     * @brief Replace range with new string
     * Handles both list and tree modes efficiently
     */
    constexpr self_type& replace (size_type pos, size_type len, string_type const& str)
    {
        if (pos >= size ()) throw std::out_of_range ("Replace position out of range");

        // Check if operation would exceed LIST_THRESHOLD
        if (!is_tree () && (size () - std::min (len, size () - pos) + str.size () > list_threshold))
        {
            convert_to_tree ();
        }

        // Erase old range
        erase (pos, len);

        // Insert new string
        return insert (pos, str);
    }

    /**
     * @brief Compare with another rope
     * Returns < 0 if this is less, 0 if equal, > 0 if greater
     */
    constexpr int compare (self_type const& other) const noexcept
    {
        auto it1 = begin ();
        auto it2 = other.begin ();

        while (it1 != end () && it2 != other.end ())
        {
            if (*it1 < *it2) return -1;
            if (*it2 < *it1) return  1;
            ++it1;
            ++it2;
        }

        if (it1 == end () && it2 == other.end ()) return 0;
        return (it1 == end ()) ? -1 : 1;
    }

    /**
     * @brief Compare with string_type
     */
    constexpr int compare (string_type const& str) const noexcept
    {
        auto it1 = begin ();
        auto it2 = str.begin ();

        while (it1 != end () && it2 != str.end ())
            {
            if (*it1 < *it2) return -1;
            if (*it2 < *it1) return  1;
            ++it1;
            ++it2;
        }

        if (it1 == end () && it2 == str.end ()) return 0;
        return (it1 == end ()) ? -1 : 1;
    }

    /**
     * @brief Compare substring with another string
     */
    constexpr int compare (size_type pos1, size_type n1, string_type const& str,
                           size_type pos2, size_type n2) const
    {
        if (pos1 >= size ()) throw std::out_of_range("Position out of range");

        self_type temp = substr (pos1, n1);
        return temp.compare (str.substr (pos2, n2));
    }

    /**
     * @brief Find first of any character in set
     */
    constexpr size_type find_first_of (string_type const& str, size_type pos = size_type ()) const noexcept
    {
        if (pos >= size () || str.empty ()) return npos;

        for (auto it = begin () + pos; it != end (); ++it)
        {
            if (str.find (*it) != npos) return std::distance (begin (), it);
        }

        return npos;
    }

    /**
     * @brief Find last of any character in set
     */
    constexpr size_type find_last_of (string_type const& str, size_type pos = npos) const noexcept
    {
        if (empty () || str.empty ()) return npos;

        if (pos >= size ()) pos = size () - 1;

        for (auto it = begin () + pos; it != begin (); --it)
        {
            if (str.find(*it) != npos) return std::distance (begin (), it);
        }

        // Check first character
        if (str.find (*begin ()) != npos) return 0;
        return npos;
    }

    /**
     * @brief Find first character not in set
     */
    constexpr size_type find_first_not_of (string_type const& str, size_type pos = size_type ()) const noexcept
    {
        if (pos >= size ()) return npos;

        for (auto it = begin () + pos; it != end (); ++it)
        {
            if (str.find (*it) == npos) return std::distance (begin (), it);
        }

        return npos;
    }

    /**
     * @brief Find last character not in set
     */
    constexpr size_type find_last_not_of (string_type const& str, size_type pos = npos) const noexcept
    {
        if (empty ()) return npos;

        if (pos >= size ()) pos = size () - 1;

        for (auto it = begin () + pos; it != begin (); --it)
        {
            if (str.find (*it) == npos) return std::distance (begin (), it);
        }

        // Check first character
        if (str.find (*begin ()) == npos) return 0;
        return npos;
    }

    /**
     * @brief Starts with prefix
     */
    constexpr bool starts_with (string_type const& str) const noexcept
    {
        if (str.size () > size ()) return false;
        return compare (0, str.size (), str) == 0;
    }

    constexpr bool starts_with (string_view sv) const noexcept
    {
        return sv.size () <= size () && string_view (c_str (), sv.size ()) == sv;
    }

    constexpr bool starts_with (value_type ch) const noexcept
    {
        return !empty () && front () == ch;
    }

    constexpr bool starts_with (const_pointer str) const
    {
        return starts_with (string_view (str));
    }

    /**
     * @brief Ends with suffix
     */
    constexpr bool ends_with (string_type const& str) const noexcept
    {
        if (str.size () > size ()) return false;
        return compare (size () - str.size (), str.size (), str) == 0;
    }


    constexpr bool ends_with (string_view sv) const noexcept
    {
        return sv.size () <= size () &&
               string_view (c_str () + size () - sv.size (), sv.size ()) == sv;
    }

    constexpr bool ends_with (value_type ch) const noexcept
    {
        return !empty () && back () == ch;
    }

    constexpr bool ends_with (const_pointer str) const
    {
        return ends_with (string_view (str));
    }

    /**
     * @brief Contains substring
     */
    constexpr bool contains (string_type const& str) const noexcept
    {
        return find (str) != npos;
    }

    constexpr bool contains (string_view const& sv) const noexcept
    {
        return find (sv) != npos;
    }

    constexpr bool contains (value_type ch) const noexcept
    {
        return find (ch) != npos;
    }

    constexpr bool contains (const_pointer str) const
    {
        return contains (string_view (str));
    }

    /**
     * @brief Remove last character
     */
    constexpr void pop_back ()
    {
        if (empty ()) return;

        if (is_tree ())
        {
            tree_node* last = find_rightmost_node (_M_gBuffer.tree.root);
            if (!last) return;

            last->data.pop_back ();
            if (last->data.empty ()) remove_tree_node (last);
        }
        else
        {
            _M_gBuffer.list.tail->data.pop_back ();

            if (_M_gBuffer.list.tail->data.empty ())
            {
                list_node* temp = _M_gBuffer.list.tail;
                _M_gBuffer.list.tail = temp->prev;

                if (_M_gBuffer.list.tail) _M_gBuffer.list.tail->next = nullptr;
                else _M_gBuffer.list.head = nullptr;
                deallocate_list_node (temp);
            }
        }

        --_M_uLength;
    }

    /**
     * @brief Access character with bounds checking
     */
    constexpr reference at (size_type pos)
    {
        if (pos >= _M_uLength) throw std::out_of_range ("Index out of range");
        return operator[](pos);
    }

    constexpr const_reference at (size_type pos) const
    {
        if (pos >= _M_uLength) throw std::out_of_range ("Index out of range");
        return operator[](pos);
    }

    /**
     * @brief Access character without bounds checking
     */
    constexpr reference operator [] (size_type pos)
    {
        if (is_tree ())
        {
            tree_node* node   = find_tree_node_at_pos (pos);
            size_type  offset = get_tree_node_offset (node);

            return node->data[pos - offset];
        }
        else
        {
            list_node* node   = find_list_node_at_pos (pos);
            size_type  offset = get_node_offset (node);

            return node->data[pos - offset];
        }
    }

    constexpr const_reference operator [] (size_type pos) const
    {
        if (is_tree ())
        {
            tree_node* node   = find_tree_node_at_pos(pos);
            size_type  offset = get_tree_node_offset(node);

            return node->data[pos - offset];
        }
        else
        {
            list_node* node   = find_list_node_at_pos (pos);
            size_type  offset = get_node_offset (node);
            return node->data[pos - offset];
        }
    }

    /**
     * @brief Access last character
     */
    constexpr reference back ()
    {
        if (empty ()) throw std::out_of_range ("Empty rope");
        return operator[](_M_uLength - 1);
    }

    constexpr const_reference back() const
    {
        if (empty ()) throw std::out_of_range ("Empty rope");
        return operator[](_M_uLength - 1);
    }

    /**
     * @brief Access first character
     */
    constexpr reference front ()
    {
        if (empty ()) throw std::out_of_range("Empty rope");
        return operator[](0);
    }

    constexpr const_reference front () const
    {
        if (empty ()) throw std::out_of_range ("Empty rope");
        return operator[](0);
    }

    // String concatenation operators
    constexpr self_type& operator += (self_type const& other) { return append (other); }
    constexpr self_type& operator += (string_type const& str) { return append (str); }
    constexpr self_type& operator += (value_type ch) { push_back (ch); return *this; }

    // String comparison operators
    friend constexpr bool operator == (self_type const& lhs, self_type const& rhs) noexcept
    {
        return lhs.compare (rhs) == 0;
    }

    friend constexpr bool operator != (self_type const& lhs, self_type const& rhs) noexcept
    {
        return lhs.compare (rhs) != 0;
    }

    friend constexpr bool operator < (self_type const& lhs, self_type const& rhs) noexcept
    {
        return lhs.compare (rhs) < 0;
    }

    friend constexpr bool operator > (self_type const& lhs, self_type const& rhs) noexcept
    {
        return lhs.compare (rhs) > 0;
    }

    friend constexpr bool operator<= (self_type const& lhs, self_type const& rhs) noexcept
    {
        return lhs.compare (rhs) <= 0;
    }

    friend constexpr bool operator >= (self_type const& lhs, self_type const& rhs) noexcept
    {
        return lhs.compare (rhs) >= 0;
    }

    template <char_t U, allocator_t _A>
    friend frope<U, _A> operator + (frope<U, _A> const& obj1, frope<U, _A> const& obj2) noexcept;

    template <char_t U, allocator_t _A>
    friend frope<U, _A> operator + (frope<U, _A> const& obj1, U const* obj2) noexcept;

    template <char_t U, allocator_t _A>
    friend frope<U, _A>& operator += (frope<U, _A>& obj1, frope<U, _A> const& obj2) noexcept;

    template <char_t U, allocator_t _A>
    friend frope<U, _A>& operator += (frope<U, _A>& obj1, U const* obj2) noexcept;

    template <char_t U, allocator_t _A>
    friend constexpr void swap (frope<U, _A>& lhs, frope<U, _A>& rhs) noexcept;

private:
    /**
     * @brief Recursive helper for clearing tree
     */
    constexpr void clear_tree_recursive (tree_node* node) noexcept
    {
        if (!node) return;

        // Clear children first
        clear_tree_recursive (node->left );
        clear_tree_recursive (node->right);

        // Clear current node
        deallocate_tree_node (node);
    }

    /**
     * @brief Find rightmost node in tree
     */
    tree_node* find_rightmost_node (tree_node* node) const
    {
        if (!node) return nullptr;

        tree_node* curr = node;

        while (curr->right) curr = curr->right;
        return curr;
    }

    /**
     * @brief Allocate list node using allocator
     */
    list_node* allocate_list_node (string_type const& str)
    {
        list_node_allocator alloc (*this);
        list_node* node = alloc.allocate (1);

        alloc.construct (node, str);
        return node;
    }

    /**
     * @brief Deallocate list node using allocator
     */
    void deallocate_list_node (list_node* node)
    {
        if (!node) return;

        list_node_allocator alloc (*this);

        alloc.destroy (node);
        alloc.deallocate (node, 1);
    }

    /**
     * @brief Allocate tree node using allocator
     */
    tree_node* allocate_tree_node (string_type const& str)
    {
        tree_node_allocator alloc (*this);
        tree_node* node = alloc.allocate (1);

        alloc.construct (node, str);
        return node;
    }

    /**
     * @brief Deallocate tree node using allocator
     */
    void deallocate_tree_node (tree_node* node)
    {
        if (!node) return;

        tree_node_allocator alloc (*this);
        alloc.destroy (node);
        alloc.deallocate (node, 1);
    }

    void copy_from_tree (tree_node* src)
    {
        if (!src) return;

        tree_node* new_node = allocate_tree_node (src->data);

        if (_M_uLength == 0) _M_gBuffer.tree.root = new_node;

        if (src->left)
        {
            new_node->left = allocate_tree_node (src->left->data);
            new_node->left->parent = new_node;
            copy_from_tree (src->left);
        }

        if (src->right)
        {
            new_node->right = allocate_tree_node (src->right->data);
            new_node->right->parent = new_node;
            copy_from_tree (src->right);
        }

        _M_uLength += src->data.size ();
    }

    // List operations
    void copy_from_list (list_node* src)
    {
        list_node* prev = nullptr;

        while (src)
        {
            list_node* new_node = allocate_list_node (src->data);

            if (!_M_gBuffer.list.head)
            {
                _M_gBuffer.list.head = new_node;
            }
            else
            {
                prev->next = new_node;
                new_node->prev = prev;
            }

            _M_gBuffer.list.tail = new_node;
            prev = new_node;
            _M_uLength += src->data.size ();
            src = src->next;
        }
    }

    /**
     * @brief Convert from tree to list mode
     * Optimized for sequential access
     */
    void convert_to_list ()
    {
        if (!is_tree ()) return;

        // Store root before we start modifying the buffer
        tree_node* root = _M_gBuffer.tree.root;

        // Initialize list pointers
        _M_gBuffer.list = { nullptr, nullptr };

        // Perform inorder traversal to build list
        tree_node* curr = root;
        dyn_array<tree_node*> stack;

        while (curr || !stack.empty ())
        {
            while (curr)
            {
                stack.push (curr);
                curr = curr->left;
            }

            curr = stack.top ();
            stack.pop ();

            list_node* list_node = allocate_list_node (curr->data);

            if (!_M_gBuffer.list.head)
            {
                _M_gBuffer.list.head = _M_gBuffer.list.tail = list_node;
            }
            else
            {
                list_node->prev = _M_gBuffer.list.tail;
                _M_gBuffer.list.tail->next = list_node;
                _M_gBuffer.list.tail = list_node;
            }

            tree_node* next = curr->right;
            deallocate_tree_node (curr);
            curr = next;
        }
    }

    /**
     * @brief Convert from list to tree mode
     * Optimized for minimal allocations
     */
    void convert_to_tree ()
    {
        if (is_tree ()) return;

        // Store head before we start modifying the buffer
        list_node* head = _M_gBuffer.list.head;

        // Initialize tree pointer
        _M_gBuffer.tree = { nullptr };

        // Create balanced tree from list nodes
        dyn_array<tree_node*> nodes;
        nodes.reserve (_M_uLength / (list_threshold / 16));  // Estimate node count

        list_node* curr = head;

        while (curr)
        {
            tree_node* tree_node = allocate_tree_node (curr->data);
            nodes.push_back (tree_node);

            list_node* next = curr->next;
            deallocate_list_node (curr);
            curr = next;
        }

        // Build balanced tree from sorted nodes
        _M_gBuffer.tree.root = build_balanced_tree (nodes.data (), nodes.size ());
    }

    /**
     * @brief Insert into list mode
     */
    void list_insert (size_type pos, string_type const& str)
    {
        list_node* curr        = find_list_node_at_pos (pos);
        size_type  node_offset = get_node_offset (curr);

        // Split current node if inserting in middle
        if (pos > node_offset)
        {
            size_type   split_pos = pos - node_offset;
            string_type remaining = curr->data.substr (split_pos);

            curr->data.erase (split_pos);

            list_node* new_node       = allocate_list_node (str);
            list_node* remaining_node = allocate_list_node (remaining);

            // Link nodes
            new_node->next = remaining_node;
            new_node->prev = curr;
            remaining_node->next = curr->next;
            remaining_node->prev = new_node;

            if (curr->next) curr->next->prev = remaining_node;
            else _M_gBuffer.list.tail = remaining_node;

            curr->next = new_node;
        }
        else
        {
            // Insert before current node
            list_node* new_node = allocate_list_node (str);
            new_node->next = curr;
            new_node->prev = curr->prev;

            if (curr->prev) curr->prev->next = new_node;
            else _M_gBuffer.list.head = new_node;

            curr->prev = new_node;
        }
    }

    /**
     * @brief Insert into tree mode
     */
    void tree_insert (size_type pos, string_type const& str)
    {
        tree_node* curr        = find_tree_node_at_pos (pos);
        size_type  node_offset = get_tree_node_offset (curr);

        // Split current node if inserting in middle
        if (pos > node_offset)
        {
            size_type   split_pos = pos - node_offset;
            string_type remaining = curr->data.substr (split_pos);

            curr->data.erase (split_pos);

            tree_node* new_node       = allocate_tree_node (str);
            tree_node* remaining_node = allocate_tree_node (remaining);

            // Link nodes
            if (curr->right)
            {
                remaining_node->right = curr->right;
                curr->right->parent = remaining_node;
            }

            remaining_node->left = new_node;
            new_node->parent = remaining_node;
            curr->right = remaining_node;
            remaining_node->parent = curr;
        }
        else
        {
            tree_node* new_node = allocate_tree_node (str);

            if (curr->left)
            {
                new_node->left = curr->left;
                curr->left->parent = new_node;
            }

            curr->left = new_node;
            new_node->parent = curr;
        }
    }

    /**
     * @brief Erase from list mode
     */
    void list_erase (size_type pos, size_type len)
    {
        list_node* start        = find_list_node_at_pos (pos);
        size_type  start_offset = get_node_offset (start);
        size_type  remaining    = len;

        // Handle partial erasure in first node
        if (pos > start_offset)
        {
            size_type node_pos  = pos - start_offset;
            size_type erase_len = std::min (remaining, start->data.size () - node_pos);
            start->data.erase (node_pos, erase_len);
            remaining -= erase_len;
            if (remaining == 0) return;
        }

        // Remove whole nodes
        list_node* curr = start->next;

        while (curr && remaining >= curr->data.size())
        {
            remaining -= curr->data.size ();
            list_node* next = curr->next;

            if (curr->prev) curr->prev->next = curr->next;
            if (curr->next) curr->next->prev = curr->prev;
            if (curr == _M_gBuffer.list.tail) _M_gBuffer.list.tail = curr->prev;

            deallocate_list_node (curr);
            curr = next;
        }

        // Handle partial erasure in last node
        if (remaining > 0 && curr) curr->data.erase (0, remaining);
    }

    /**
     * @brief Erase from tree mode
     */
    void tree_erase (size_type pos, size_type len)
    {
        tree_node* start        = find_tree_node_at_pos (pos);
        size_type  start_offset = get_tree_node_offset (start);
        size_type  remaining    = len;

        // Handle partial erasure in first node
        if (pos > start_offset)
        {
            size_type node_pos  = pos - start_offset;
            size_type erase_len = std::min (remaining, start->data.size () - node_pos);

            start->data.erase (node_pos, erase_len);
            remaining -= erase_len;
            if (remaining == 0) return;
        }

        // Remove nodes and rebalance
        tree_node* curr = find_next_tree_node (start);

        while (curr && remaining > 0)
        {
            if (remaining >= curr->data.size ())
            {
                remaining -= curr->data.size ();
                tree_node* next = find_next_tree_node (curr);
                remove_tree_node (curr);
                curr = next;
            }
            else
            {
                curr->data.erase (0, remaining);
                remaining = 0;
            }
        }
    }

    /**
     * @brief Collect substring in list mode
     */
    void collect_substring_list (size_type pos, size_type len, frope& result) const
    {
        list_node* start        = find_list_node_at_pos (pos);
        size_type  start_offset = get_node_offset (start);
        size_type  remaining    = len;

        // Handle first node
        if (pos > start_offset)
        {
            size_type node_pos = pos - start_offset;
            size_type copy_len = std::min (remaining, start->data.size () - node_pos);
            result.append (start->data.substr (node_pos, copy_len));
            remaining -= copy_len;
        }

        // Copy whole nodes
        list_node* curr = start->next;

        while (curr && remaining > 0)
        {
            if (remaining >= curr->data.size ())
            {
                result.append (curr->data);
                remaining -= curr->data.size ();
            }
            else
            {
                result.append (curr->data.substr (0, remaining));
                remaining = 0;
            }

            curr = curr->next;
        }
    }

    /**
     * @brief Collect substring in tree mode
     */
    void collect_substring_tree (tree_node* node, size_type pos, size_type len, frope& result) const
    {
        if (!node || len == 0) return;

        size_type left_size = get_subtree_size (node->left);
        size_type curr_pos  = get_tree_node_offset (node);

        // Check left subtree
        if (pos < curr_pos + left_size)
        {
            collect_substring_tree (node->left, pos, len, result);
            return;
        }

        // Current node
        if (pos < curr_pos + node->data.size ())
        {
            size_type node_pos = pos - curr_pos;
            size_type copy_len = std::min (len, node->data.size () - node_pos);
            result.append (node->data.substr (node_pos, copy_len));

            if (copy_len < len)
            {
                // Continue to right subtree if needed
                collect_substring_tree (node->right, pos + copy_len, len - copy_len, result);
            }
        }
        else
        {
            // Try right subtree
            collect_substring_tree (node->right, pos, len, result);
        }
    }

    /**
     * @brief Find pattern in list mode
     */
    size_type find_in_list (string_type const& str, size_type pos) const
    {
        if (str.empty ()) return pos;

        list_node* curr     = find_list_node_at_pos (pos);
        size_type  curr_pos = get_node_offset (curr);

        while (curr)
        {
            if (pos <= curr_pos + curr->data.size ())
            {
                size_type found = curr->data.find (str, pos - curr_pos);

                if (found != npos) return curr_pos + found;
            }

            curr_pos += curr->data.size ();
            curr = curr->next;
        }

        return npos;
    }

    /**
     * @brief Find pattern in tree mode
     */
    size_type find_in_tree (string_type const& str, size_type pos) const
    {
        if (str.empty ()) return pos;

        tree_node* curr = find_tree_node_at_pos (pos);
        while (curr)
        {
            size_type node_pos = get_tree_node_offset (curr);
            size_type found    = curr->data.find (str, pos - node_pos);

            if (found != npos) return node_pos + found;

            curr = find_next_tree_node (curr);
            if (curr) pos = get_tree_node_offset (curr);
        }

        return npos;
    }

    /**
     * @brief Find list node containing given position
     */
    list_node* find_list_node_at_pos (size_type pos) const
    {
        if (pos >= size ()) return nullptr;

        list_node* curr = _M_gBuffer.list.head;
        size_type  curr_pos = 0;

        while (curr)
        {
            if (curr_pos <= pos && pos < curr_pos + curr->data.size ()) return curr;

            curr_pos += curr->data.size ();
            curr = curr->next;
        }

        return nullptr;
    }

    /**
     * @brief Get offset of list node from start of rope
     */
    size_type get_node_offset (list_node const* node) const
    {
        if (!node) return 0;

        size_type offset = 0;
        const list_node* curr = _M_gBuffer.list.head;

        while (curr && curr != node)
        {
            offset += curr->data.size ();
            curr = curr->next;
        }

        return offset;
    }

    /**
     * @brief Find tree node containing given position
     */
    tree_node* find_tree_node_at_pos (size_type pos) const
    {
        if (pos >= size ()) return nullptr;

        tree_node* curr     = _M_gBuffer.tree.root;
        size_type  curr_pos = 0;

        if (!curr) throw std::runtime_error ("Invalid tree state");

        while (curr)
        {
            size_type left_size = get_subtree_size (curr->left);

            if (pos < curr_pos + left_size)
            {
                curr = curr->left;
            }
            else if (pos < curr_pos + left_size + curr->data.size())
            {
                return curr;
            }
            else
            {
                curr_pos += left_size + curr->data.size();
                curr = curr->right;
            }
        }

        return nullptr;
    }

    /**
     * @brief Get offset of tree node from start of rope
     */
    size_type get_tree_node_offset (tree_node const* node) const
    {
        if (!node) return 0;

        size_type  offset = get_subtree_size (node->left);
        tree_node* parent = node->parent;
        tree_node* child  = const_cast<tree_node*> (node);

        while (parent)
        {
            if (parent->right == child)
            {
                offset += parent->data.size () + get_subtree_size (parent->left);
            }

            child = parent;
            parent = parent->parent;
        }
        return offset;
    }

    /**
     * @brief Find next node in tree inorder traversal
     */
    tree_node* find_next_tree_node (tree_node* node) const
    {
        if (!node) return nullptr;

        // If has right child, find leftmost in right subtree
        if (node->right)
        {
            tree_node* curr = node->right;

            while (curr->left) curr = curr->left;
            return curr;
        }

        // Otherwise, go up until we find a parent we haven't visited
        tree_node* parent = node->parent;
        tree_node* curr   = node;

        while (parent && parent->right == curr)
        {
            curr = parent;
            parent = parent->parent;
        }
        return parent;
    }

    /**
     * @brief Remove node from tree and maintain balance
     */
    void remove_tree_node (tree_node* node)
    {
        if (!node) return;

        // Case 1: Leaf node
        if (!node->left && !node->right)
        {
            if (node->parent)
            {
                if (node->parent->left == node) node->parent->left = nullptr;
                else node->parent->right = nullptr;
            }
            else
            {
                _M_gBuffer.tree.root = nullptr;
            }
            deallocate_tree_node (node);
            return;
        }

        // Case 2: Node with one child
        if (!node->left || !node->right)
        {
            tree_node* child = node->left ? node->left : node->right;
            child->parent = node->parent;

            if (node->parent)
            {
                if (node->parent->left == node) node->parent->left = child;
                else node->parent->right = child;
            }
            else
            {
                _M_gBuffer.tree.root = child;
            }
            deallocate_tree_node (node);
            return;
        }

        // Case 3: Node with two children
        tree_node* successor = node->right;
        while (successor->left) successor = successor->left;

        node->data = successor->data;
        remove_tree_node (successor);  // Recursively remove successor (will be case 1 or 2)
    }

    /**
     * @brief Build balanced tree from sorted nodes
     */
    tree_node* build_balanced_tree (tree_node** nodes, size_type count)
    {
        if (count == 0) return nullptr;

        size_type  mid  = count / 2;
        tree_node* root = nodes[mid];

        // Build left subtree
        root->left = build_balanced_tree (nodes, mid);
        if (root->left) root->left->parent = root;

        // Build right subtree
        root->right = build_balanced_tree (nodes + mid + 1, count - mid - 1);
        if (root->right) root->right->parent = root;

        return root;
    }

    constexpr size_type get_subtree_size (tree_node* node) const
    {
        if (!node) return 0;
        return node->data.size () + get_subtree_size (node->left) + get_subtree_size (node->right);
    }

    constexpr bool is_tree () const noexcept
    { return (size () * sizeof (value_type)) > list_threshold; }

private:
    buffer    _M_gBuffer { };         // Current storage buffer
    size_type _M_uLength { };         // Total text size
};

// ====================================================

template <char_t T, allocator_t A>
constexpr bool operator <=> (frope<T, A> const& lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) <=> std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_t A>
constexpr bool operator <=> (frope<T, A> const& lhs, T const* rhs) noexcept
{
    return std::basic_string_view<T> (lhs) <=> std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_t A>
constexpr bool operator <=> (T const* lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) <=> std::basic_string_view<T> (rhs);
}

// ====================================================

template <char_t T, allocator_t A>
constexpr bool operator == (frope<T, A> const& lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) == std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_t A>
constexpr bool operator == (frope<T, A> const& lhs, T const* rhs) noexcept
{
    return std::basic_string_view<T> (lhs) == std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_t A>
constexpr bool operator == (T const* lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) == std::basic_string_view<T> (rhs);
}

// ====================================================

template <char_t T, allocator_t A>
constexpr bool operator != (frope<T, A> const& lhObj, frope<T, A> const& rhObj) noexcept
{ return !(lhObj == rhObj); }

template <char_t T, allocator_t A>
constexpr bool operator != (frope<T, A> const& lhObj, T const* pText2) noexcept
{ return !(lhObj == pText2); }

template <char_t T, allocator_t A>
constexpr bool operator != (T const* pText1, frope<T, A> const& rhObj) noexcept
{ return !(pText1 == rhObj); }

// ====================================================

// template <char_t T, allocator_t A>
// inline frope<T, A>& operator += (frope<T, A>& lhObj, frope<T, A> const& rhObj) noexcept
// { return add_to_string (lhObj, rhObj._M_gBuffer.heap.data, rhObj._M_uLength); }

// template <char_t T, allocator_t A>
// inline frope<T, A>& operator += (frope<T>& gObj, T const* pText) noexcept
// { return add_to_string (gObj, pText, str_size (pText)); }

// ====================================================

// template <char_t T, allocator_t A>
// inline frope<T, A> operator + (frope<T, A> const& lhObj, frope<T, A> const& rhObj) noexcept
// {
//     frope<T, A> gStr (lhObj);
//     return std::move (add_to_string (gStr, rhObj._M_gBuffer.heap.data, rhObj._M_uLength));
// }

// ====================================================

// template <char_t T, allocator_t A>
// inline frope<T, A> operator + (frope<T, A> const& gObj, T const* pText) noexcept
// {
//     frope<T, A> gStr (gObj);
//     return std::move (add_to_string (gStr, pText, str_size (pText)));
// }

// ====================================================

template <char_t T, allocator_t A>
constexpr void swap (frope<T, A>& lhs, frope<T, A>& rhs) noexcept
{
    std::swap (lhs._M_gBuffer, rhs._M_gBuffer);
    std::swap (lhs._M_uLength, rhs._M_uLength);
}

// ====================================================

typedef frope<char8 , memory::allocator<char8>>  fu8rope ;
typedef frope<char16, memory::allocator<char16>> fu16rope;
typedef frope<char32, memory::allocator<char32>> fu32rope;
typedef frope<wchar , memory::allocator<wchar>>  fwrope  ;

// ====================================================

template <allocator_t A>
struct is_string<frope<char, A>> : std::true_type
{ };

template <allocator_t A>
struct is_string<frope<char8, A>> : std::true_type
{ };

template <allocator_t A>
struct is_string<frope<char16, A>> : std::true_type
{ };

template <allocator_t A>
struct is_string<frope<char32, A>> : std::true_type
{ };

template <allocator_t A>
struct is_string<frope<wchar, A>> : std::true_type
{ };

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_TEXT_ROPE_H_