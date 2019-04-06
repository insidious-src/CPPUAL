#ifndef CPPUAL_GENERIC_SIGNATURE_HPP
#define CPPUAL_GENERIC_SIGNATURE_HPP

#define SIGNATURE(NAME) cppual::generic::signature<decltype(NAME), NAME>

namespace cppual
{
namespace generic
{
template<typename Signature, Signature & S>
class signature;
}
}

#endif // CPPUAL_GENERIC_SIGNATURE_HPP
