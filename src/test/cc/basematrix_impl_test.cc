#include <gtest/gtest.h>

#include <iostream>

#include <mdl/matrix.h>
#include "src/lib/h/singlethread/basematrix_impl.h"
#include "src/lib/h/singlethread/basematrix_reflexive_impl.h"

namespace mdl {
namespace math {
  using singlethread::BaseMatrixImpl;
  using singlethread::BaseMatrixReflexiveImpl;
  using std::cout;
  using std::endl;

  class BaseMatrixImplTestSuite : public ::testing::Test {
    protected:
      Matrix matrix;
      Matrix vector;
      Matrix colVector;

      BaseMatrixImplTestSuite() 
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

  TEST_F(BaseMatrixImplTestSuite, MatrixScalarOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix, 20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, SlicedMatrixScalarOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix({0, matrix.NumRows(), 2}, {}), 20);
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < result.NumRows(); row++) {
      for (size_t col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(matrix(row * 2, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, TransposedMatrixScalarOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix().Transpose(), 20);
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(col, row));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ScalarMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(21, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 21, result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ScalarSlicedMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(21, matrix({0, matrix.NumRows(), 2}, {}));
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < result.NumRows(); row++) {
      for (size_t col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(matrix(row * 2, col) + 21, result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ScalarTransposedMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(21, matrix().Transpose());
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 21, result(col, row));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixScalarReflexiveOperateTest) {
    Matrix result = matrix;
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, 20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, SlicedMatrixScalarReflexiveOperateTest) {
    Matrix result = matrix;
    Slice slice = result({0, matrix.NumRows(), 2}, {});
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&slice, 20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        if (row % 2 == 0) {
          ASSERT_EQ(matrix(row, col) + 20, result(row, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, TransposedMatrixScalarReflexiveOperateTest) {
    Matrix result = matrix;
    Slice slice = result().Transpose();
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&slice, 20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixUnaryOperateTest) {
    Matrix result = BaseMatrixImpl::UnaryOperate<op::Negate>(matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, SlicedMatrixUnaryOperateTest) {
    Matrix result = BaseMatrixImpl::UnaryOperate<op::Negate>(matrix({0, matrix.NumRows(), 2}, {}));
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < result.NumRows(); row++) {
      for (size_t col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(-matrix(row * 2, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, TransposedMatrixUnaryOperateTest) {
    Matrix result = BaseMatrixImpl::UnaryOperate<op::Negate>(matrix().Transpose());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(col, row));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ReflexiveUnaryOperateTest) {
    Matrix result = matrix;
    // result.ReflexiveUnaryOperate<op::Negate>();
    BaseMatrixReflexiveImpl::ReflexiveUnaryOperate<op::Negate>(&result);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ReflexiveUnaryOperateTest_Sliced) {
    Matrix result = matrix;
    Slice slice = result({0, matrix.NumRows(), 2}, {});
    BaseMatrixReflexiveImpl::ReflexiveUnaryOperate<op::Negate>(&slice);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        if (row % 2 == 0) {
          ASSERT_EQ(-matrix(row, col), result(row, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ReflexiveUnaryOperateTest_Transposed) {
    Matrix result = matrix;
    Slice transposed = result().Transpose();
    BaseMatrixReflexiveImpl::ReflexiveUnaryOperate<op::Negate>(&transposed);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(2 * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixVectorOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix, vector);
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

  TEST_F(BaseMatrixImplTestSuite, VectorMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(vector, matrix);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ColVectorMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(colVector, matrix);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixColVectorOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix, colVector);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixScalarMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix, matrix(UnitRange(0), UnitRange(0)));
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

  TEST_F(BaseMatrixImplTestSuite, ScalarMatrixMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix(UnitRange(0), UnitRange(0)), matrix);
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

  TEST_F(BaseMatrixImplTestSuite, VectorVectorOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(vector, matrix(UnitRange(1), RightRange(0)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, VectorVectorOperateTest2) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix(UnitRange(1), RightRange(0)), vector);

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, VectorScalarMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(vector, matrix(UnitRange(1), UnitRange(0)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, 0) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ScalarMatrixVectorOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix(UnitRange(1), UnitRange(0)), vector);

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, 0) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ColVectorColVectorOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(colVector, matrix(RightRange(0), UnitRange(1)));

    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (size_t row = 0; row < colVector.NumRows(); row++) {
      for (size_t col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(matrix(row, 1) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ColVectorColVectorOperateTest2) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(matrix(RightRange(0), UnitRange(1)), colVector);

    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (size_t row = 0; row < colVector.NumRows(); row++) {
      for (size_t col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(matrix(row, 1) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ScalarMatrixScalarMatrixOperateTest) {
    Matrix result = BaseMatrixImpl::Operate<op::Addition>(
        matrix(UnitRange(1), UnitRange(0)), 
        matrix(UnitRange(0), UnitRange(1)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(matrix(1,0) + matrix(0,1), result(0,0));
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixOperateInvalidSizesTest1) {
    Matrix m2 = matrix(LeftRange(2), RightRange(0));
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(matrix, m2), std::invalid_argument);
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(m2, matrix), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixOperateInvalidSizesTest2) {
    Matrix m2 = matrix(RightRange(0), LeftRange(2));
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(matrix, m2), std::invalid_argument);
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(m2, matrix), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, VectorOperateInvalidSizesTest1) {
    Matrix v2 = vector(UnitRange(0), LeftRange(2));
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(vector, v2), std::invalid_argument);
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(v2, vector), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, VectorOperateInvalidSizesTest2) {
    Matrix v2 = colVector(LeftRange(2), UnitRange(0));
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(colVector, v2), std::invalid_argument);
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(v2, colVector), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, VectorOperateInvalidSizesTest3) {
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(colVector, vector), std::invalid_argument);
    ASSERT_THROW(BaseMatrixImpl::Operate<op::Addition>(vector, colVector), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixMatrixReflexiveOperateTest) {
    Matrix result = matrix;
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(2 * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixVectorReflexiveOperateTest) {
    Matrix result = matrix;
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, vector);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - vector(0, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixColVectorReflexiveOperateTest) {
    Matrix result = matrix;
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, colVector);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - colVector(row, 0));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, MatrixScalarMatrixReflexiveOperateTest) {
    Matrix result = matrix;
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, matrix(UnitRange(1), UnitRange(1)));
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - matrix(1, 1));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, SliceReflexiveOperateTest1) {
    Matrix original = matrix;
    Slice result = matrix(UnitRange(0), RightRange(0));
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(original(row, col), matrix(row, col) - vector(0, col));
        } else {
          ASSERT_EQ(original(row, col), matrix(row, col));
        }
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, SliceReflexiveOperateTest2) {
    Matrix original = matrix;
    Slice result = matrix(RightRange(0), UnitRange(0));
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&result, colVector);
    ASSERT_EQ(original.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        if (col == 0) {
          ASSERT_EQ(original(row, col), matrix(row, col) - colVector(row, 0));
        } else {
          ASSERT_EQ(original(row, col), matrix(row, col));
        }
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, VectorVectorReflexiveOperateTest) {
    Matrix original = vector;
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&vector, vector);

    for (size_t row = 0; row < vector.NumRows(); row++) {
      for (size_t col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(2 * original(row, col), vector(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ColVectorColVectorReflexiveOperateTest) {
    Matrix original = colVector;
    BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&colVector, colVector);

    for (size_t row = 0; row < colVector.NumRows(); row++) {
      for (size_t col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(2 * original(row, col), colVector(row, col));
      }
    }
  }

  TEST_F(BaseMatrixImplTestSuite, ReflexiveOperateInvalidSizesTest1) {
    ASSERT_THROW(BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&vector, matrix), std::invalid_argument);
    ASSERT_THROW(BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&colVector, matrix), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, ReflexiveOperateInvalidSizesTest2) {
    Slice slice = matrix(LeftRange(2), LeftRange(2));
    ASSERT_THROW(BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&slice, matrix), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, ReflexiveOperateInvalidSizesTest3) {
    Slice slice = matrix(RightRange(0), LeftRange(2));
    ASSERT_THROW(BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&slice, matrix), std::invalid_argument);
  }

  TEST_F(BaseMatrixImplTestSuite, ReflexiveOperateInvalidSizesTest4) {
    Slice slice = matrix(LeftRange(2), RightRange(0));
    ASSERT_THROW(BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(&slice, matrix), std::invalid_argument);
  }


  TEST_F(BaseMatrixImplTestSuite, RowReduceTest_Matrix) {
    Matrix result = BaseMatrixImpl::RowReduce<op::Addition>(matrix);
    ASSERT_EQ(4, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(6, result(0,0));
    ASSERT_EQ(15, result(1,0));
    ASSERT_EQ(24, result(2,0));
    ASSERT_EQ(33, result(3,0));
  }

  TEST_F(BaseMatrixImplTestSuite, RowReduceTest_Vector) {
    Matrix result = BaseMatrixImpl::RowReduce<op::Addition>(vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(15, result(0,0));
  }

  TEST_F(BaseMatrixImplTestSuite, RowReduceTest_ColVector) {
    Matrix result = BaseMatrixImpl::RowReduce<op::Addition>(colVector);
    ASSERT_EQ(4, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(2, result(0,0));
    ASSERT_EQ(5, result(1,0));
    ASSERT_EQ(8, result(2,0));
    ASSERT_EQ(11, result(3,0));
  }

  TEST_F(BaseMatrixImplTestSuite, ColReduceTest_Matrix) {
    Matrix result = BaseMatrixImpl::ColReduce<op::Addition>(matrix);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(3, result.NumCols());

    ASSERT_EQ(22, result(0,0));
    ASSERT_EQ(26, result(0,1));
    ASSERT_EQ(30, result(0,2));
  }

  TEST_F(BaseMatrixImplTestSuite, ColReduceTest_Vector) {
    Matrix result = BaseMatrixImpl::ColReduce<op::Addition>(vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(3, result.NumCols());

    ASSERT_EQ(4, result(0,0));
    ASSERT_EQ(5, result(0,1));
    ASSERT_EQ(6, result(0,2));
  }

  TEST_F(BaseMatrixImplTestSuite, ColReduceTest_ColVector) {
    Matrix result = BaseMatrixImpl::ColReduce<op::Addition>(colVector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(26, result(0,0));
  }

  TEST_F(BaseMatrixImplTestSuite, ReduceTest) {
    Matrix result = BaseMatrixImpl::Reduce<op::Addition>(matrix);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    ASSERT_EQ(22, result(0,0));
    ASSERT_EQ(26, result(0,1));
    ASSERT_EQ(30, result(0,2));

    result = BaseMatrixImpl::Reduce<op::Addition>(result);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());
    ASSERT_EQ(78, result(0,0));

    result = BaseMatrixImpl::Reduce<op::Addition>(result);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());
    ASSERT_EQ(78, result(0,0));
  }

} // math
} // mdl
