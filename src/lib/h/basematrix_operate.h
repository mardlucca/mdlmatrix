#ifndef _MDL_MATH_BASEMATRIX_OPERATE
#define _MDL_MATH_BASEMATRIX_OPERATE

#include <exception>
#include <sstream>

#include "float_t.h"
#include "matrix.h"

namespace mdl {
namespace math {

  template <typename Operation>
  Matrix Operate(float_t scalar, const BaseMatrix& matrix) {
    int rows = matrix.NumRows();
    int cols = matrix.NumCols();
    Matrix result(rows, cols);

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        Operation::operate(scalar, matrix(row, col), result(row, col));
      }
    }

    return result;
  }

  template <typename Operation>
  Matrix Operate(const BaseMatrix& matrix, float_t scalar) {
    int rows = matrix.NumRows();
    int cols = matrix.NumCols();
    Matrix result(rows, cols);

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        Operation::operate(matrix(row, col), scalar, result(row, col));
      }
    }

    return result;
  }

  template <typename Operation>
  Matrix Operate(
      const BaseMatrix& matrix1, 
      const BaseMatrix& matrix2) {
    int rows1 = matrix1.NumRows();
    int cols1 = matrix1.NumCols();
    int rows2 = matrix2.NumRows();
    int cols2 = matrix2.NumCols();

    if ((rows1 == rows2 && 
            (cols1 == cols2 || cols1 == 1 || cols2 == 1))
        || (cols1 == cols2 && 
            (rows1 == 1 || rows2 == 1))
        || (rows1 == 1 && cols1 == 1)
        || (rows2 == 1 && cols2 == 1)) {

      int rows = std::max(rows1, rows2);
      int cols = std::max(cols1, cols2);
      Matrix result(rows, cols);

      rows1--; cols1--; rows2--; cols2--;

      for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
          Operation::operate(
              matrix1(row <= rows1 ? row : rows1, col <= cols1 ? col : cols1), 
              matrix2(row <= rows2 ? row : rows2, col <= cols2 ? col : cols2), 
              result(row, col));
        }
      }

      return result;
    }

    std::ostringstream os;
    os << "Cannot operate on matrices of different dimensions: " << rows1 << 'x' << cols1
        << " and " << rows2 << 'x' << cols2;
    throw std::invalid_argument(os.str());
  }

  template <typename Operation>
  Matrix UnaryOperate(const BaseMatrix& matrix) {
    int rows = matrix.NumRows();
    int cols = matrix.NumCols();
    Matrix result(rows, cols);

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        Operation::operate(matrix(row, col), result(row, col));
      }
    }

    return result;
  }

  template <typename Operation>
  Matrix RowReduce(const BaseMatrix& matrix, float_t initialValue = 0.0) {
    Matrix result(
        matrix.NumRows(), 
        1, 
        [initialValue] (int row, int col) { return initialValue; });

    int rows = matrix.NumRows();
    int cols = matrix.NumCols();

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        Operation::operate(result(row, 0), matrix(row, col));
      }
    }

    return result;
  }

  template <typename Operation>
  Matrix ColReduce(const BaseMatrix& matrix, float_t initialValue = 0.0) {
    Matrix result(1, 
        matrix.NumCols(), 
        [initialValue] (int row, int col) { return initialValue; });

    int rows = matrix.NumRows();
    int cols = matrix.NumCols();

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        Operation::operate(result(0, col), matrix(row, col));
      }
    }

    return result;
  }

  template <typename Operation>
  Matrix Reduce(const BaseMatrix& matrix, float_t initialValue = 0.0) {
    return matrix.NumRows() > 1
        ? ColReduce<Operation>(matrix, initialValue)
        : RowReduce<Operation>(matrix, initialValue);
  }

} // math
} // mdl

#endif// _MDL_MATH_BASEMATRIX_OPERATE