#include "../h/basematrix.h"

#include "../h/misc.h"

namespace mdl {
namespace math {

  BaseMatrix::~BaseMatrix() {}


  bool BaseMatrix::Equals(const BaseMatrix& other) const {
    if (this == &other) { return true; }

    size_t rows = NumRows();
    size_t cols = NumCols();

    if (rows != other.NumRows() || cols != other.NumCols()) { return false; }

    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
      float_t diff = std::abs((*this)(row, col) - other(row, col));
        if (diff > kFloatPrecision) {
          return false;
        }
      }
    }

    return true;
  }

  float_t& BaseMatrix::operator[](size_t index) {
    return operator()(index / NumCols(), index % NumCols());
  }

  float_t BaseMatrix::operator[](size_t index) const {
    return operator()(index / NumCols(), index % NumCols());
  }

  bool BaseMatrix::Equals(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return matrix1.Equals(matrix2);
  }

  BaseMatrix& BaseMatrix::ForEach(std::function<void (size_t, size_t, float_t&)> fn) {
    size_t rows = NumRows();
    size_t cols = NumCols();

    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        fn(row, col, (*this)(row, col));
      }
    }

    return *this;
  }

  BaseMatrix& BaseMatrix::RowSwap(size_t row1, size_t row2) {
    if (row1 == row2) { return *this; }

    size_t cols = NumCols();

    for (size_t col = 0; col < cols; col++) {
      float_t temp = (*this)(row1, col);
      (*this)(row1, col) = (*this)(row2, col);
      (*this)(row2, col) = temp;
    }

    return *this;
  }

  BaseMatrix& BaseMatrix::Shuffle() {
    if (NumRows() == 1) {
      for (size_t i = NumCols() - 1; i > 0; i--) {
        size_t j = NextRand(i + 1);
        float_t temp = (*this)(0, i);
        (*this)(0, i) = (*this)(0, j);
        (*this)(0, j) = temp;
      }
    } else {
      for (size_t i = NumRows() - 1; i > 0; i--) {
        size_t j = NextRand(i + 1);
        RowSwap(i, j);
      }
    }

    return *this;
  }


  void BaseMatrix::Copy(const BaseMatrix& other) {
    size_t rows = std::min(NumRows(), other.NumRows());
    size_t cols = std::min(NumCols(), other.NumCols());
    
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        (*this)(row, col) = other(row, col);
      }
    }
  }

  void BaseMatrix::BufferedCopy(const BaseMatrix& other) {
    size_t rows = std::min(NumRows(), other.NumRows());
    size_t cols = std::min(NumCols(), other.NumCols());
    
    //TODO: See about pooling/reusing these things somehow.
    float_t * buffer = new float_t[rows * cols];

    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        buffer[row * cols + col] = other(row, col);
      }
    }

    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        (*this)(row, col) = buffer[row * cols + col];
      }
    }

    delete[] buffer;
  }

  std::ostream& operator<<(std::ostream& os, const BaseMatrix& matrix) {
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        os << '\t' << matrix(row,col);
      }
      os << std::endl;
    }
    return os;
  }

} // math
} // mdl
