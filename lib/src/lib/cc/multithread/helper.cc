#include "../../h/multithread/helper.h"

#include <mdl/concurrent.h>
#include <mdl/matrix.h>
#include <vector>


namespace mdl {
namespace math {
namespace multithread {

  mdl::concurrent::ThreadFactory factory("yo");
  mdl::concurrent::ExecutorService executor(kNumKernels, factory);

  void Partition(
      size_t numCells, 
      std::function<std::function<int ()> (size_t, size_t)> dispatch, 
      int numKernels) {
    size_t partitionSize = numCells / numKernels;
    size_t mod = numCells % numKernels;

    std::vector<mdl::concurrent::Future<int>> futures;
    size_t left = 0;
    while (left < numCells) {
      size_t right = left + partitionSize + (mod > 0);
      futures.push_back(executor.Submit(dispatch(left, right)));
      mod--;
      left = right;
    }

    // wait for all computations to finish.
    for (auto it = futures.begin(); it != futures.end(); it++) {
      it->Get();
    }
  }

} // namespace multithread
} // namespace math
} // namespace mdl