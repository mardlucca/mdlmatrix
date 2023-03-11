#include "../h/matrix.h"
#include "../h/metal/matriximpl.h"
#include "../h/multithread/matriximpl.h"
#include "../h/singlethread/matriximpl.h"

#include <mdl/profiler.h>

namespace mdl {
namespace math {
  Matrix::Matrix() : Matrix(0, 0) {}

  Matrix::Matrix(size_t rows, size_t cols) 
      : rows(rows), cols(cols), 
        data(std::shared_ptr<float_t[]>(new float_t[rows * cols])) {
    float_t * ptr = data.get();

    for (size_t i = 0; i < rows * cols; i++) {
        ptr[i] = 0.0;
    }
  }

  Matrix::Matrix(size_t rows, size_t cols, float_t data[]) 
      : rows(rows), cols(cols), 
        data(std::shared_ptr<float_t[]>(data)) {
  }

  Matrix::Matrix(size_t rows, size_t cols, std::function<float_t (size_t, size_t)> initFn)
      : rows(rows), cols(cols), 
        data(std::shared_ptr<float_t[]>(new float_t[rows * cols])) {
    float_t * ptr = data.get();

    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        *ptr = initFn(row, col);
        ptr++;
      }
    }
  }

  Matrix::Matrix(const Matrix& other)
      : rows(other.rows), cols(other.cols), 
        data(std::shared_ptr<float_t[]>(new float_t[rows * cols])) {
    std::memcpy(data.get(), other.data.get(), rows * cols * sizeof(float_t));
  }

  Matrix::Matrix(Matrix&& other)
      : rows(other.rows), cols(other.cols), 
        data(other.data) {
  }

  Matrix::Matrix(const BaseMatrix& other) 
      : rows(other.NumRows()), cols(other.NumCols()),
        data(new float_t[rows * cols]) {
    float_t * buffer = data.get();

    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        buffer[row * cols + col] = other(row, col);
      }
    }
  }

  Matrix& Matrix::operator=(const Matrix& other) {
    rows = other.rows;
    cols = other.cols;
    data.reset(new float_t[rows * cols]);
    std::memcpy(data.get(), other.data.get(), rows * cols * sizeof(float_t));

    return *this;
  }

  Matrix& Matrix::operator=(Matrix&& other) {
    rows = other.rows;
    cols = other.cols;
    data = other.data;

    return *this;
  }

  Matrix& Matrix::operator=(const BaseMatrix& other) {
    rows = other.NumRows();
    cols = other.NumCols();

    data.reset(new float_t[rows * cols]);
    float_t * buffer = data.get();

    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        buffer[row * cols + col] = other(row, col);
      }
    }

    return *this;
  }

  std::ostream& Matrix::operator<<(std::ostream& os) {
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        os << '\t' << data.get()[row * cols + col];
      }
      os << endl;
    }
    return os;
  }

  Matrix& Matrix::operator+=(const Matrix& other) {
    ReflexiveOperate<op::Addition>(other);
    return *this;
  }

  Matrix& Matrix::operator+=(float_t scalar) {
    ReflexiveOperate<op::Addition>(scalar);
    return *this;
  }

  Matrix& Matrix::operator-=(const Matrix& other) {
    ReflexiveOperate<op::Subtraction>(other);
    return *this;
  }

  Matrix& Matrix::operator-=(float_t scalar) {
    ReflexiveOperate<op::Subtraction>(scalar);
    return *this;
  }

  Matrix& Matrix::operator*=(const Matrix& other) {
    ReflexiveOperate<op::Multiplication>(other);
    return *this;
  }

  Matrix& Matrix::operator*=(float_t scalar) {
    ReflexiveOperate<op::Multiplication>(scalar);
    return *this;
  }

  Matrix& Matrix::operator/=(const Matrix& other) {
    ReflexiveOperate<op::Division>(other);
    return *this;
  }

  Matrix& Matrix::operator/=(float_t scalar) {
    ReflexiveOperate<op::Division>(scalar);
    return *this;
  }

  Slice<LeftRange, LeftRange, DirectAccessor> Matrix::operator()() {
    return (*this)(LeftRange(rows), LeftRange(cols));
  }
  const Slice<LeftRange, LeftRange, DirectAccessor> Matrix::operator()() const {
    return (*this)(LeftRange(rows), LeftRange(cols));
  }

  Matrix Matrix::Transpose() const {
    return multithread::MatrixImpl::Transpose(*this);
  }
} // math
} // mdl
