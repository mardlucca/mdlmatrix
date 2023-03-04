#ifndef _MDL_MATH_MATRIX_FUNCTIONS
#define _MDL_MATH_MATRIX_FUNCTIONS

#include "float_t.h"
#include "matrix.h"
#include "slice.h"
#include "basematrix_operator_overload.h"
#include "matrix_operator_overload.h"

#include <vector>

namespace mdl {
namespace math {

  Matrix Abs(const BaseMatrix& matrix);
  Matrix Ceil(const BaseMatrix& matrix);
  Matrix Floor(const BaseMatrix& matrix);
  Matrix Round(const BaseMatrix& matrix);

  Matrix Exp(const BaseMatrix& matrix);
  Matrix Log(const BaseMatrix& matrix);
  Matrix Log2(const BaseMatrix& matrix);
  Matrix Log10(const BaseMatrix& matrix);

  Matrix Sin(const BaseMatrix& matrix);
  Matrix Cos(const BaseMatrix& matrix);
  Matrix Tan(const BaseMatrix& matrix);
  Matrix Asin(const BaseMatrix& matrix);
  Matrix Acos(const BaseMatrix& matrix);
  Matrix Atan(const BaseMatrix& matrix);

  Matrix Sqr(const BaseMatrix& matrix);
  Matrix Sqrt(const BaseMatrix& matrix);

  Matrix Sum(const BaseMatrix& matrix);
  Matrix Mean(const BaseMatrix& matrix);
  Matrix Min(const BaseMatrix& matrix);
  Matrix Max(const BaseMatrix& matrix);
  Matrix Variance(const BaseMatrix& matrix);
  Matrix StdDev(const BaseMatrix& matrix);

  Matrix Prod(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  float_t DotProd(const BaseMatrix& matrix1, const BaseMatrix& matrix2);

  Matrix Sigmoid(const BaseMatrix& matrix);
  Matrix SigmoidGradient(const Matrix& matrix);
  Matrix ReLU(const Matrix& matrix);
  Matrix ReLUGradient(const Matrix& matrix);
  Matrix Identity(const Matrix& matrix);
  Matrix IdentityGradient(const Matrix& matrix);

  Matrix Pack(const std::vector<Matrix>& matrices);
  std::vector<Matrix> Unpack(
      const Matrix& matrix, 
      const std::vector<std::pair<int, int>>& sizes);

  std::vector<Matrix> FromMtx(const char* fileName);
  void SaveMtx(const char* fileName, const Matrix& matrix);
  void SaveMtx(const char* fileName, const std::vector<Matrix>& mats);

} // math
} // mdl

#endif //_MDL_MATH_MATRIX_FUNCTIONS