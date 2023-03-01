#include "../h/basematrix_operator_overload.h"

#include <sstream>

#include "../h/matrix.h"
#include "../h/basematrix_operate.h"
#include "../h/operation.hpp"

namespace mdl {
namespace math {

  Matrix operator+(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Addition>(matrix1, matrix2);
  }

  Matrix operator+(double scalar, const BaseMatrix& matrix) {
    return Operate<op::Addition>(scalar, matrix);
  }

  Matrix operator+(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::Addition>(matrix, scalar);
  }

  Matrix operator-(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Subtraction>(matrix1, matrix2);
  }

  Matrix operator-(double scalar, const BaseMatrix& matrix) {
    return Operate<op::Subtraction>(scalar, matrix);
  }

  Matrix operator-(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::Subtraction>(matrix, scalar);
  }

  Matrix operator-(const BaseMatrix& matrix) {
    return UnaryOperate<op::Negate>(matrix);
  }

  Matrix operator*(double scalar, const BaseMatrix& matrix) {
    return Operate<op::Multiplication>(scalar, matrix);
  }

  Matrix operator*(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::Multiplication>(matrix, scalar);
  }

  Matrix operator/(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Division>(matrix1, matrix2);
  }

  Matrix operator/(double scalar, const BaseMatrix& matrix) {
    return Operate<op::Division>(scalar, matrix);
  }

  Matrix operator/(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::Division>(matrix, scalar);
  }

  Matrix operator%(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Mod>(matrix1, matrix2);
  }

  Matrix operator%(double scalar, const BaseMatrix& matrix) {
    return Operate<op::Mod>(scalar, matrix);
  }

  Matrix operator%(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::Mod>(matrix, scalar);
  }

  Matrix operator^(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Pow>(matrix1, matrix2);
  }

  Matrix operator^(double scalar, const BaseMatrix& matrix) {
    return Operate<op::Pow>(scalar, matrix);
  }

  Matrix operator^(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::Pow>(matrix, scalar);
  }

  Matrix operator<(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::LessThan>(matrix1, matrix2);
  }

  Matrix operator<(double scalar, const BaseMatrix& matrix) {
    return Operate<op::LessThan>(scalar, matrix);
  }

  Matrix operator<(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::LessThan>(matrix, scalar);
  }

  Matrix operator<=(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::LessThanEquals>(matrix1, matrix2);
  }

  Matrix operator<=(double scalar, const BaseMatrix& matrix) {
    return Operate<op::LessThanEquals>(scalar, matrix);
  }

  Matrix operator<=(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::LessThanEquals>(matrix, scalar);
  }

  Matrix operator>(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::GreaterThan>(matrix1, matrix2);
  }

  Matrix operator>(double scalar, const BaseMatrix& matrix) {
    return Operate<op::GreaterThan>(scalar, matrix);
  }

  Matrix operator>(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::GreaterThan>(matrix, scalar);
  }

  Matrix operator>=(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::GreaterThanEquals>(matrix1, matrix2);
  }

  Matrix operator>=(double scalar, const BaseMatrix& matrix) {
    return Operate<op::GreaterThanEquals>(scalar, matrix);
  }

  Matrix operator>=(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::GreaterThanEquals>(matrix, scalar);
  }

  Matrix operator==(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Equals>(matrix1, matrix2);
  }

  Matrix operator==(double scalar, const BaseMatrix& matrix) {
    return Operate<op::Equals>(scalar, matrix);
  }

  Matrix operator==(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::Equals>(matrix, scalar);
  }  

  Matrix operator!=(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::NotEquals>(matrix1, matrix2);
  }

  Matrix operator!=(double scalar, const BaseMatrix& matrix) {
    return Operate<op::NotEquals>(scalar, matrix);
  }

  Matrix operator!=(const BaseMatrix& matrix,
      double scalar) {
    return Operate<op::NotEquals>(matrix, scalar);
  }

  Matrix operator!(const BaseMatrix& matrix) {
    return UnaryOperate<op::Not>(matrix);
  }

  Matrix operator&&(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::And>(matrix1, matrix2);
  }
  
  Matrix operator||(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Or>(matrix1, matrix2);
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