#ifndef CPPUAL_GENERIC_REPLY_ITERATOR_HPP
#define CPPUAL_GENERIC_REPLY_ITERATOR_HPP

#include <cppual/ui/platforms/xcb/generic/factory.hpp>
#include <cppual/ui/platforms/xcb/generic/signature.hpp>
#include <cppual/ui/platforms/xcb/generic/iterator_traits.hpp>
#include <cppual/containers.h>

#include <cstdlib> // size_t
#include <memory>
#include <stack>
#include <xcb/xcb.h> // ::xcb_str_*

#define NEXT_TEMPLATE \
  void (&Next)(XcbIterator *)

#define NEXT_SIGNATURE \
  cppual::generic::signature<void (XcbIterator *), Next>

#define SIZEOF_TEMPLATE \
  int (&SizeOf)(const void *)

#define SIZEOF_SIGNATURE \
  cppual::generic::signature<int (const void *), SizeOf>

#define GETITERATOR_TEMPLATE \
  XcbIterator (&GetIterator)(const Reply *)

#define GETITERATOR_SIGNATURE \
  cppual::generic::signature<XcbIterator (const Reply *), GetIterator>

#define ACCESSOR_TEMPLATE \
  Data * (&Accessor)(const Reply *)

#define ACCESSOR_SIGNATURE \
  cppual::generic::signature<Data * (const Reply *), Accessor>

#define LENGTH_TEMPLATE \
  int (&Length)(const Reply *)

#define LENGTH_SIGNATURE \
  cppual::generic::signature<int (const Reply *), Length>

namespace cppual {

namespace generic {

template<typename Data>
class get
{
public:
    Data
    operator()(Data * const data)
    {
        return *data;
    }
};

template<>
class get<xcb_str_t>
{
public:
    string
    operator()(xcb_str_t * const data)
    {
        return string(xcb_str_name(data), static_cast<std::size_t>(xcb_str_name_length(data)));
    }
};

namespace detail
{

template<typename F>
struct function_traits;

template<typename Signature, Signature& S>
struct function_traits<signature<Signature, S>> : function_traits<Signature> {};

template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)> {};

template<typename R, typename... Args>
struct function_traits<R(Args...)>
{
    using result_type = R;
    const static std::size_t arity = sizeof...(Args);

