#ifndef _MDL_MATH_MULTI_THREAD_HELPER
#define _MDL_MATH_MULTI_THREAD_HELPER

#include <mdl/concurrent.h>
#include <mdl/profiler.h>
#include <functional>
#include <vector>

#include "../matrix.h"

namespace mdl {
namespace math {
namespace multithread {
  const int kNumKernels = 8;

  // Lamba takes Matrix, start index and final index
  void Partition(
      size_t numCells, 
      std::function<std::function<int ()> (size_t, size_t)> dispatch, 
      int numKernels = kNumKernels);

} // namespace multithread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_MULTI_THREAD_HELPER