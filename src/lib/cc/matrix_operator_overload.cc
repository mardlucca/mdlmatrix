#include "../h/matrix_operator_overload.h"

#include <sstream>

#include <mdl/profiler.h>

namespace mdl {
namespace math {

  Matrix operator*(const Matrix& matrix1, const Matrix& matrix2) {
    auto g1 = profiler::probe("Matrix Multiply");

    if (matrix1.cols != matrix2.rows) {
      std::ostringstream os;
      os << "Cannot multiply matrices of incompatible dimensions: " 
          << matrix1.rows << 'x' << matrix1.cols
          << " and " << matrix2.rows << 'x' << matrix2.cols;
      throw std::invalid_argument(os.str());
    }
    
    size_t rows = matrix1.rows;
    size_t cols = matrix2.cols;
    float_t * data = new float_t[rows * cols];
    float_t * runner = data;
    float_t * data1 = matrix1.data.get();
    float_t * data2 = matrix2.data.get();
    size_t cols1 = matrix1.cols;
    size_t cols2 = matrix2.cols;
    
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        float_t sum = 0.0;
        float_t * data2Runner = data2 + col;
        for (size_t k = 0; k < cols1; k++) {
          sum += data1[k] * *data2Runner;
          data2Runner += cols2;
        }
        *runner++ = sum;
      }
      data1 += cols1;
    }

    return Matrix(rows, cols, data);
  }

} // math
} // mdl