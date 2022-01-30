#include <cppual/memory_resource>
#include <cppual/circular_queue>

#include <iostream>

int main (int /*argc*/, char** /*argv*/)
{
    constexpr auto max_vectors = 1000U;

    typedef int                                  value_type  ;
    typedef cppual::circular_queue<value_type  > value_vector;
    typedef cppual::circular_queue<value_vector> vectors     ;

    cppual::memory::stacked_resource res((sizeof (value_vector) * max_vectors) +
                                         (sizeof (value_type  ) * max_vectors  * max_vectors));

    cppual::memory::set_default_resource (res);

    std::cout << "resource max_size: " << cppual::memory::get_default_resource ()->max_size ()
              << " bytes" << std::endl;

    vectors vec;

    std::cout << "vec max_size: " << vec.get_allocator ().max_size ()
              << " elements" << std::endl;

    vec.reserve (max_vectors);

    for (auto i = 0U; i < max_vectors; ++i)
    {
        vec.emplace_back();

        vec[i].reserve (max_vectors);

        for (auto n = 0U; n < max_vectors; ++n)
        {
            vec[i].push_back (std::rand ());
        }

        std::cout << "vec[" << i << "] max_size: " << vec.get_allocator ().max_size ()
                  << " elements" << std::endl;
    }

    for (auto i = 0U; i < max_vectors; ++i)
    {
        vec.pop_back ();
    }

    return 0;
}
