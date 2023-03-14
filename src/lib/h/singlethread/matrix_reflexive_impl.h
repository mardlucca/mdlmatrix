#ifndef _MDL_MATH_SINGLE_THREAD_MATRIX_REFLEXIVE_IMPL
#define _MDL_MATH_SINGLE_THREAD_MATRIX_REFLEXIVE_IMPL

#include "../matrix.h"

namespace mdl {
namespace math {
namespace singlethread {

  class MatrixReflexiveImpl {
    public:
      template <typename Operation> 
      static void ReflexiveOperate(Matrix* self, float_t scalar) {
        size_t length = self->rows * self->cols;
        float_t * cell = self->data.get();
        for (size_t i = 0; i < length; i++) {
          Operation::operate(*cell, scalar);
          cell++;
        }
      }

      template <typename Operation>
      static void ReflexiveOperate(Matrix* self, const Matrix& other) {
        size_t rows = self->rows;
        size_t cols = self->cols;
        size_t rows2 = other.rows;
        size_t cols2 = other.cols;

        if ((rows == rows2 && (cols == cols2 || cols2 == 1))
            || (cols == cols2 && rows2 == 1)
            || (rows2 == 1 && cols2 == 1)) {


          float_t * thisBuffer = self->data.get();
          float_t * otherBuffer = other.data.get();
          for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
              Operation::operate(
                  thisBuffer[row * cols + col], 
                  otherBuffer[(rows2 == 1 ? 0 : row) * cols2 + (cols2 == 1 ? 0 : col)]);
            }
          }
        } else {
          std::ostringstream os;
          os << "Cannot operate on matrices of different dimensions: " << rows << 'x' << cols
              << " and " << rows2 << 'x' << cols2;
          throw std::invalid_argument(os.str());
        }
      }

      template <typename Operation>
      static void ReflexiveUnaryOperate(Matrix* self) {
        size_t length = self->rows * self->cols;
        float_t * cell = self->data.get();
        for (size_t i = 0; i < length; i++) {
          Operation::operate(*cell);
          cell++;
        }
      }    
  };
} // namespace singlethread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_SINGLE_THREAD_MATRIX_REFLEXIVE_IMPL