#include <gtest/gtest.h>

#include <iostream>

#include <mdl/matrix.h>
#include "../../lib/h/singlethread/matrix_impl.h"
#include "../../lib/h/singlethread/matrix_reflexive_impl.h"
#include "../../lib/h/multithread/matrix_impl.h"
#include "../../lib/h/multithread/matrix_reflexive_impl.h"


namespace mdl {
namespace math {
  using multithread::MatrixImpl;
  using multithread::MatrixReflexiveImpl;
  using std::cout;
  using std::endl;

  class MatrixImplTestSuite : public ::testing::Test {
    protected:
      Matrix matrix;
      Matrix vector;
      Matrix colVector;

      MatrixImplTestSuite() 
          : matrix(Matrix(4,3)), vector(Matrix(1,3)), colVector(Matrix(4,1)) {
        size_t count = 0;
        for (size_t row = 0; row < matrix.NumRows(); row++) {
          for (size_t col = 0; col < matrix.NumCols(); col++) {
            matrix(row, col) = ++count;
          }
        }

        vector(RightRange(0), RightRange(0)) = 
          matrix(UnitRange(1), LeftRange(matrix.NumCols()));
        colVector(RightRange(0), RightRange(0)) = 
          matrix(LeftRange(matrix.NumRows()), UnitRange(1));
      }
  };

