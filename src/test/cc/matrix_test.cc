#include <gtest/gtest.h>

#include <iostream>
using std::cout;
using std::endl;

#include <mdl/matrix.h>

namespace mdl {
namespace math {

  class MatrixTestSuite : public ::testing::Test {
    protected:
      Matrix matrix;

      MatrixTestSuite() : matrix(Matrix(4,3)) {
        size_t count = 0;
        for (size_t row = 0; row < matrix.NumRows(); row++) {
          for (size_t col = 0; col < matrix.NumCols(); col++) {
            matrix(row, col) = ++count;
          }
        }
      }
  };

  TEST_F(MatrixTestSuite, TestGetSet) {
    size_t count = 0;
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        ASSERT_EQ(++count, matrix(row, col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestNumRowsCols) {
    ASSERT_EQ(4, matrix.NumRows());
    ASSERT_EQ(3, matrix.NumCols());
  }

  TEST_F(MatrixTestSuite, TestEquals) {
    ASSERT_TRUE(matrix.Equals(matrix));
    ASSERT_TRUE(matrix.Equals(Matrix(matrix)));
    ASSERT_FALSE(matrix.Equals(matrix().Transpose()));
    ASSERT_FALSE(matrix().Transpose().Equals(matrix));
    ASSERT_TRUE(matrix().Transpose().Equals(matrix().Transpose()));

    Matrix other = matrix;
    ASSERT_TRUE(other.Equals(matrix));

    other(1,1) = -other(1,1);
    ASSERT_FALSE(other.Equals(matrix));
  }

  TEST_F(MatrixTestSuite, TestForEach) {
    matrix.ForEach([](size_t row, size_t col, float_t& val) {
      val = -val;
    });
    size_t count = 0;
    matrix.ForEach([&count](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0);
      count++;
    });
    ASSERT_EQ(matrix.NumRows() * matrix.NumCols(), count);
  }

  TEST_F(MatrixTestSuite, TestBasicSlice) {
    Slice slice = matrix(Range(1,3), RightRange(1));
    ASSERT_EQ(2, slice.NumRows());
    ASSERT_EQ(2, slice.NumCols());

    for (size_t row = 0; row < slice.NumRows(); row++) {
      for (size_t col = 0; col < slice.NumCols(); col++) {
        ASSERT_EQ(slice(row, col), matrix(1 + row, 1 + col));
      }
    }

    slice(0, 0) += 100;
    slice(0, 1) += 101;
    slice(1, 0) += 1000;
    slice(1, 1) += 1001;

    for (size_t row = 0; row < slice.NumRows(); row++) {
      for (size_t col = 0; col < slice.NumCols(); col++) {
        ASSERT_EQ(slice(row, col), matrix(1 + row, 1 + col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestCopyConstructor) {
    Matrix m = matrix;

    cout << matrix << endl;
    cout << m << endl;

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        m(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), m(row, col) - 100);
      }
    }
  }

  TEST_F(MatrixTestSuite, TestCopyConstructorFromSlice) {
    Slice slice = matrix(RightRange(0), RightRange(0));
    Matrix m = slice;

    for (size_t row = 0; row < slice.NumRows(); row++) {
      for (size_t col = 0; col < slice.NumCols(); col++) {
        slice(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(m(row, col) + 100, slice(row, col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestMoveConstructor) {
    Matrix m = std::move(matrix);

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        m(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), m(row, col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestMoveConstructorFromSlice) {
    Slice slice = matrix(RightRange(0), RightRange(0));
    // move semantics not defined when converting from slice to matrix,
    //  so a copy will be performed instead. If move semantics is required
    //  user should keep a reference to the slice instead, rather than converting
    //  to a matrix.
    Matrix m = std::move(slice);

    for (size_t row = 0; row < slice.NumRows(); row++) {
      for (size_t col = 0; col < slice.NumCols(); col++) {
        slice(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(m(row, col) + 100, slice(row, col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestCopyAsssignment) {
    Matrix m(0,0);

    m = matrix;

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        m(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), m(row, col) - 100);
      }
    }
  }

  TEST_F(MatrixTestSuite, TestCopyAsssignmentFromSlice) {
    Matrix m(0,0);
    Slice slice = matrix(RightRange(0), RightRange(0));
    m = slice;

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        m(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), m(row, col) - 100);
      }
    }
  }

  TEST_F(MatrixTestSuite, TestMoveAsssignment) {
    Matrix m(0,0);

    m = std::move(matrix);

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        m(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), m(row, col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestMoveAsssignmentFromSlice) {
    Matrix m(0,0);
    Slice slice = matrix(RightRange(0), RightRange(0));
    m = std::move(slice);

    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        m(row, col) += 100;
      }
    }

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), m(row, col) - 100);
      }
    }
  }

  TEST_F(MatrixTestSuite, TestTranspose) {
    Slice transposed = matrix().Transpose();

    for (size_t row = 0; row < transposed.NumRows(); row++) {
      for (size_t col = 0; col < transposed.NumCols(); col++) {
        ASSERT_EQ(matrix(col, row), transposed(row, col));
        transposed(row, col) += 100;
        ASSERT_EQ(matrix(col, row), transposed(row, col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestMatrixTranspose) {
    Matrix transposed = matrix.Transpose();

    for (size_t row = 0; row < transposed.NumRows(); row++) {
      for (size_t col = 0; col < transposed.NumCols(); col++) {
        ASSERT_EQ(matrix(col, row), transposed(row, col));
        transposed(row, col) += 100;
        ASSERT_EQ(matrix(col, row), transposed(row, col) - 100);
      }
    }
  }

  TEST_F(MatrixTestSuite, TestMaterializeTransposed) {
    Matrix transposed = matrix().Transpose();

    for (size_t row = 0; row < transposed.NumRows(); row++) {
      for (size_t col = 0; col < transposed.NumCols(); col++) {
        ASSERT_EQ(matrix(col, row), transposed(row, col));
        transposed(row, col) += 100;
        ASSERT_EQ(matrix(col, row), transposed(row, col) - 100);
      }
    }
  }

  TEST_F(MatrixTestSuite, TestTransposeTwice) {
    Slice transposed = matrix().Transpose().Transpose();

    for (size_t row = 0; row < transposed.NumRows(); row++) {
      for (size_t col = 0; col < transposed.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), transposed(row, col));
        transposed(row, col) += 100;
        ASSERT_EQ(matrix(row, col), transposed(row, col));
      }
    }
  }

  TEST_F(MatrixTestSuite, TestTransposeTwiceAndMaterialize) {
    Matrix transposed = matrix().Transpose().Transpose();

    for (size_t row = 0; row < transposed.NumRows(); row++) {
      for (size_t col = 0; col < transposed.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), transposed(row, col));
        transposed(row, col) += 100;
        ASSERT_EQ(matrix(row, col), transposed(row, col) - 100);
      }
    }
  }

} // math
} // mdl