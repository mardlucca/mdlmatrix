#ifndef _MDL_MATH_MULTI_THREAD_MATRIXIMPL
#define _MDL_MATH_MULTI_THREAD_MATRIXIMPL

#include <mdl/concurrent.h>
#include <mdl/profiler.h>
#include <functional>
#include <vector>

#include "../matrix.h"
#include "../../h/multithread/helper.h"

namespace mdl {
namespace math {
namespace multithread {

  class MatrixImpl {
    public:

      inline static Matrix Transpose(const Matrix& matrix) {
        size_t rows = matrix.rows;
        size_t cols = matrix.cols;
        size_t numCells = rows * cols;
        float_t * ddata = new float_t[numCells];
        float_t * sdata = matrix.data.get();

        Partition(numCells, [sdata, ddata, rows, cols](size_t from, size_t to) {
          return [sdata, ddata, from, to, rows, cols]() { 
            size_t frow = from / cols;
            size_t fcol = from % cols;
            size_t trow = to / cols;
            size_t tcol = to % cols;
            float_t * runner = sdata + from;

            for (size_t row = frow, col = fcol; 
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
} // namespace multithread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_MULTI_THREAD_MATRIX