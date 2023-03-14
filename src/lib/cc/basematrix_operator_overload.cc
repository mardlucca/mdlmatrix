#include "../h/basematrix_operator_overload.h"

#include <sstream>

#include "../h/matrix.h"
#include "../h/singlethread/basematrix_impl.h"
#include "../h/operation.h"

namespace mdl {
namespace math {
  using singlethread::BaseMatrixImpl;

  Matrix operator+(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Addition>(matrix1, matrix2);
  }

  Matrix operator+(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::Addition>(scalar, matrix);
  }

  Matrix operator+(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::Addition>(matrix, scalar);
  }

  Matrix operator-(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Subtraction>(matrix1, matrix2);
  }

  Matrix operator-(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::Subtraction>(scalar, matrix);
  }

  Matrix operator-(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::Subtraction>(matrix, scalar);
  }

  Matrix operator-(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Negate>(matrix);
  }

  Matrix operator*(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::Multiplication>(scalar, matrix);
  }

  Matrix operator*(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::Multiplication>(matrix, scalar);
  }

  Matrix operator/(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Division>(matrix1, matrix2);
  }

  Matrix operator/(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::Division>(scalar, matrix);
  }

  Matrix operator/(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::Division>(matrix, scalar);
  }

  Matrix operator%(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Mod>(matrix1, matrix2);
  }

  Matrix operator%(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::Mod>(scalar, matrix);
  }

  Matrix operator%(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::Mod>(matrix, scalar);
  }

  Matrix operator^(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Pow>(matrix1, matrix2);
  }

  Matrix operator^(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::Pow>(scalar, matrix);
  }

  Matrix operator^(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::Pow>(matrix, scalar);
  }

  Matrix operator<(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::LessThan>(matrix1, matrix2);
  }

  Matrix operator<(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::LessThan>(scalar, matrix);
  }

  Matrix operator<(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::LessThan>(matrix, scalar);
  }

  Matrix operator<=(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::LessThanEquals>(matrix1, matrix2);
  }

  Matrix operator<=(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::LessThanEquals>(scalar, matrix);
  }

  Matrix operator<=(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::LessThanEquals>(matrix, scalar);
  }

  Matrix operator>(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::GreaterThan>(matrix1, matrix2);
  }

  Matrix operator>(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::GreaterThan>(scalar, matrix);
  }

  Matrix operator>(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::GreaterThan>(matrix, scalar);
  }

  Matrix operator>=(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::GreaterThanEquals>(matrix1, matrix2);
  }

  Matrix operator>=(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::GreaterThanEquals>(scalar, matrix);
  }

  Matrix operator>=(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::GreaterThanEquals>(matrix, scalar);
  }

  Matrix operator==(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Equals>(matrix1, matrix2);
  }

  Matrix operator==(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::Equals>(scalar, matrix);
  }

  Matrix operator==(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::Equals>(matrix, scalar);
  }  

  Matrix operator!=(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::NotEquals>(matrix1, matrix2);
  }

  Matrix operator!=(float_t scalar, const BaseMatrix& matrix) {
    return BaseMatrixImpl::Operate<op::NotEquals>(scalar, matrix);
  }

  Matrix operator!=(const BaseMatrix& matrix,
      float_t scalar) {
    return BaseMatrixImpl::Operate<op::NotEquals>(matrix, scalar);
  }

  Matrix operator!(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Not>(matrix);
  }

  Matrix operator&&(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::And>(matrix1, matrix2);
  }
  
  Matrix operator||(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Or>(matrix1, matrix2);
  }

  Matrix operator|(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    Matrix result(std::max(matrix1.NumRows(), matrix2.NumRows()),
        matrix1.NumCols() + matrix2.NumCols());

    result(LeftRange(matrix1.NumRows()), LeftRange(matrix1.NumCols())) = matrix1;
    result(LeftRange(matrix2.NumRows()), RightRange(matrix1.NumCols())) = matrix2;
    return result;
  }

  Matrix operator,(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    Matrix result(matrix1.NumRows() + matrix2.NumRows(),
        std::max(matrix1.NumCols(), matrix2.NumCols()));

    result(LeftRange(matrix1.NumRows()), LeftRange(matrix1.NumCols())) = matrix1;
    result(RightRange(matrix1.NumRows()), LeftRange(matrix2.NumCols())) = matrix2;
    return result;
  }

} // math
} // mdl