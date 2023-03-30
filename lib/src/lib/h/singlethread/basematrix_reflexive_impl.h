#ifndef _MDL_MATH_SINGLE_THREAD_BASE_MATRIX_REFLEXIVE_IMPL
#define _MDL_MATH_SINGLE_THREAD_BASE_MATRIX_REFLEXIVE_IMPL

#include "../basematrix.h"

namespace mdl {
namespace math {
namespace singlethread {

  class BaseMatrixReflexiveImpl {
    public:
      template <typename Operation>
      static void ReflexiveOperate(BaseMatrix* self, float_t scalar) {
        size_t rows = self->NumRows();
        size_t cols = self->NumCols();

        for (size_t row = 0; row < rows; row++) {
          for (size_t col = 0; col < cols; col++) {
            Operation::operate((*self)(row, col), scalar);
          }
        }
      }

      template <typename Operation>
      static void ReflexiveOperate(BaseMatrix* self, const BaseMatrix& other) {
        size_t rows1 = self->NumRows();
        size_t cols1 = self->NumCols();
        size_t rows2 = other.NumRows();
        size_t cols2 = other.NumCols();

        if ((rows1 == rows2 && (cols1 == cols2 || cols2 == 1))
            || (cols1 == cols2 && rows2 == 1)
            || (rows2 == 1 && cols2 == 1)) {

          for (size_t row = 0; row < rows1; row++) {
            for (size_t col = 0; col < cols1; col++) {
              Operation::operate(
                  (*self)(row, col), 
                  other(rows2 == 1 ? 0 : row, cols2 == 1 ? 0 : col));
            }
          }
        } else {
          std::ostringstream os;
          os << "Cannot operate on matrices of different dimensions: " << rows1 << 'x' << cols1
              << " and " << rows2 << 'x' << cols2;
          throw std::invalid_argument(os.str());
        }
      }

      template <typename Operation>
      static void ReflexiveUnaryOperate(BaseMatrix* self) {
        size_t rows = self->NumRows();
        size_t cols = self->NumCols();

        for (size_t row = 0; row < rows; row++) {
          for (size_t col = 0; col < cols; col++) {
            Operation::operate((*self)(row, col));
          }
        }
      }
  };
} // namespace singlethread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_SINGLE_THREAD_BASE_MATRIX_REFLEXIVE_IMPL