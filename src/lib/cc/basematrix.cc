#include "../h/basematrix.h"

namespace mdl {
namespace math {

  BaseMatrix::~BaseMatrix() {}


  bool BaseMatrix::Equals(const BaseMatrix& other) const {
    if (this == &other) { return true; }

    int rows = NumRows();
    int cols = NumCols();

    if (rows != other.NumRows() || cols != other.NumCols()) { return false; }

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        if ((*this)(row, col) != other(row, col)) { return false; }
      }
    }

    return true;
  }

  bool BaseMatrix::Equals(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return matrix1.Equals(matrix2);
  }

  void BaseMatrix::ForEach(std::function<void (int, int, double&)> fn) {
    int rows = NumRows();
    int cols = NumCols();

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        fn(row, col, (*this)(row, col));
      }
    }
  }

  void BaseMatrix::Copy(const BaseMatrix& other) {
    int rows = std::min(NumRows(), other.NumRows());
    int cols = std::min(NumCols(), other.NumCols());
    
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        (*this)(row, col) = other(row, col);
      }
    }
  }

  void BaseMatrix::BufferedCopy(const BaseMatrix& other) {
    int rows = std::min(NumRows(), other.NumRows());
    int cols = std::min(NumCols(), other.NumCols());
    
    //TODO: See about pooling/reusing these things somehow.
    double * buffer = new double[rows * cols];

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        buffer[row * cols + col] = other(row, col);
      }
    }

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        (*this)(row, col) = buffer[row * cols + col];
      }
    }

    delete[] buffer;
  }

  std::ostream& operator<<(std::ostream& os, const BaseMatrix& matrix) {
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        os << '\t' << matrix(row,col);
      }
      os << std::endl;
    }
    return os;
  }

} // math
} // mdl
