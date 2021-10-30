#pragma once

#include <experimental/mdspan>

namespace metacore {

template<class T, ptrdiff_t... Exts>
using mdspan = std::experimental::mdspan<T, Exts...>;

} // namespace metacore