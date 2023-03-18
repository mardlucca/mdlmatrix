#ifndef _MDL_MATH_MULTI_THREAD_BASE_MATRIX_REFLEXIVE_IMPL
#define _MDL_MATH_MULTI_THREAD_BASE_MATRIX_REFLEXIVE_IMPL

#include "../basematrix.h"
#include "helper.h"

namespace mdl {
namespace math {
namespace multithread {

  class BaseMatrixReflexiveImpl {
    public:
      template <typename Operation>
      static void ReflexiveOperate(BaseMatrix* self, float_t scalar) {
        Partition(self->NumRows() * self->NumCols(), [self, scalar](size_t from, size_t to) {
          return [self, from, to, scalar]() { 
            size_t cols = self->NumCols();
            size_t frow = from / cols;
            size_t fcol = from % cols;
            size_t trow = to / cols;
            size_t tcol = to % cols;

            for (size_t row = frow, col = fcol; 
                row != trow || col != tcol;) {
              Operation::operate((*self)(row, col), scalar);
              col++;
              if (col >= cols) { col = 0; row++;}
            }
            return 0; 
          };
        });
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

          Partition(rows1 * cols1, [self, &other](size_t from, size_t to) {
            return [self, from, to, &other]() { 
              size_t cols = self->NumCols();
              size_t rows2 = other.NumRows();
              size_t cols2 = other.NumCols();
              size_t frow = from / cols;
              size_t fcol = from % cols;
              size_t trow = to / cols;
              size_t tcol = to % cols;

              for (size_t row = frow, col = fcol; 
                  row != trow || col != tcol;) {
                Operation::operate(
                    (*self)(row, col), 
                    other(rows2 == 1 ? 0 : row, cols2 == 1 ? 0 : col));
                col++;
                if (col >= cols) { col = 0; row++;}
              }
              return 0; 
            };
          });

        } else {
          std::ostringstream os;
          os << "Cannot operate on matrices of different dimensions: " << rows1 << 'x' << cols1
              << " and " << rows2 << 'x' << cols2;
          throw std::invalid_argument(os.str());
        }
      }

      template <typename Operation>
      static void ReflexiveUnaryOperate(BaseMatrix* self) {
        Partition(self->NumRows() * self->NumCols(), [self](size_t from, size_t to) {
          return [self, from, to]() { 
            size_t cols = self->NumCols();
            size_t frow = from / cols;
            size_t fcol = from % cols;
            size_t trow = to / cols;
            size_t tcol = to % cols;

            for (size_t row = frow, col = fcol; 
                row != trow || col != tcol;) {
              Operation::operate((*self)(row, col));
              col++;
              if (col >= cols) { col = 0; row++;}
            }
            return 0; 
          };
      });
    }
  };
} // namespace multithread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_SINGLE_THREAD_BASE_MATRIX_REFLEXIVE_IMPL