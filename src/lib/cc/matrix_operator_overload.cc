#include "../h/matrix_operator_overload.h"

#include "../h/singlethread/matrix_impl.h"
#include "../h/multithread/matrix_impl.h"

namespace mdl {
namespace math {

  Matrix operator*(const Matrix& matrix1, const Matrix& matrix2) {
    return multithread::MatrixImpl::Multiply(matrix1, matrix2);
  }

} // math
} // mdl