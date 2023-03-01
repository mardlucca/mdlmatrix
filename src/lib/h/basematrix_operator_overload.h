#ifndef _MDL_MATH_BASESLICE_OPERATOR_OVERLOAD
#define _MDL_MATH_BASESLICE_OPERATOR_OVERLOAD

#include "matrix.h"

namespace mdl {
namespace math {

  Matrix operator+(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator+(double scalar, const BaseMatrix& matrix);
  Matrix operator+(const BaseMatrix& matrix, double scalar);
  Matrix operator-(const BaseMatrix& matrix1, const BaseMatrix& matrix2) ;
  Matrix operator-(double scalar, const BaseMatrix& matrix);
  Matrix operator-(const BaseMatrix& matrix, double scalar);
  Matrix operator-(const BaseMatrix& matrix);

  Matrix operator*(double scalar, const BaseMatrix& matrix);
  Matrix operator*(const BaseMatrix& matrix, double scalar);
  Matrix operator/(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator/(double scalar, const BaseMatrix& matrix);
  Matrix operator/(const BaseMatrix& matrix, double scalar);
  Matrix operator%(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator%(double scalar, const BaseMatrix& matrix);
  Matrix operator%(const BaseMatrix& matrix, double scalar);
  Matrix operator^(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator^(double scalar, const BaseMatrix& matrix);
  Matrix operator^(const BaseMatrix& matrix, double scalar);
  Matrix operator<(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator<(double scalar, const BaseMatrix& matrix);
  Matrix operator<(const BaseMatrix& matrix, double scalar);
  Matrix operator<=(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator<=(double scalar, const BaseMatrix& matrix);
  Matrix operator<=(const BaseMatrix& matrix, double scalar);
  Matrix operator>(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator>(double scalar, const BaseMatrix& matrix);
  Matrix operator>(const BaseMatrix& matrix, double scalar);
  Matrix operator>=(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator>=(double scalar, const BaseMatrix& matrix);
  Matrix operator>=(const BaseMatrix& matrix, double scalar);
  Matrix operator==(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator==(double scalar, const BaseMatrix& matrix);
  Matrix operator==(const BaseMatrix& matrix, double scalar);
  Matrix operator!=(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator!=(double scalar, const BaseMatrix& matrix);
  Matrix operator!=(const BaseMatrix& matrix, double scalar);

  Matrix operator&&(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator||(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator!(const BaseMatrix& matrix);
  Matrix operator|(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator,(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
} // math
} // mdl

#endif // _MDL_MATH_BASESLICE_OPERATOR_OVERLOAD