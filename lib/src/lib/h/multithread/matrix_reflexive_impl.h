#ifndef _MDL_MATH_MULTI_THREAD_MATRIX_REFLEXIVE_IMPL
#define _MDL_MATH_MULTI_THREAD_MATRIX_REFLEXIVE_IMPL

#include "../matrix.h"
#include "helper.h"

namespace mdl {
namespace math {
namespace multithread {

  class MatrixReflexiveImpl {
    public:
      template <typename Operation>
      static void ReflexiveOperate(Matrix * self, float_t scalar) {
        Partition(self->NumCells(), 
            [self, scalar](size_t from, size_t to) {
          float_t * selfData = self->data.get();

          return [from, to, selfData, scalar]() { 
            // #pragma clang loop vectorize(enable) 
            for (size_t i = from; i < to; i++) {
              Operation::operate(selfData[i], scalar);
            }
            return 0;
          };
        });
      }

      template <typename Operation>
      static void ReflexiveOperate(Matrix * self, const Matrix& other) {
        size_t rows = self->rows;
        size_t cols = self->cols;
        size_t rows2 = other.rows;
        size_t cols2 = other.cols;

        if (rows == rows2) {
          if (cols == cols2) {
            // matrices with same size
            MatrixReflexiveOperate<Operation>(self, other);
          } else if (cols2 == 1) {
            // other is a colVector
            MatrixColVectorReflexiveOperate<Operation>(self, other);
          } else {
            std::ostringstream os;
            os << "Cannot operate on matrices of different dimensions: " << rows << 'x' << cols
                << " and " << rows2 << 'x' << cols2;
            throw std::invalid_argument(os.str());
          }
        } else if (cols == cols2 && rows2 == 1) {
          // other is a vector
          MatrixVectorReflexiveOperate<Operation>(self, other);
        } else if (rows2 == 1 && cols2 == 1) {
          // other is a scalar
          ReflexiveOperate<Operation>(self, other(0, 0));
        } else {

          std::ostringstream os;
          os << "Cannot operate on matrices of different dimensions: " << rows << 'x' << cols
              << " and " << rows2 << 'x' << cols2;
          throw std::invalid_argument(os.str());
        }
      }

      template <typename Operation>
      static void ReflexiveUnaryOperate(Matrix * self) {
        Partition(self->NumCells(), 
            [self](size_t from, size_t to) {
          float_t * selfData = self->data.get();

          return [from, to, selfData]() { 
            // #pragma clang loop vectorize(enable) 
            for (size_t i = from; i < to; i++) {
              Operation::operate(selfData[i]);
            }
            return 0;
          };
        });
      }

    private:
      template <typename Operation>
      static void MatrixReflexiveOperate(
          Matrix * self, 
          const Matrix& other) {
        Partition(self->NumCells(), 
            [self, &other](size_t from, size_t to) {
          float_t * selfData = self->data.get();
          float_t * otherData = other.data.get();

          return [from, to, selfData, otherData]() { 
            // #pragma clang loop vectorize(enable) 
            for (size_t i = from; i < to; i++) {
              Operation::operate(selfData[i], otherData[i]);
            }
            return 0;
          };
        });
      }
      
      template <typename Operation>
      static void MatrixVectorReflexiveOperate(
          Matrix * self, 
          const Matrix& vector) {
        Partition(self->NumCells(), 
            [&vector, self](size_t from, size_t to) {
          float_t * vData = vector.data.get();
          float_t * selfData = self->data.get();
          size_t cols = self->NumCols();

          return [from, to, vData, selfData, cols]() { 
            for (size_t i = from; i < to; i++) {
              Operation::operate(selfData[i], vData[i % cols]);
            }
            return 0;
          };
        });
      }

      template <typename Operation>
      static void MatrixColVectorReflexiveOperate(
          Matrix * self, 
          const Matrix& vector) {
        Partition(self->NumCells(), 
            [&vector, self](size_t from, size_t to) {
          float_t * vData = vector.data.get();
          float_t * selfData = self->data.get();
          size_t cols = self->NumCols();

          return [from, to, vData, selfData, cols]() { 
            for (size_t i = from; i < to; i++) {
              Operation::operate(selfData[i], vData[i / cols]);
            }
            return 0;
          };
        });
      }      
  };
} // namespace multithread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_SINGLE_THREAD_MATRIX_REFLEXIVE_IMPL