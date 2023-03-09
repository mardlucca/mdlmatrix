#ifndef _MDL_MATH_MATRIX_OPERATOR_OVERLOAD
#define _MDL_MATH_MATRIX_OPERATOR_OVERLOAD

#include "typedefs.h"
#include "matrix.h"

namespace mdl {
namespace math {

  Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);

} // math
} // mdl

#endif // _MDL_MATH_MATRIX_OPERATOR_OVERLOAD