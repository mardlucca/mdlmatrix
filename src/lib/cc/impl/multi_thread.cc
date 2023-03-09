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
      int numCells, 
      std::function<std::function<int ()> (int, int)> dispatch, 
      int numKernels) {
    int partitionSize = numCells / numKernels;
    int mod = numCells % numKernels;

    std::vector<mdl::concurrent::Future<int>> futures;
    int left = 0;
    while (left < numCells) {
      int right = left + partitionSize + (mod > 0);
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