#include <cppual/memory_resource>
#include <cppual/circular_queue>

#include <iostream>

void test1 ()
{
    typedef int                                  value_type  ;
    typedef cppual::circular_queue<value_type  > value_vector;
    typedef cppual::circular_queue<value_vector> vectors     ;

    constexpr const value_vector::size_type max_vectors = 1000U;

    cppual::memory::stacked_resource res((sizeof (value_vector) * max_vectors) +
                                         (sizeof (value_type  ) * max_vectors  * max_vectors));

    cppual::memory::set_default_resource (res);

    std::cout << "stacked_resource max_size: " << cppual::memory::get_default_resource ()->max_size ()
              << " bytes" << std::endl;

    vectors vec;

    std::cout << "vec max_size: " << vec.get_allocator ().max_size ()
              << " elements remaining" << std::endl;

    vec.reserve (max_vectors);

    for (auto i = 0U; i < max_vectors; ++i)
    {
        vec.emplace_back ();

        vec[i].reserve (max_vectors);

        for (auto n = 0U; n < max_vectors; ++n)
        {
            vec[i].push_back (static_cast<value_type>(n));
        }

        auto const random_pos = static_cast<value_vector::size_type> (std::rand () % 1000);

        std::cout << "vec[" << i << "][" << random_pos << "] value: " << vec[i][random_pos] << std::endl;

        std::cout << "vec[" << i << "] size: " << vec[i].size ()
                  << " elements\nvec["
                  << i << "] max_size: " << vec[i].get_allocator ().max_size ()
                  << " elements remaining" << std::endl;
    }

    for (auto i = 0U; i < max_vectors; ++i)
    {
        vec.pop_back ();
    }
}

void test2 ()
{
    typedef int                                  value_type  ;
    typedef cppual::circular_queue<value_type  > value_vector;
    typedef cppual::circular_queue<value_vector> vectors     ;

    constexpr const value_vector::size_type max_vectors = 1000U;

    cppual::memory::set_default_resource (*cppual::memory::new_delete_resource ());

    cppual::memory::dstacked_resource res((sizeof (value_vector) * max_vectors) +
                                          (sizeof (value_type  ) * max_vectors  * max_vectors),
                                           sizeof (value_vector) * max_vectors);

    cppual::memory::set_default_resource (res);

    std::cout << "dstacked_resource max_size: " << cppual::memory::get_default_resource ()->max_size ()
              << " bytes" << std::endl;

    vectors vec;

    std::cout << "vec max_size: " << vec.get_allocator ().max_size ()
              << " elements remaining" << std::endl;

    vec.reserve (max_vectors);

    for (auto i = 0U; i < max_vectors; ++i)
    {
        vec.emplace_back ();

        vec[i].reserve (max_vectors);

        for (auto n = 0U; n < max_vectors; ++n)
        {
            vec[i].push_back (static_cast<value_type>(n));
        }

        auto const random_pos = static_cast<value_vector::size_type> (std::rand () % 1000);

        std::cout << "vec[" << i << "][" << random_pos << "] value: " << vec[i][random_pos] << std::endl;

        std::cout << "vec[" << i << "] size: " << vec[i].size ()
                  << " elements\nvec["
                  << i << "] max_size: " << vec[i].get_allocator ().max_size ()
                  << " elements remaining" << std::endl;
    }

    for (auto i = 0U; i < max_vectors; ++i)
    {
        vec.pop_back ();
    }
}

void test3 ()
{
    auto size_val1 = static_cast<std::size_t>(-1);
    auto size_val2 = std::size_t () - 1;

    auto zero_val  = std::size_t ();

    std::cout << "size_val1: "   << size_val1
              << "\nsize_val2: " << size_val2 << std::endl;

    std::cout << "size_val1 incremented: "   << ++size_val1
              << "\nsize_val2 incremented: " << ++size_val2 << std::endl;

    std::cout << "size_val1 decremented: "   << --size_val1
              << "\nsize_val2 decremented: " << --size_val2 << std::endl;

    std::cout << "zero_val decremented: " << --zero_val << std::endl;

    std::cout << "zero_val < 0: " << (zero_val < 0) << std::endl;
}

int main (int /*argc*/, char** /*argv*/)
{
    std::cout << "\n============ Test 1 ============\n" << std::endl;

    test1 ();

    std::cout << "\n============ Test 2 ============\n" << std::endl;

    test2 ();

    std::cout << "\n============ Test 3 ============\n" << std::endl;

    test3 ();

    return 0;
}
