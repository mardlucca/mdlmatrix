#include <gtest/gtest.h>

#include <iostream>

#include <mdl/matrix.h>
#include "src/lib/h/matrix_operate.h"

namespace mdl {
namespace math {

  using std::cout;
  using std::endl;

  class MatrixOperateTestSuite : public ::testing::Test {
    protected:
      Matrix matrix;
      Matrix vector;
      Matrix colVector;

      MatrixOperateTestSuite() 
          : matrix(Matrix(4,3)), vector(Matrix(1,3)), colVector(Matrix(4,1)) {
        int count = 0;
        for (int row = 0; row < matrix.NumRows(); row++) {
          for (int col = 0; col < matrix.NumCols(); col++) {
            matrix(row, col) = ++count;
          }
        }

        vector(RightRange(0), RightRange(0)) = 
          matrix(UnitRange(1), LeftRange(matrix.NumCols()));
        colVector(RightRange(0), RightRange(0)) = 
          matrix(LeftRange(matrix.NumRows()), UnitRange(1));
      }
  };

  TEST_F(MatrixOperateTestSuite, MatrixScalarOperateTest) {
    Matrix result = Operate<op::Addition>(matrix, 20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, SlicedMatrixScalarOperateTest) {
    Matrix result = Operate<op::Addition>(matrix({0, matrix.NumRows(), 2}, {}), 20);
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(matrix(row * 2, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, TransposedMatrixScalarOperateTest) {
    Matrix result = Operate<op::Addition>(matrix.Transpose(), 20);
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(col, row));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ScalarMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(21, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 21, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ScalarSlicedMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(21, matrix({0, matrix.NumRows(), 2}, {}));
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(matrix(row * 2, col) + 21, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ScalarTransposedMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(21, matrix.Transpose());
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 21, result(col, row));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixScalarReflexiveOperateTest) {
    Matrix result = matrix;
    result.ReflexiveOperate<op::Addition>(20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, SlicedMatrixScalarReflexiveOperateTest) {
    Matrix result = matrix;
    Slice slice = result({0, matrix.NumRows(), 2}, {});
    slice.ReflexiveOperate<op::Addition>(20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row % 2 == 0) {
          ASSERT_EQ(matrix(row, col) + 20, result(row, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, TransposedMatrixScalarReflexiveOperateTest) {
    Matrix result = matrix;
    Slice slice = result.Transpose();
    slice.ReflexiveOperate<op::Addition>(20);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + 20, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixUnaryOperateTest) {
    Matrix result = UnaryOperate<op::Negate>(matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, SlicedMatrixUnaryOperateTest) {
    Matrix result = UnaryOperate<op::Negate>(matrix({0, matrix.NumRows(), 2}, {}));
    ASSERT_EQ(matrix.NumRows() / 2, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(-matrix(row * 2, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, TransposedMatrixUnaryOperateTest) {
    Matrix result = UnaryOperate<op::Negate>(matrix.Transpose());
    ASSERT_EQ(matrix.NumRows(), result.NumCols());
    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(col, row));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ReflexiveUnaryOperateTest) {
    Matrix result = matrix;
    // result.ReflexiveUnaryOperate<op::Negate>();
    result.ReflexiveUnaryOperate<op::Negate>();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ReflexiveUnaryOperateTest_Sliced) {
    Matrix result = matrix;
    Slice slice = result({0, matrix.NumRows(), 2}, {});
    slice.ReflexiveUnaryOperate<op::Negate>();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row % 2 == 0) {
          ASSERT_EQ(-matrix(row, col), result(row, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ReflexiveUnaryOperateTest_Transposed) {
    Matrix result = matrix;
    Slice tranposed = result.Transpose();
    tranposed.ReflexiveUnaryOperate<op::Negate>();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(-matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(matrix, matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(2 * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixVectorOperateTest) {
    Matrix result = Operate<op::Addition>(matrix, vector);
    cout << result << endl; 

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, VectorMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(vector, matrix);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ColVectorMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(colVector, matrix);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixColVectorOperateTest) {
    Matrix result = Operate<op::Addition>(matrix, colVector);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixScalarMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(matrix, matrix(UnitRange(0), UnitRange(0)));
    cout << result << endl; 

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + matrix(0,0), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ScalarMatrixMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(matrix(UnitRange(0), UnitRange(0)), matrix);
    cout << result << endl; 

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    cout << result << endl; 

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) + matrix(0,0), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, VectorVectorOperateTest) {
    Matrix result = Operate<op::Addition>(vector, matrix(UnitRange(1), RightRange(0)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (int row = 0; row < vector.NumRows(); row++) {
      for (int col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, VectorVectorOperateTest2) {
    Matrix result = Operate<op::Addition>(matrix(UnitRange(1), RightRange(0)), vector);

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (int row = 0; row < vector.NumRows(); row++) {
      for (int col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, col) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, VectorScalarMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(vector, matrix(UnitRange(1), UnitRange(0)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (int row = 0; row < vector.NumRows(); row++) {
      for (int col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, 0) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ScalarMatrixVectorOperateTest) {
    Matrix result = Operate<op::Addition>(matrix(UnitRange(1), UnitRange(0)), vector);

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());

    for (int row = 0; row < vector.NumRows(); row++) {
      for (int col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(matrix(1, 0) + vector(0, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ColVectorColVectorOperateTest) {
    Matrix result = Operate<op::Addition>(colVector, matrix(RightRange(0), UnitRange(1)));

    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (int row = 0; row < colVector.NumRows(); row++) {
      for (int col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(matrix(row, 1) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ColVectorColVectorOperateTest2) {
    Matrix result = Operate<op::Addition>(matrix(RightRange(0), UnitRange(1)), colVector);

    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (int row = 0; row < colVector.NumRows(); row++) {
      for (int col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(matrix(row, 1) + colVector(row, 0), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ScalarMatrixScalarMatrixOperateTest) {
    Matrix result = Operate<op::Addition>(
        matrix(UnitRange(1), UnitRange(0)), 
        matrix(UnitRange(0), UnitRange(1)));

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(matrix(1,0) + matrix(0,1), result(0,0));
  }

  TEST_F(MatrixOperateTestSuite, MatrixOperateInvalidSizesTest1) {
    Matrix m2 = matrix(LeftRange(2), RightRange(0));
    ASSERT_THROW(Operate<op::Addition>(matrix, m2), std::invalid_argument);
    ASSERT_THROW(Operate<op::Addition>(m2, matrix), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, MatrixOperateInvalidSizesTest2) {
    Matrix m2 = matrix(RightRange(0), LeftRange(2));
    ASSERT_THROW(Operate<op::Addition>(matrix, m2), std::invalid_argument);
    ASSERT_THROW(Operate<op::Addition>(m2, matrix), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, VectorOperateInvalidSizesTest1) {
    Matrix v2 = vector(UnitRange(0), LeftRange(2));
    ASSERT_THROW(Operate<op::Addition>(vector, v2), std::invalid_argument);
    ASSERT_THROW(Operate<op::Addition>(v2, vector), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, VectorOperateInvalidSizesTest2) {
    Matrix v2 = colVector(LeftRange(2), UnitRange(0));
    ASSERT_THROW(Operate<op::Addition>(colVector, v2), std::invalid_argument);
    ASSERT_THROW(Operate<op::Addition>(v2, colVector), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, VectorOperateInvalidSizesTest3) {
    ASSERT_THROW(Operate<op::Addition>(colVector, vector), std::invalid_argument);
    ASSERT_THROW(Operate<op::Addition>(vector, colVector), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, MatrixMatrixReflexiveOperateTest) {
    Matrix result = matrix;
    result.ReflexiveOperate<op::Addition>(matrix);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(2 * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixVectorReflexiveOperateTest) {
    Matrix result = matrix;
    result.ReflexiveOperate<op::Addition>(vector);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - vector(0, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixColVectorReflexiveOperateTest) {
    Matrix result = matrix;
    result.ReflexiveOperate<op::Addition>(colVector);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - colVector(row, 0));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, MatrixScalarMatrixReflexiveOperateTest) {
    Matrix result = matrix;
    result.ReflexiveOperate<op::Addition>(matrix(UnitRange(1), UnitRange(1)));
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - matrix(1, 1));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, SliceReflexiveOperateTest1) {
    Matrix original = matrix;
    Slice result = matrix(UnitRange(0), RightRange(0));
    result.ReflexiveOperate<op::Addition>(vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(original(row, col), matrix(row, col) - vector(0, col));
        } else {
          ASSERT_EQ(original(row, col), matrix(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, SliceReflexiveOperateTest2) {
    Matrix original = matrix;
    Slice result = matrix(RightRange(0), UnitRange(0));
    result.ReflexiveOperate<op::Addition>(colVector);
    ASSERT_EQ(original.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (col == 0) {
          ASSERT_EQ(original(row, col), matrix(row, col) - colVector(row, 0));
        } else {
          ASSERT_EQ(original(row, col), matrix(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, VectorVectorReflexiveOperateTest) {
    Matrix original = vector;
    vector.ReflexiveOperate<op::Addition>(vector);

    for (int row = 0; row < vector.NumRows(); row++) {
      for (int col = 0; col < vector.NumCols(); col++) {
        ASSERT_EQ(2 * original(row, col), vector(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ColVectorColVectorReflexiveOperateTest) {
    Matrix original = colVector;
    colVector.ReflexiveOperate<op::Addition>(colVector);

    for (int row = 0; row < colVector.NumRows(); row++) {
      for (int col = 0; col < colVector.NumCols(); col++) {
        ASSERT_EQ(2 * original(row, col), colVector(row, col));
      }
    }
  }

  TEST_F(MatrixOperateTestSuite, ReflexiveOperateInvalidSizesTest1) {
    ASSERT_THROW(vector.ReflexiveOperate<op::Addition>(matrix), std::invalid_argument);
    ASSERT_THROW(colVector.ReflexiveOperate<op::Addition>(matrix), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, ReflexiveOperateInvalidSizesTest2) {
    Slice slice = matrix(LeftRange(2), LeftRange(2));
    ASSERT_THROW(slice.ReflexiveOperate<op::Addition>(matrix), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, ReflexiveOperateInvalidSizesTest3) {
    Slice slice = matrix(RightRange(0), LeftRange(2));
    ASSERT_THROW(slice.ReflexiveOperate<op::Addition>(matrix), std::invalid_argument);
  }

  TEST_F(MatrixOperateTestSuite, ReflexiveOperateInvalidSizesTest4) {
    Slice slice = matrix(LeftRange(2), RightRange(0));
    ASSERT_THROW(slice.ReflexiveOperate<op::Addition>(matrix), std::invalid_argument);
  }


  TEST_F(MatrixOperateTestSuite, RowReduceTest_Matrix) {
    Matrix result = RowReduce<op::Addition>(matrix);
    ASSERT_EQ(4, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(6, result(0,0));
    ASSERT_EQ(15, result(1,0));
    ASSERT_EQ(24, result(2,0));
    ASSERT_EQ(33, result(3,0));
  }

  TEST_F(MatrixOperateTestSuite, RowReduceTest_Vector) {
    Matrix result = RowReduce<op::Addition>(vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(15, result(0,0));
  }

  TEST_F(MatrixOperateTestSuite, RowReduceTest_ColVector) {
    Matrix result = RowReduce<op::Addition>(colVector);
    ASSERT_EQ(4, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(2, result(0,0));
    ASSERT_EQ(5, result(1,0));
    ASSERT_EQ(8, result(2,0));
    ASSERT_EQ(11, result(3,0));
  }

  TEST_F(MatrixOperateTestSuite, ColReduceTest_Matrix) {
    Matrix result = ColReduce<op::Addition>(matrix);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(3, result.NumCols());

    ASSERT_EQ(22, result(0,0));
    ASSERT_EQ(26, result(0,1));
    ASSERT_EQ(30, result(0,2));
  }

  TEST_F(MatrixOperateTestSuite, ColReduceTest_Vector) {
    Matrix result = ColReduce<op::Addition>(vector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(3, result.NumCols());

    ASSERT_EQ(4, result(0,0));
    ASSERT_EQ(5, result(0,1));
    ASSERT_EQ(6, result(0,2));
  }

  TEST_F(MatrixOperateTestSuite, ColReduceTest_ColVector) {
    Matrix result = ColReduce<op::Addition>(colVector);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(1, result.NumCols());

    ASSERT_EQ(26, result(0,0));
  }

} // math
} // mdl
