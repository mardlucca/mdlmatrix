#include <gtest/gtest.h>

#include <iostream>

#include <mdl/matrix.h>

namespace mdl {
namespace math {

  using std::cout;
  using std::endl;

  class MatrixOperatorOverloadTestSuite : public ::testing::Test {
    protected:
      Matrix matrix;
      Matrix vector;
      Matrix colVector;
      Matrix identity;

      MatrixOperatorOverloadTestSuite() 
          : matrix(Matrix(4,3)), 
            vector(Matrix(1,3)), 
            colVector(Matrix(4,1)),
            identity(Matrix(2,2)) {
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

        identity(0,0) = 1;
        identity(1,1) = 1;
      }
  };

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixMatrixAdditionTest) {
    Matrix result = matrix + matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) * 2, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveMatrixMatrixAdditionTest) {
    Matrix result = matrix;
    result += matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) * 2, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveSliceMatrixAdditionTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)) += vector;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) - vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixAdditionTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)).Transpose() += vector.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) - vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixAdditionTest2) {
    Matrix result = matrix;
    result.Transpose() += matrix.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) * 2, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarAdditionTest) {
    Matrix result = matrix + 100;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - 100);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarMatrixAdditionTest) {
    Matrix result = 101 + matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - 101);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarAdditionTest_Reflexive) {
    Matrix result = matrix;
    result += 102;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - 102);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarAdditionTest_Reflexive_Transposed) {
    Matrix result = matrix;
    result.Transpose() += 102;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) - 102);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarAdditionTest_Reflexive_Sliced) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)) += 103;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) - 103);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarAdditionTest_Reflexive_Sliced_Tranposed) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)).Transpose() += 104;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) - 104);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }





  TEST_F(MatrixOperatorOverloadTestSuite, MatrixMatrixSubtractionTest) {
    Matrix result = matrix - matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(0, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveMatrixMatrixSubtractionTest) {
    Matrix result = matrix;
    result -= matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(0, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveSliceMatrixSubtractionTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)) -= vector;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) + vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixSubtractionTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)).Transpose() -= vector.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) + vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixSubtractionTest2) {
    Matrix result = matrix;
    result.Transpose() -= matrix.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(0, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixVectorSubtractionTest) {
    Matrix result = matrix - Matrices::Default(1, matrix.NumCols(), 98);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) + 98);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixVectorSubtractionTest2) {
    Matrix result = matrix - Matrices::Default(matrix.NumRows(), 1, 99);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) + 99);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarSubtractionTest) {
    Matrix result = matrix - 100;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) + 100);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarSubtractionTest2) {
    Matrix result = matrix - Matrices::Default(1, 1, 100);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) + 100);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarMatrixSubtractionTest) {
    Matrix result = 101 - matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), 101 - result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarMatrixSubtractionTest2) {
    Matrix result = Matrices::Default(1, 1, 101) - matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), 101 - result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarSubtractionTest_Reflexive) {
    Matrix result = matrix;
    result -= 102;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) + 102);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarSubtractionTest_Reflexive_Transposed) {
    Matrix result = matrix;
    result.Transpose() -= 102;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) + 102);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarSubtractionTest_Reflexive_Sliced) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)) -= 103;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) + 103);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarSubtractionTest_Reflexive_Sliced_Tranposed) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)).Transpose() -= 104;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) + 104);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixNegationTest) {
    Matrix result = -matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_TRUE(matrix(row, col) > 0);
        ASSERT_TRUE(result(row, col) < 0);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, SliceNegationTest) {
    matrix(RightRange(0), LeftRange(1)) = -(matrix(RightRange(0), LeftRange(1)));
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_TRUE(matrix(row, col) < 0 || col > 0);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixMatrixMultiplicationTest) {
    Matrix result = matrix.Transpose() * matrix;

    cout << result << endl;

    ASSERT_EQ(matrix.NumCols(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    ASSERT_EQ(166, result(0,0));
    ASSERT_EQ(188, result(0,1));
    ASSERT_EQ(210, result(0,2));

    ASSERT_EQ(188, result(1,0));
    ASSERT_EQ(214, result(1,1));
    ASSERT_EQ(240, result(1,2));

    ASSERT_EQ(210, result(2,0));
    ASSERT_EQ(240, result(2,1));
    ASSERT_EQ(270, result(2,2));
  }

  TEST_F(MatrixOperatorOverloadTestSuite, VectorMatrixMultiplicationTest) {
    Matrix result = vector * matrix.Transpose();

    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(4, result.NumCols());

    ASSERT_EQ(32, result(0,0));
    ASSERT_EQ(77, result(0,1));
    ASSERT_EQ(122, result(0,2));
    ASSERT_EQ(167, result(0,3));
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveMatrixMatrixMultiplicationTest) {
    Matrix result = matrix;
    result *= matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) / matrix(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveSliceMatrixMultiplicationTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)) *= vector;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) / vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixMultiplicationTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)).Transpose() *= vector.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) / vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixMultiplicationTest2) {
    Matrix result = matrix;
    result.Transpose() *= matrix.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) / matrix(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarMultiplicationTest) {
    Matrix result = matrix * 256;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) / 256);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarMatrixMultiplicationTest) {
    Matrix result = 256 * matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) / 256);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarMultiplicationTest_Reflexive) {
    Matrix result = matrix;
    result *= 256;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) / 256);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarMultiplicationTest_Reflexive_Transposed) {
    Matrix result = matrix;
    result.Transpose() *= 256;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) / 256);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarMultiplicationTest_Reflexive_Sliced) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)) *= 256;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) / 256);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarMultiplicationTest_Reflexive_Sliced_Tranposed) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)).Transpose() *= 256;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) / 256);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }







  TEST_F(MatrixOperatorOverloadTestSuite, MatrixMatrixDivisionTest) {
    Matrix result = matrix / matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(1, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveMatrixMatrixDivisionTest) {
    Matrix result = matrix;
    result /= matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(1, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveSliceMatrixDivisionTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)) /= vector;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) * vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixDivisionTest) {
    Matrix result = matrix;
    result(UnitRange(0), RightRange(0)).Transpose() /= vector.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row == 0) {
          ASSERT_EQ(matrix(row, col), result(row, col) * vector(0, col));
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ReflexiveTransposedMatrixDivisionTest2) {
    Matrix result = matrix;
    result.Transpose() /= matrix.Transpose();
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) * matrix(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarDivisionTest) {
    Matrix result = matrix / 2;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) * 2);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarMatrixDivisionTest) {
    Matrix result = 256 / matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), 256 / result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarDivisionTest_Reflexive) {
    Matrix result = matrix;
    result /= 2;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) * 2);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarDivisionTest_Reflexive_Transposed) {
    Matrix result = matrix;
    result.Transpose() /= 2;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), result(row, col) * 2);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarDivisionTest_Reflexive_Sliced) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)) /= 2;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) * 2);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarDivisionTest_Reflexive_Sliced_Tranposed) {
    Matrix result = matrix;
    result(LeftRange(2), RightRange(0)).Transpose() /= 2;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        if (row < 2) {
          ASSERT_EQ(matrix(row, col), result(row, col) * 2);
        } else {
          ASSERT_EQ(matrix(row, col), result(row, col));
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixScalarPowTest) {
    Matrix result = matrix ^ 2;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarMatrixPowTest) {
    Matrix result = 2 ^ (Matrices::Ones(matrix.NumRows(), matrix.NumCols()) * 2);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(4, result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixMatrixPowTest) {
    Matrix result = matrix ^ (Matrices::Ones(matrix.NumRows(), matrix.NumCols()) * 2);
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    
    for (int row = 0; row < matrix.NumRows(); row++) {
      for (int col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, SliceScalarPowTest) {
    Matrix result = matrix(RightRange(0), LeftRange(1)) ^ 2;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(1, result.NumCols());
    
    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col) * matrix(row, col), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarSlicePowTest) {
    Matrix result = 2 ^ matrix(LeftRange(1), RightRange(0));
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());
    
    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_EQ(std::pow(2, matrix(row, col)), result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixLessThanScalarTest) {
    Matrix result = matrix < 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) < 5) 
            || (result(row, col) == 0) && matrix(row, col) >= 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarLessThanMatrixTest) {
    Matrix result = 5 < matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) <= 5) 
            || (result(row, col) == 1) && matrix(row, col) > 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixLessThanMatrixTest) {
    Matrix result = matrix < Matrices::Zeros(matrix.NumRows(), matrix.NumCols()) + 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) < 5) 
            || (result(row, col) == 0) && matrix(row, col) >= 5);
      }
    }
  } 

  TEST_F(MatrixOperatorOverloadTestSuite, SliceLessThanScalarTest) {
    Matrix result = matrix(RightRange(0), RightRange(0)) < 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) < 5) 
            || (result(row, col) == 0) && matrix(row, col) >= 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarLessThanSliceTest) {
    Matrix result = 5 < matrix(RightRange(0), RightRange(0));
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) <= 5) 
            || (result(row, col) == 1) && matrix(row, col) > 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, SliceLessThanSliceTest) {
    Matrix result = matrix(RightRange(0), RightRange(0)) < 
        (Matrix(matrix.NumRows(), matrix.NumCols()) + 5)(RightRange(0), RightRange(0));
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) < 5) 
            || (result(row, col) == 0) && matrix(row, col) >= 5);
      }
    }
  } 

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixLessThanEqualsScalarTest) {
    Matrix result = matrix <= 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) <= 5) 
            || (result(row, col) == 0) && matrix(row, col) > 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarLessEqualsThanMatrixTest) {
    Matrix result = 5 <= matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) < 5) 
            || (result(row, col) == 1) && matrix(row, col) >= 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixLessThanEqualsMatrixTest) {
    Matrix result = matrix <= Matrices::Zeros(matrix.NumRows(), matrix.NumCols()) + 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) <= 5) 
            || (result(row, col) == 0) && matrix(row, col) > 5);
      }
    }
  } 

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixGreaterThanScalarTest) {
    Matrix result = matrix > 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) <= 5) 
            || (result(row, col) == 1) && matrix(row, col) > 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarGreaterThanMatrixTest) {
    Matrix result = 5 > matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) < 5) 
            || (result(row, col) == 0) && matrix(row, col) >= 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixLessGreaterMatrixTest) {
    Matrix result = matrix > Matrices::Zeros(matrix.NumRows(), matrix.NumCols()) + 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) <= 5) 
            || (result(row, col) == 1) && matrix(row, col) > 5);
      }
    }
  } 

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixGreaterThanEqualsScalarTest) {
    Matrix result = matrix >= 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) < 5) 
            || (result(row, col) == 1) && matrix(row, col) >= 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarGreaterThanEqualsMatrixTest) {
    Matrix result = 5 >= matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) <= 5) 
            || (result(row, col) == 0) && matrix(row, col) > 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixLessGreaterEqualsMatrixTest) {
    Matrix result = matrix >= Matrices::Zeros(matrix.NumRows(), matrix.NumCols()) + 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) < 5) 
            || (result(row, col) == 1) && matrix(row, col) >= 5);
      }
    }
  } 

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixEqualsScalarTest) {
    Matrix result = matrix == 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) != 5) 
            || (result(row, col) == 1) && matrix(row, col) == 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarEqualsMatrixTest) {
    Matrix result = 5 == matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) == 5) 
            || (result(row, col) == 0) && matrix(row, col) != 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixEqualsMatrixTest) {
    Matrix result = matrix == Matrices::Zeros(matrix.NumRows(), matrix.NumCols()) + 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) != 5) 
            || (result(row, col) == 1) && matrix(row, col) == 5);
      }
    }
  } 

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixNotEqualsScalarTest) {
    Matrix result = matrix != 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 01&& matrix(row, col) != 5) 
            || (result(row, col) == 0) && matrix(row, col) == 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, ScalarNotEqualsMatrixTest) {
    Matrix result = 5 != matrix;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 0 && matrix(row, col) == 5) 
            || (result(row, col) == 1) && matrix(row, col) != 5);
      }
    }
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixNotEqualsMatrixTest) {
    Matrix result = matrix != Matrices::Zeros(matrix.NumRows(), matrix.NumCols()) + 5;
    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) != 5) 
            || (result(row, col) == 0) && matrix(row, col) == 5);
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, NotMatrixTest) {
    Matrix result = !(matrix - 1);

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE((result(row, col) == 1 && matrix(row, col) == 1)
            || (result(row, col) == 0 && matrix(row, col) != 0));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixAndMatrixTest) {
    Matrix result = matrix && matrix;

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE(result(row, col));
      }
    }

    result = matrix && Matrix(matrix.NumRows(), matrix.NumCols());

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_FALSE(result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, MatrixOrMatrixTest) {
    Matrix result = Matrix(matrix.NumRows(), matrix.NumCols()) || matrix;

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_TRUE(result(row, col));
      }
    }

    result = Matrix(matrix.NumRows(), matrix.NumCols())
        || Matrix(matrix.NumRows(), matrix.NumCols());

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        ASSERT_FALSE(result(row, col));
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, HCatTest1) {
    Matrix result = matrix | identity;

    ASSERT_EQ(matrix.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols() + identity.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        if (col >= matrix.NumCols()) {
          if (row < identity.NumRows()) {
            ASSERT_EQ(identity(row, col - matrix.NumCols()), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        } else {
          if (row < matrix.NumRows()) {
            ASSERT_EQ(matrix(row, col), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, HCatTest2) {
    Matrix result = vector | identity;

    ASSERT_EQ(identity.NumRows(), result.NumRows());
    ASSERT_EQ(vector.NumCols() + identity.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        if (col >= vector.NumCols()) {
          if (row < identity.NumRows()) {
            ASSERT_EQ(identity(row, col - vector.NumCols()), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        } else {
          if (row < vector.NumRows()) {
            ASSERT_EQ(vector(row, col), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, HCatTest3) {
    Matrix empty;
    Matrix result = (empty | colVector);

    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(colVector.NumCols(), result.NumCols());
    ASSERT_TRUE(result.Equals(colVector));

    result = (colVector | empty);
    ASSERT_EQ(colVector.NumRows(), result.NumRows());
    ASSERT_EQ(colVector.NumCols(), result.NumCols());
    ASSERT_TRUE(result.Equals(colVector));
  }  

  TEST_F(MatrixOperatorOverloadTestSuite, VCatTest1) {
    Matrix result = (matrix , identity);

    ASSERT_EQ(matrix.NumRows() + identity.NumRows(), result.NumRows());
    ASSERT_EQ(matrix.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        if (row >= matrix.NumRows()) {
          if (col < identity.NumCols()) {
            ASSERT_EQ(identity(row - matrix.NumRows(), col), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        } else {
          if (col < matrix.NumCols()) {
            ASSERT_EQ(matrix(row, col), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, VCatTest2) {
    Matrix result = (colVector , identity);

    ASSERT_EQ(colVector.NumRows() + identity.NumRows(), result.NumRows());
    ASSERT_EQ(identity.NumCols(), result.NumCols());

    for (int row = 0; row < result.NumRows(); row++) {
      for (int col = 0; col < result.NumCols(); col++) {
        if (row >= colVector.NumRows()) {
          if (col < identity.NumCols()) {
            ASSERT_EQ(identity(row - colVector.NumRows(), col), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        } else {
          if (col < colVector.NumCols()) {
            ASSERT_EQ(colVector(row, col), result(row, col));
          } else {
            ASSERT_EQ(0, result(row, col));
          }
        }
      }
    }
  }

  TEST_F(MatrixOperatorOverloadTestSuite, VCatTest3) {
    Matrix empty;
    Matrix result = (empty , vector);

    ASSERT_EQ(vector.NumRows(), result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());
    ASSERT_TRUE(result.Equals(vector));

    result = (vector, empty);
    ASSERT_EQ(vector.NumRows(), result.NumRows());
    ASSERT_EQ(vector.NumCols(), result.NumCols());
    ASSERT_TRUE(result.Equals(vector));
  }

} // math
} // mdl
