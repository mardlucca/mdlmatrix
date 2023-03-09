#include "../../h/impl/multi_thread.h"

#include <mdl/concurrent.h>
#include <mdl/matrix.h>
#include <vector>


namespace mdl {
namespace math {
namespace impl {

  const int kNumKernels = 6;
  mdl::concurrent::ThreadFactory factory("yo");
  mdl::concurrent::ExecutorService executor(kNumKernels, factory);

  void Partition(
      std::size_t numCells, 
      std::function<std::function<int ()> (std::size_t, std::size_t)> dispatch, 
      int numKernels) {
    std::size_t partitionSize = numCells / numKernels;
    size_t mod = numCells % numKernels;

    std::vector<mdl::concurrent::Future<int>> futures;
    std::size_t left = 0;
    while (left < numCells) {
      std::size_t right = left + partitionSize + (mod > 0);
      futures.push_back(executor.Submit(dispatch(left, right)));
      mod--;
      left = right;
    }

    // wait for all computations to finish.
    for (auto it = futures.begin(); it != futures.end(); it++) {
      it->Get();
    }
  }

} // namespace impl
} // namespace math
} // namespace mdl