#ifndef FUNCTIONAL_ITER_H
#define FUNCTIONAL_ITER_H

#include <functional>

// Berechne über für jede Iteration
void for_each_iter(std::size_t& iter, const size_t size, std::function<void(void)> fn);
void for_each_iter(std::size_t& iter, const size_t size, std::function<void(std::size_t&)> fn);


#endif //FUNCTIONAL_ITER_H