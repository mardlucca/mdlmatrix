#include "../h/matrix_operator_overload.h"

#include <sstream>

namespace mdl {
namespace math {

  // Matrix operator*(const Matrix& matrix1, const Matrix& matrix2) {
  //   if (matrix1.cols != matrix2.rows) {
  //     std::ostringstream os;
  //     os << "Cannot multiply matrices of incompatible dimensions: " 
  //         << matrix1.rows << 'x' << matrix1.cols
  //         << " and " << matrix2.rows << 'x' << matrix2.cols;
  //     throw std::invalid_argument(os.str());
  //   }
    
  //   int rows = matrix1.rows;
  //   int cols = matrix2.cols;
  //   double * data = new double[rows * cols];
  //   double * data1 = matrix1.data.get();
  //   double * data2 = matrix2.data.get();
  //   int cols1 = matrix1.cols;
  //   int cols2 = matrix2.cols;
    
  //   for (int row = 0; row < rows; row++) {
  //     for (int col = 0; col < cols; col++) {
  //       double sum = 0.0;
  //       for (int k = 0; k < cols1; k++) {
  //         sum += data1[row * cols1 + k] * data2[k * cols2 + col];
  //       }
  //       data[row * cols + col] = sum;
  //     }
  //   }

  //   return Matrix(rows, cols, data);
  // }


  Matrix operator*(const Matrix& matrix1, const Matrix& matrix2) {
    if (matrix1.cols != matrix2.rows) {
      std::ostringstream os;
      os << "Cannot multiply matrices of incompatible dimensions: " 
          << matrix1.rows << 'x' << matrix1.cols
          << " and " << matrix2.rows << 'x' << matrix2.cols;
      throw std::invalid_argument(os.str());
    }
    
    int rows = matrix1.rows;
    int cols = matrix2.cols;
    double * data = new double[rows * cols];
    double * runner = data;
    double * data1 = matrix1.data.get();
    double * data2 = matrix2.data.get();
    int cols1 = matrix1.cols;
    int cols2 = matrix2.cols;
    
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        double sum = 0.0;
        double * data2Runner = data2 + col;
        for (int k = 0; k < cols1; k++) {
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