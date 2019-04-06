#ifndef CPPUAL_GENERIC_ITERATOR_TRAITS_HPP
#define CPPUAL_GENERIC_ITERATOR_TRAITS_HPP

namespace cppual {

namespace generic {

template<typename T>
struct traits
{
    typedef T type;
};

template<typename Object>
struct conversion_type
{
    using type = typename traits<Object>::type;
};

} // namespace generic

}

#endif // CPPUAL_GENERIC_ITERATOR_TRAITS_HPP
