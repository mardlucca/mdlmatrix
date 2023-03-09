#ifndef _MDL_MATH_BASESLICE_OPERATOR_OVERLOAD
#define _MDL_MATH_BASESLICE_OPERATOR_OVERLOAD

#include "typedefs.h"
#include "matrix.h"

namespace mdl {
namespace math {

  Matrix operator+(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator+(float_t scalar, const BaseMatrix& matrix);
  Matrix operator+(const BaseMatrix& matrix, float_t scalar);
  Matrix operator-(const BaseMatrix& matrix1, const BaseMatrix& matrix2) ;
  Matrix operator-(float_t scalar, const BaseMatrix& matrix);
  Matrix operator-(const BaseMatrix& matrix, float_t scalar);
  Matrix operator-(const BaseMatrix& matrix);

  Matrix operator*(float_t scalar, const BaseMatrix& matrix);
  Matrix operator*(const BaseMatrix& matrix, float_t scalar);
  Matrix operator/(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator/(float_t scalar, const BaseMatrix& matrix);
  Matrix operator/(const BaseMatrix& matrix, float_t scalar);
  Matrix operator%(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator%(float_t scalar, const BaseMatrix& matrix);
  Matrix operator%(const BaseMatrix& matrix, float_t scalar);
  Matrix operator^(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator^(float_t scalar, const BaseMatrix& matrix);
  Matrix operator^(const BaseMatrix& matrix, float_t scalar);
  Matrix operator<(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator<(float_t scalar, const BaseMatrix& matrix);
  Matrix operator<(const BaseMatrix& matrix, float_t scalar);
  Matrix operator<=(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator<=(float_t scalar, const BaseMatrix& matrix);
  Matrix operator<=(const BaseMatrix& matrix, float_t scalar);
  Matrix operator>(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator>(float_t scalar, const BaseMatrix& matrix);
  Matrix operator>(const BaseMatrix& matrix, float_t scalar);
  Matrix operator>=(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator>=(float_t scalar, const BaseMatrix& matrix);
  Matrix operator>=(const BaseMatrix& matrix, float_t scalar);
  Matrix operator==(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator==(float_t scalar, const BaseMatrix& matrix);
  Matrix operator==(const BaseMatrix& matrix, float_t scalar);
  Matrix operator!=(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator!=(float_t scalar, const BaseMatrix& matrix);
  Matrix operator!=(const BaseMatrix& matrix, float_t scalar);

  Matrix operator&&(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator||(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator!(const BaseMatrix& matrix);
  Matrix operator|(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  Matrix operator,(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
} // math
} // mdl

#endif // _MDL_MATH_BASESLICE_OPERATOR_OVERLOAD