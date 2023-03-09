#ifndef _MDL_MATH_IMPL_SINGLE_THREAD
#define _MDL_MATH_IMPL_SINGLE_THREAD

#include <mdl/profiler.h>
#include "../matrix.h"

namespace mdl {
namespace math {
namespace impl {

  class SingleThreaded {
    public:

      inline static Matrix Transpose(const Matrix& matrix) {
        int rows = matrix.rows;
        int cols = matrix.cols;
        float_t * data1 = matrix.data.get();
        float_t * data2 = new float_t[rows * cols];
    {
        for (int row = 0; row < rows; row++) {
          for (int col = 0; col < cols; col++) {
            *(data2 + rows * col + row) = *data1;
            data1++;
          }
        }
    }
        return Matrix(cols, rows, data2);
      }
  };
} // namespace impl
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_IMPL_SINGLE_THREAD