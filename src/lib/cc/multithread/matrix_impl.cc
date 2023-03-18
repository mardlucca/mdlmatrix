#include "../../h/multithread/matrix_impl.h"
#include "../../h/multithread/helper.h"

namespace mdl {
namespace math {
namespace multithread {
  Matrix MatrixImpl::Multiply(const Matrix& matrix1, const Matrix& matrix2) {
    if (matrix1.cols != matrix2.rows) {
      std::ostringstream os;
      os << "Cannot multiply matrices of incompatible dimensions: " 
          << matrix1.rows << 'x' << matrix1.cols
          << " and " << matrix2.rows << 'x' << matrix2.cols;
      throw std::invalid_argument(os.str());
    }
    
    Matrix m2Transp = matrix2.Transpose();

    size_t rows = matrix1.rows;
    size_t cols = matrix2.cols;
    size_t cols1 = matrix1.cols;
    size_t numCells = rows * cols;
    float_t * m1Data = matrix1.data.get();
    float_t * m2Data = m2Transp.data.get();
    float_t * rData = new float_t[numCells];


    Partition(numCells, [m1Data, m2Data, rData, cols, cols1](size_t from, size_t to) {
      return [m1Data, m2Data, rData, from, to, cols, cols1]() { 
        size_t frow = from / cols;
        size_t fcol = from % cols;
        size_t trow = to / cols;
        size_t tcol = to % cols;

        for (size_t row = frow, col = fcol; 
            row != trow || col != tcol;) {
          float_t * runner1 = m1Data + row * cols1;
          float_t * runner2 = m2Data + col * cols1;
          
          float_t sum = 0.0;
          #pragma clang loop vectorize(enable)
          for (int i = 0; i < cols1; i++) {
            sum += *runner1 * *runner2;
            runner1++;
            runner2++;
          }
          rData[row * cols + col] = sum;
          col++;
          if (col >= cols) { col = 0; row++;}
        }
        return 0; 
      };
    }, 1);

    return Matrix(rows, cols, rData);
  }

} // namepsace multithread
} // namespace math
} // namespace mdl