    template <std::size_t I>
    struct argument
    {
        static_assert(I < arity, "invalid argument index");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

}

/// iterator reimplementation
template<class _Category,
         class _Tp,
         class _Distance  = ptrdiff_t,
         class _Pointer   = _Tp*,
         class _Reference = _Tp&>
struct basic_iterator
{
    typedef _Tp        value_type;
    typedef _Distance  difference_type;
    typedef _Pointer   pointer;
    typedef _Reference reference;
    typedef _Category  iterator_category;
};

// iterator for variable size data fields

template<typename ... Types>
class iterator;

template<typename Connection,
         typename Object,
         typename NextTemplate,
         NextTemplate& Next,
         typename SizeOfTemplate,
         SizeOfTemplate& SizeOf,
         typename GetIteratorTemplate,
         GetIteratorTemplate& GetIterator>
class iterator<Connection,
        Object,
        cppual::generic::signature<NextTemplate, Next>,
        cppual::generic::signature<SizeOfTemplate, SizeOf>,
        cppual::generic::signature<GetIteratorTemplate, GetIterator>>
        : public basic_iterator<typename std::input_iterator_tag,
        Object,
        typename std::size_t,
        Object *,
        const Object &>
{
protected:
    using self = iterator<Connection,
    Object,
    cppual::generic::signature<NextTemplate, Next>,
    cppual::generic::signature<SizeOfTemplate, SizeOf>,
    cppual::generic::signature<GetIteratorTemplate, GetIterator>>;

    using get_iterator_traits = detail::function_traits<GetIteratorTemplate>;
    using const_reply_ptr = typename get_iterator_traits::template argument<0>::type;
    using Reply = typename std::remove_pointer<typename std::remove_const<const_reply_ptr>::type>::type;
    using XcbIterator = typename get_iterator_traits::result_type;

    Connection _M_c;
    std::shared_ptr<Reply> _M_reply;
    std::stack<std::size_t> _M_lengths;
    XcbIterator _M_iterator;

public:
    iterator(void) {}

    template<typename C>
    iterator(C && c, const std::shared_ptr<Reply> & reply)
        : _M_c(std::forward<C>(c))
        , _M_reply(reply)
        , _M_iterator(GetIterator(reply.get()))
    {}

    bool
    operator==(const iterator & other)
    {
        return _M_iterator.rem == other._M_iterator.rem;
    }

    bool
    operator!=(const iterator & other)
    {
        return ! (*this == other);
    }

    auto
    operator*(void) -> decltype(get<Object>()(this->_M_iterator.data))
    {
        return get<Object>()(_M_iterator.data);
    }

    // prefix
    self &
    operator++(void)
    {
        _M_lengths.push(SizeOf(_M_iterator.data));
        Next(&_M_iterator);
        return *this;
    }

    // postfix
    self
    operator++(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    // prefix
    self &
    operator--(void)
    {
        typedef typename std::remove_pointer<decltype(_M_iterator.data)>::type data_t;
        if (_M_lengths.empty()) {
            data_t * data = _M_iterator.data;
            data_t * prev = data - _M_lengths.top();
            _M_lengths.pop();
            _M_iterator.index = reinterpret_cast<char*>(_M_iterator.data) - reinterpret_cast<char*>(prev);
            _M_iterator.data = prev;
            ++_M_iterator.rem;
        }
        return *this;
    }

    // postfix
    self
    operator--(int)
    {
        auto copy = *this;
        --(*this);
        return copy;
    }

    template<typename C>
    static
    self
    begin(C && c, const std::shared_ptr<Reply> & reply)
    {
        return self { std::forward<C>(c), reply };
    }

    template<typename C>
    static
    self
    end(C && c, const std::shared_ptr<Reply> & reply)
    {
        auto it = self { std::forward<C>(c), reply };
        it._M_iterator.rem = 0;
        return it;
    }
}; // class iterator

// iterator for fixed size data fields

template<typename Connection,
         typename Object,
         typename AccessorTemplate,
         AccessorTemplate& Accessor,
         typename LengthTemplate,
         LengthTemplate& Length>
class iterator<Connection,
        Object,
        signature<AccessorTemplate, Accessor>,
        signature<LengthTemplate, Length>>
        : public basic_iterator<typename std::input_iterator_tag,
        Object,
        typename std::size_t,
        Object *,
        const Object &>
{
protected:

    using accessor_traits = detail::function_traits<AccessorTemplate>;
    using Data = typename std::remove_pointer<typename accessor_traits::result_type>::type;
    using const_reply_ptr = typename accessor_traits::template argument<0>::type;
    using Reply = typename std::remove_pointer<typename std::remove_const<const_reply_ptr>::type>::type;

    using data_t = typename std::conditional<std::is_void<Data>::value,
    typename cppual::generic::conversion_type<Object>::type, Data>::type;
    using make = cppual::generic::factory::make<Connection, data_t, Object>;

    Connection _M_c;
    std::size_t _M_index = 0;
    std::shared_ptr<Reply> _M_reply;

public:
    typedef iterator<Connection,
    Object,
    signature<AccessorTemplate, Accessor>,
    signature<LengthTemplate, Length>>
    self;

    iterator(void) {}

    template<typename C>
    iterator(C && c,
             const std::shared_ptr<Reply> & reply,
             std::size_t index)
        : _M_c(c)
        , _M_index(index)
        , _M_reply(reply)
    {
        if (std::is_void<Data>::value) {
            _M_index /= sizeof(data_t);
        }
    }

    bool operator==(const iterator & other)
    {
        return _M_index == other._M_index;
    }

    bool operator!=(const iterator & other)
    {
        return ! (*this == other);
    }

    Object operator*(void)
    {
        return make()(_M_c, static_cast<data_t *>(Accessor(_M_reply.get()))[_M_index]);
    }

    // prefix
    self & operator++(void)
    {
        ++_M_index;
        return *this;
    }

    // postfix
    self operator++(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    // prefix
    self & operator--(void)
    {
        --_M_index;
        return *this;
    }

    // postfix
    self operator--(int)
    {
        auto copy = *this;
        --(*this);
        return copy;
    }

    template<typename C>
    static
    self
    begin(C && c, const std::shared_ptr<Reply> & reply)
    {
        return self { std::forward<C>(c), reply, 0 };
    }

    template<typename C>
    static
    self
    end(C && c, const std::shared_ptr<Reply> & reply)
    {
        return self { std::forward<C>(c),
                    reply,
                    static_cast<std::size_t>(Length(reply.get())) };
    }
}; // class iterator

template<typename Connection, typename Reply, typename Iterator>
class list {
private:
    // before public part, to make decltype in begin() & end() work!
    Connection _M_c;
    std::shared_ptr<Reply> _M_reply;

public:
    template<typename C>
    list(C && c, const std::shared_ptr<Reply> & reply)
        : _M_c(std::forward<C>(c)), _M_reply(reply)
    {}

    auto
    begin(void) -> decltype(Iterator::begin(this->_M_c, this->_M_reply))
    {
        return Iterator::begin(_M_c, _M_reply);
    }

    auto
    end(void) -> decltype(Iterator::end(this->_M_c, this->_M_reply))
    {
        return Iterator::end(_M_c, _M_reply);
    }
}; // class list

} // namespace generic

} // namespace cppual

#endif // CPPUAL_GENERIC_REPLY_ITERATOR_HPP
