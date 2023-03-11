#ifndef _MDL_MATH_SINGLE_THREAD_MATRIXIMPL
#define _MDL_MATH_SINGLE_THREAD_MATRIXIMPL

#include "../matrix.h"

namespace mdl {
namespace math {
namespace singlethread {

  class MatrixImpl {
    public:

      inline static Matrix Transpose(const Matrix& matrix) {
        size_t rows = matrix.rows;
        size_t cols = matrix.cols;
        float_t * data1 = matrix.data.get();
        float_t * data2 = new float_t[rows * cols];

        for (size_t row = 0; row < rows; row++) {
          for (size_t col = 0; col < cols; col++) {
            *(data2 + rows * col + row) = *data1;
            data1++;
          }
        }

        return Matrix(cols, rows, data2);
      }
  };
} // namespace singlethread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_SINGLE_THREAD_MATRIXIMPL