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
      std::size_t numCells, 
      std::function<std::function<int ()> (std::size_t, std::size_t)> dispatch, 
      int numKernels = kNumKernels);

  class MultiThreaded {
    public:

      inline static Matrix Transpose(const Matrix& matrix) {
        std::size_t rows = matrix.rows;
        std::size_t cols = matrix.cols;
        std::size_t numCells = rows * cols;
        float_t * ddata = new float_t[numCells];
        float_t * sdata = matrix.data.get();

        Partition(numCells, [sdata, ddata, rows, cols](std::size_t from, std::size_t to) {
          return [sdata, ddata, from, to, rows, cols]() { 
            std::size_t frow = from / cols;
            std::size_t fcol = from % cols;
            std::size_t trow = to / cols;
            std::size_t tcol = to % cols;
            float_t * runner = sdata + from;

            for (std::size_t row = frow, col = fcol; 
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