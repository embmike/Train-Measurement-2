#include "functional_iter.hpp"


void for_each_iter(std::size_t& iter, const size_t size, std::function<void(void)> fn)
{
    for(; iter < size; iter++)
    {
        fn();
    }
}

void for_each_iter(std::size_t& iter, const size_t size, std::function<void(std::size_t&)> fn)
{
    for(; iter < size; iter++)
    {
        fn(iter);
    }   
}