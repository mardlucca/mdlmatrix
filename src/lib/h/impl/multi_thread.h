#ifndef _MDL_MATH_IMPL_MULTI_THREAD
#define _MDL_MATH_IMPL_MULTI_THREAD

#include <mdl/concurrent.h>
#include <mdl/profiler.h>
#include <functional>
#include <vector>

#include "../matrix.h"

namespace mdl {
namespace math {
namespace impl {
  extern const int kNumKernels;

  // Lamba takes Matrix, start index and final index
  void Partition(
      int numCells, 
      std::function<std::function<int ()> (int, int)> dispatch, 
      int numKernels = kNumKernels);

  class MultiThreaded {
    public:

      inline static Matrix Transpose(const Matrix& matrix) {
        int rows = matrix.rows;
        int cols = matrix.cols;
        int numCells = rows * cols;
        float_t * ddata = new float_t[numCells];
        float_t * sdata = matrix.data.get();

        Partition(numCells, [sdata, ddata, rows, cols](int from, int to) {
          return [sdata, ddata, from, to, rows, cols]() { 
            int frow = from / cols;
            int fcol = from % cols;
            int trow = to / cols;
            int tcol = to % cols;
            float_t * runner = sdata + from;

            for (int row = frow, col = fcol; 
                row != trow || col != tcol;) {

              *(ddata + rows * col + row) = *runner;
              col++;
              runner++;
              if (col >= cols) { col = 0; row++;}
            }
            return 0; 
          };
        });

        return Matrix(cols, rows, ddata);
      }
  };
} // namespace impl
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_IMPL_SINGLE_THREAD