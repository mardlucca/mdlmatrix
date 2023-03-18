#ifndef _MDL_MATH_MULTI_THREAD_MATRIX_IMPL
#define _MDL_MATH_MULTI_THREAD_MATRIX_IMPL

#include <mdl/concurrent.h>
#include <mdl/profiler.h>
#include <functional>
#include <vector>

#include "../matrix.h"
#include "../../h/multithread/helper.h"

namespace mdl {
namespace math {
namespace multithread {

  class MatrixImpl {
    public:
      static Matrix Multiply(const Matrix& matrix1, const Matrix& matrix2);

      inline static Matrix Transpose(const Matrix& matrix);

      template <typename Operation>
      static Matrix Operate(float_t scalar, const Matrix& matrix);

      template <typename Operation>
      static Matrix Operate(const Matrix& matrix, float_t scalar);

      template <typename Operation>
      static Matrix Operate(const Matrix& matrix1, const Matrix& matrix2);

      template <typename Operation>
      static Matrix UnaryOperate(const Matrix& matrix);

      template <typename Operation>
      static Matrix RowReduce(const Matrix& matrix, float_t initialValue = 0.0);

      template <typename Operation>
      static Matrix ColReduce(const Matrix& matrix, float_t initialValue = 0.0);

      template <typename Operation>
      static Matrix Reduce(const Matrix& matrix, float_t initialValue = 0.0);

    private:
      template <typename Operation>
      static Matrix MatrixOperate(const Matrix& matrix1, const Matrix& matrix2);
      
      template <typename Operation>
      static Matrix VectorMatrixOperate(const Matrix& vector, const Matrix& matrix);

      template <typename Operation>
      static Matrix MatrixVectorOperate(const Matrix& matrix, const Matrix& vector);

      template <typename Operation>
      static Matrix ColVectorMatrixOperate(const Matrix& vector, const Matrix& matrix);

      template <typename Operation>
      static Matrix MatrixColVectorOperate(const Matrix& matrix, const Matrix& vector);
  };


  Matrix MatrixImpl::Transpose(const Matrix& matrix) {
    size_t rows = matrix.rows;
    size_t cols = matrix.cols;
    size_t numCells = rows * cols;
    float_t * ddata = new float_t[numCells];
    float_t * sdata = matrix.data.get();

    Partition(numCells, [sdata, ddata, rows, cols](size_t from, size_t to) {
      return [sdata, ddata, from, to, rows, cols]() { 
        size_t frow = from / cols;
        size_t fcol = from % cols;
        size_t trow = to / cols;
        size_t tcol = to % cols;
        float_t * runner = sdata + from;

        for (size_t row = frow, col = fcol; 
            row != trow || col != tcol;) {

          *(ddata + rows * col + row) = *runner;
          col++;
          runner++;
          if (col >= cols) { col = 0; row++;}
        }
        return 0; 
      };
    });

    return Matrix(cols, rows, ddata);
  }


