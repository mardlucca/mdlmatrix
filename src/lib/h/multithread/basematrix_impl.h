#ifndef _MDL_MATH_MULTI_THREAD_BASE_MATRIX_IMPL
#define _MDL_MATH_MULTI_THREAD_BASE_MATRIX_IMPL

#include "../matrix.h"
#include "helper.h"

namespace mdl {
namespace math {
namespace multithread {

  class BaseMatrixImpl {
    public:

      template <typename Operation>
      static Matrix Operate(float_t scalar, const BaseMatrix& matrix) {
        Matrix result(matrix.NumRows(), matrix.NumCols());

        Partition(matrix.NumRows() * matrix.NumCols(), [&matrix, &result, scalar](size_t from, size_t to) {
          return [&matrix, &result, from, to, scalar]() { 
            size_t cols = matrix.NumCols();
            size_t frow = from / cols;
            size_t fcol = from % cols;
            size_t trow = to / cols;
            size_t tcol = to % cols;

            for (size_t row = frow, col = fcol; 
                row != trow || col != tcol;) {
              Operation::operate(scalar, matrix(row, col), result(row, col));
              col++;
              if (col >= cols) { col = 0; row++;}
            }
            return 0; 
          };
        });

        return result;
      }

      template <typename Operation>
      static Matrix Operate(const BaseMatrix& matrix, float_t scalar) {
        Matrix result(matrix.NumRows(), matrix.NumCols());

        Partition(matrix.NumRows() * matrix.NumCols(), [&matrix, &result, scalar](size_t from, size_t to) {
          return [&matrix, &result, from, to, scalar]() { 
            size_t cols = matrix.NumCols();
            size_t frow = from / cols;
            size_t fcol = from % cols;
            size_t trow = to / cols;
            size_t tcol = to % cols;

            for (size_t row = frow, col = fcol; 
                row != trow || col != tcol;) {
              Operation::operate(matrix(row, col), scalar, result(row, col));
              col++;
              if (col >= cols) { col = 0; row++;}
            }
            return 0; 
          };
        });

        return result;
      }

      template <typename Operation>
      static Matrix Operate(
          const BaseMatrix& matrix1, 
          const BaseMatrix& matrix2) {
        size_t rows1 = matrix1.NumRows();
        size_t cols1 = matrix1.NumCols();
        size_t rows2 = matrix2.NumRows();
        size_t cols2 = matrix2.NumCols();

        if ((rows1 == rows2 && 
                (cols1 == cols2 || cols1 == 1 || cols2 == 1))
            || (cols1 == cols2 && 
                (rows1 == 1 || rows2 == 1))
            || (rows1 == 1 && cols1 == 1)
            || (rows2 == 1 && cols2 == 1)) {

          size_t rows = std::max(rows1, rows2);
          size_t cols = std::max(cols1, cols2);
          Matrix result(rows, cols);

          Partition(rows * cols, [&matrix1, &matrix2, &result, cols](size_t from, size_t to) {
            return [&matrix1, &matrix2, &result, from, to, cols]() { 
              size_t rows1 = matrix1.NumRows() - 1;
              size_t cols1 = matrix1.NumCols() - 1;
              size_t rows2 = matrix2.NumRows() - 1;
              size_t cols2 = matrix2.NumCols() - 1;
              size_t frow = from / cols;
              size_t fcol = from % cols;
              size_t trow = to / cols;
              size_t tcol = to % cols;

              for (size_t row = frow, col = fcol; 
                  row != trow || col != tcol;) {
                Operation::operate(
                    matrix1(row <= rows1 ? row : rows1, col <= cols1 ? col : cols1), 
                    matrix2(row <= rows2 ? row : rows2, col <= cols2 ? col : cols2), 
                    result(row, col));
                col++;
                if (col >= cols) { col = 0; row++;}
              }
              return 0; 
            };
          });

          return result;
        }

        std::ostringstream os;
        os << "Cannot operate on matrices of different dimensions: " << rows1 << 'x' << cols1
            << " and " << rows2 << 'x' << cols2;
        throw std::invalid_argument(os.str());
      }

      template <typename Operation>
      static Matrix UnaryOperate(const BaseMatrix& matrix) {
        Matrix result(matrix.NumRows(), matrix.NumCols());

        Partition(matrix.NumRows() * matrix.NumCols(), [&matrix, &result](size_t from, size_t to) {
          return [&matrix, &result, from, to]() { 
            size_t cols = matrix.NumCols();
            size_t frow = from / cols;
            size_t fcol = from % cols;
            size_t trow = to / cols;
            size_t tcol = to % cols;

            for (size_t row = frow, col = fcol; 
                row != trow || col != tcol;) {
              Operation::operate(matrix(row, col), result(row, col));
              col++;
              if (col >= cols) { col = 0; row++;}
            }
            return 0; 
          };
        });

        return result;
      }

      template <typename Operation>
      static Matrix RowReduce(const BaseMatrix& matrix, float_t initialValue = 0.0) {
        Matrix result(
            matrix.NumRows(), 
            1, 
            [initialValue] (size_t row, size_t col) { return initialValue; });

        Partition(matrix.NumRows(), [&matrix, &result](size_t from, size_t to) {
          return [&matrix, &result, from, to]() { 
            for (size_t row = from; row < to; row++) {
              for (size_t col = 0; col < matrix.NumCols(); col++) {
                Operation::operate(result(row, 0), matrix(row, col));
              }
            }
            return 0; 
          };
        });

        return result;
      }

      template <typename Operation>
      static Matrix ColReduce(const BaseMatrix& matrix, float_t initialValue = 0.0) {
        Matrix result(1, 
            matrix.NumCols(), 
            [initialValue] (size_t row, size_t col) { return initialValue; });

        Partition(matrix.NumCols(), [&matrix, &result](size_t from, size_t to) {
          return [&matrix, &result, from, to]() { 
            for (size_t row = 0; row < matrix.NumRows(); row++) {
              for (size_t col = from; col < to; col++) {
                Operation::operate(result(0, col), matrix(row, col));
              }
            }
            return 0; 
          };
        });

        return result;
      }

      template <typename Operation>
      static Matrix Reduce(const BaseMatrix& matrix, float_t initialValue = 0.0) {
        return matrix.NumRows() > 1
            ? ColReduce<Operation>(matrix, initialValue)
            : RowReduce<Operation>(matrix, initialValue);
      }
  };
} // namespace multithread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_MULTI_THREAD_BASE_MATRIX_IMPL