  TEST_F(MatrixImplTestSuite, MatrixScalarOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix, 20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, SlicedMatrixScalarOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix({0, matrix.NumRows(), 2}, {}), 20);
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < result.NumRows(); row++) {
      for (size_t col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(matrix(row * 2, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, TransposedMatrixScalarOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix().Transpose(), 20);
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(col, row));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ScalarMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(21, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 21, result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ScalarSlicedMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(21, matrix({0, matrix.NumRows(), 2}, {}));
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < result.NumRows(); row++) {
      for (size_t col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(matrix(row * 2, col) + 21, result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ScalarTransposedMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(21, matrix().Transpose());
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 21, result(col, row));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixScalarReflexiveOperateTest) {
    Matrix result = matrix;
    MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, 20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixUnaryOperateTest) {
    Matrix result = MatrixImpl::UnaryOperate<op::Negate>(matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, SlicedMatrixUnaryOperateTest) {
    Matrix result = MatrixImpl::UnaryOperate<op::Negate>(matrix({0, matrix.NumRows(), 2}, {}));
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < result.NumRows(); row++) {
      for (size_t col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(-matrix(row * 2, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, TransposedMatrixUnaryOperateTest) {
    Matrix result = MatrixImpl::UnaryOperate<op::Negate>(matrix().Transpose());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(col, row));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ReflexiveUnaryOperateTest) {
    Matrix result = matrix;
    // result.ReflexiveUnaryOperate<op::Negate>();
    MatrixReflexiveImpl::ReflexiveUnaryOperate<op::Negate>(&result);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(2 * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixVectorOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix, vector);
    cout << result << endl; 

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, VectorMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(vector, matrix);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ColVectorMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(colVector, matrix);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixColVectorOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix, colVector);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixScalarMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix, matrix(UnitRange(0), UnitRange(0)));
    cout << result << endl; 

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + matrix(0,0), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ScalarMatrixMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix(UnitRange(0), UnitRange(0)), matrix);
    cout << result << endl; 

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + matrix(0,0), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, VectorVectorOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(vector, matrix(UnitRange(1), RightRange(0)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, VectorVectorOperateTest2) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix(UnitRange(1), RightRange(0)), vector);

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, VectorScalarMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(vector, matrix(UnitRange(1), UnitRange(0)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, 0) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ScalarMatrixVectorOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix(UnitRange(1), UnitRange(0)), vector);

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, 0) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ColVectorColVectorOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(colVector, matrix(RightRange(0), UnitRange(1)));

    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (size_t row = 0; row < colVector.NumRows(); row++) {
      for (size_t col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(matrix(row, 1) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ColVectorColVectorOperateTest2) {
    Matrix result = MatrixImpl::Operate<op::Addition>(matrix(RightRange(0), UnitRange(1)), colVector);

    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (size_t row = 0; row < colVector.NumRows(); row++) {
      for (size_t col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(matrix(row, 1) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ScalarMatrixScalarMatrixOperateTest) {
    Matrix result = MatrixImpl::Operate<op::Addition>(
        matrix(UnitRange(1), UnitRange(0)), 
        matrix(UnitRange(0), UnitRange(1)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(matrix(1,0) + matrix(0,1), result(0,0));
  }

  TEST_F(MatrixImplTestSuite, MatrixOperateInvalidSizesTest1) {
    Matrix m2 = matrix(LeftRange(2), RightRange(0));
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(matrix, m2), std::invalid_argument);
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(m2, matrix), std::invalid_argument);
  }

  TEST_F(MatrixImplTestSuite, MatrixOperateInvalidSizesTest2) {
    Matrix m2 = matrix(RightRange(0), LeftRange(2));
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(matrix, m2), std::invalid_argument);
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(m2, matrix), std::invalid_argument);
  }

  TEST_F(MatrixImplTestSuite, VectorOperateInvalidSizesTest1) {
    Matrix v2 = vector(UnitRange(0), LeftRange(2));
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(vector, v2), std::invalid_argument);
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(v2, vector), std::invalid_argument);
  }

  TEST_F(MatrixImplTestSuite, VectorOperateInvalidSizesTest2) {
    Matrix v2 = colVector(LeftRange(2), UnitRange(0));
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(colVector, v2), std::invalid_argument);
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(v2, colVector), std::invalid_argument);
  }

  TEST_F(MatrixImplTestSuite, VectorOperateInvalidSizesTest3) {
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(colVector, vector), std::invalid_argument);
    ASSERT_THROW(MatrixImpl::Operate<op::Addition>(vector, colVector), std::invalid_argument);
  }

  TEST_F(MatrixImplTestSuite, MatrixMatrixReflexiveOperateTest) {
    Matrix result = matrix;
    MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(2 * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixVectorReflexiveOperateTest) {
    Matrix result = matrix;
    MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, vector);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - vector(0, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixColVectorReflexiveOperateTest) {
    Matrix result = matrix;
    MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, colVector);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - colVector(row, 0));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, MatrixScalarMatrixReflexiveOperateTest) {
    Matrix result = matrix;
    MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, matrix(UnitRange(1), UnitRange(1)));
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - matrix(1, 1));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, VectorVectorReflexiveOperateTest) {
    Matrix original = vector;
    MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&vector, vector);

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(2 * original(row, col), vector(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ColVectorColVectorReflexiveOperateTest) {
    Matrix original = colVector;
    MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&colVector, colVector);

    for (size_t row = 0; row < colVector.NumRows(); row++) {
      for (size_t col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(2 * original(row, col), colVector(row, col));
      }
    }
  }

  TEST_F(MatrixImplTestSuite, ReflexiveOperateInvalidSizesTest1) {
    ASSERT_THROW(MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&vector, matrix), std::invalid_argument);
    ASSERT_THROW(MatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&colVector, matrix), std::invalid_argument);
  }

  TEST_F(MatrixImplTestSuite, RowReduceTest_Matrix) {
    Matrix result = MatrixImpl::RowReduce<op::Addition>(matrix);
    ASSERT_EQ(4, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(6, result(0,0));
    ASSERT_EQ(15, result(1,0));
    ASSERT_EQ(24, result(2,0));
    ASSERT_EQ(33, result(3,0));
  }

  TEST_F(MatrixImplTestSuite, RowReduceTest_Vector) {
    Matrix result = MatrixImpl::RowReduce<op::Addition>(vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(15, result(0,0));
  }

  TEST_F(MatrixImplTestSuite, RowReduceTest_ColVector) {
    Matrix result = MatrixImpl::RowReduce<op::Addition>(colVector);
    ASSERT_EQ(4, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(2, result(0,0));
    ASSERT_EQ(5, result(1,0));
    ASSERT_EQ(8, result(2,0));
    ASSERT_EQ(11, result(3,0));
  }

  TEST_F(MatrixImplTestSuite, ColReduceTest_Matrix) {
    Matrix result = MatrixImpl::ColReduce<op::Addition>(matrix);
    cout << result << endl;
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(3, result.NumCols());

    ASSERT_EQ(22, result(0,0));
    ASSERT_EQ(26, result(0,1));
    ASSERT_EQ(30, result(0,2));
  }

  TEST_F(MatrixImplTestSuite, ColReduceTest_Vector) {
    Matrix result = MatrixImpl::ColReduce<op::Addition>(vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(3, result.NumCols());

    ASSERT_EQ(4, result(0,0));
    ASSERT_EQ(5, result(0,1));
    ASSERT_EQ(6, result(0,2));
  }

  TEST_F(MatrixImplTestSuite, ColReduceTest_ColVector) {
    Matrix result = MatrixImpl::ColReduce<op::Addition>(colVector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(26, result(0,0));
  }

} // math
} // mdl