  template <typename Operation>
  Matrix MatrixImpl::Operate(float_t scalar, const Matrix& matrix) {
    Matrix result(matrix.NumRows(), matrix.NumCols());

    Partition(matrix.NumCells(), 
        [&matrix, &result, scalar](size_t from, size_t to) {
      float_t * mData = matrix.data.get();
      float_t * outData = result.data.get();

      return [from, to, mData, outData, scalar]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(scalar, mData[i], outData[i]);
        }
        return 0;
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::Operate(const Matrix& matrix, float_t scalar) {
    Matrix result(matrix.NumRows(), matrix.NumCols());

    Partition(matrix.NumCells(), 
        [&matrix, &result, scalar](size_t from, size_t to) {
      float_t * mData = matrix.data.get();
      float_t * outData = result.data.get();

      return [from, to, mData, outData, scalar]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(mData[i], scalar, outData[i]);
        }
        return 0;
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::Operate(
      const Matrix& matrix1, 
      const Matrix& matrix2) {
    size_t rows1 = matrix1.NumRows();
    size_t cols1 = matrix1.NumCols();
    size_t rows2 = matrix2.NumRows();
    size_t cols2 = matrix2.NumCols();

    if (rows1 == rows2) {
      if (cols1 == cols2) {
        return MatrixOperate<Operation>(matrix1, matrix2);
      } else if (cols1 == 1) {
        // matrix1 is colVector
        return ColVectorMatrixOperate<Operation>(matrix1, matrix2);
      } else if (cols2 == 1) {
        // matrix2 is colVector
        return MatrixColVectorOperate<Operation>(matrix1, matrix2);
      }
    } else if (cols1 == cols2) {
      if (rows1 == 1) {
        // matrix1 is a vector
        return VectorMatrixOperate<Operation>(matrix1, matrix2);
      } else if(rows2 == 1) {
        // matrix2 is a vector
        return MatrixVectorOperate<Operation>(matrix1, matrix2);
      }
    } else if (rows1 == 1 && cols1 == 1) {
      // m1 is scalar
      return Operate<Operation>(matrix1(0, 0), matrix2);
    } else if (rows2 == 1 && cols2 == 1) {
      // m2 is scalar
      return Operate<Operation>(matrix1, matrix2(0, 0));
    }
    
    std::ostringstream os;
    os << "Cannot operate on matrices of different dimensions: " << rows1 << 'x' << cols1
        << " and " << rows2 << 'x' << cols2;
    throw std::invalid_argument(os.str());
  }


  template <typename Operation>
  Matrix MatrixImpl::UnaryOperate(const Matrix& matrix) {
    Matrix result(matrix.NumRows(), matrix.NumCols());

    Partition(matrix.NumCells(), 
        [&matrix, &result](size_t from, size_t to) {
      float_t * mData = matrix.data.get();
      float_t * outData = result.data.get();

      return [from, to, mData, outData]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(mData[i], outData[i]);
        }
        return 0;
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::RowReduce(const Matrix& matrix, float_t initialValue) {
    Matrix result(
        matrix.NumRows(), 
        1, 
        [initialValue] (size_t row, size_t col) { return initialValue; });

    Partition(matrix.NumRows(), [&matrix, &result](size_t from, size_t to) {
      return [&matrix, &result, from, to]() { 
        float_t * outData = result.data.get();
        for (size_t row = from; row < to; row++) {
          float_t * mData = matrix.data.get() + row * matrix.NumCols();
          for (size_t col = 0; col < matrix.NumCols(); col++) {
            Operation::operate(outData[row], mData[col]);
          }
        }
        return 0; 
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::ColReduce(const Matrix& matrix, float_t initialValue) {
    Matrix result(1, 
        matrix.NumCols(), 
        [initialValue] (size_t row, size_t col) { return initialValue; });

    Partition(matrix.NumCols(), [&matrix, &result](size_t from, size_t to) {
      return [&matrix, &result, from, to]() { 
        float_t * outData = result.data.get();
        for (size_t row = 0; row < matrix.NumRows(); row++) {
          float_t * mData = matrix.data.get() + row * matrix.NumCols();
          for (size_t col = from; col < to; col++) {
            Operation::operate(outData[col], mData[col]);
          }
        }
        return 0; 
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::Reduce(const Matrix& matrix, float_t initialValue) {
    return matrix.NumRows() > 1
        ? ColReduce<Operation>(matrix, initialValue)
        : RowReduce<Operation>(matrix, initialValue);
  }


  template <typename Operation>
  Matrix MatrixImpl::MatrixOperate(
      const Matrix& matrix1, 
      const Matrix& matrix2) {
    // assumes matrices have the same sizes
    Matrix result(matrix1.NumRows(), matrix1.NumCols());

    Partition(matrix1.NumCells(), 
        [&matrix1, &matrix2, &result](size_t from, size_t to) {
      float_t * mData1 = matrix1.data.get();
      float_t * mData2 = matrix2.data.get();
      float_t * outData = result.data.get();

      return [from, to, mData1, mData2, outData]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(mData1[i], mData2[i], outData[i]);
        }
        return 0;
      };
    });

    return result;
  }
  

  template <typename Operation>
  Matrix MatrixImpl::VectorMatrixOperate(
      const Matrix& vector, 
      const Matrix& matrix) {
    Matrix result(matrix.NumRows(), matrix.NumCols());

    Partition(matrix.NumCells(), 
        [&vector, &matrix, &result](size_t from, size_t to) {
      float_t * vData = vector.data.get();
      float_t * mData = matrix.data.get();
      float_t * outData = result.data.get();
      size_t cols = matrix.NumCols();

      return [from, to, vData, mData, outData, cols]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(vData[i % cols], mData[i], outData[i]);
        }
        return 0;
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::MatrixVectorOperate(
      const Matrix& matrix, 
      const Matrix& vector) {
    Matrix result(matrix.NumRows(), matrix.NumCols());

    Partition(matrix.NumCells(), 
        [&vector, &matrix, &result](size_t from, size_t to) {
      float_t * vData = vector.data.get();
      float_t * mData = matrix.data.get();
      float_t * outData = result.data.get();
      size_t cols = matrix.NumCols();

      return [from, to, vData, mData, outData, cols]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(mData[i], vData[i % cols], outData[i]);
        }
        return 0;
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::ColVectorMatrixOperate(
      const Matrix& vector, 
      const Matrix& matrix) {
    Matrix result(matrix.NumRows(), matrix.NumCols());

    Partition(matrix.NumCells(), 
        [&vector, &matrix, &result](size_t from, size_t to) {
      float_t * vData = vector.data.get();
      float_t * mData = matrix.data.get();
      float_t * outData = result.data.get();
      size_t cols = matrix.NumCols();

      return [from, to, vData, mData, outData, cols]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(vData[i / cols], mData[i], outData[i]);
        }
        return 0;
      };
    });

    return result;
  }


  template <typename Operation>
  Matrix MatrixImpl::MatrixColVectorOperate(
      const Matrix& matrix, 
      const Matrix& vector) {
    Matrix result(matrix.NumRows(), matrix.NumCols());

    Partition(matrix.NumCells(), 
        [&vector, &matrix, &result](size_t from, size_t to) {
      float_t * vData = vector.data.get();
      float_t * mData = matrix.data.get();
      float_t * outData = result.data.get();
      size_t cols = matrix.NumCols();

      return [from, to, vData, mData, outData, cols]() { 
        for (size_t i = from; i < to; i++) {
          Operation::operate(mData[i], vData[i / cols], outData[i]);
        }
        return 0;
      };
    });

    return result;
  }
} // namespace multithread
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_MULTI_THREAD_MATRIX_IMPL