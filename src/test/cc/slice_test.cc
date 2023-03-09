#include <gtest/gtest.h>

#include <iostream>

#include <mdl/matrix.h>

namespace mdl {
namespace math {

  using std::cout;
  using std::endl;

  class SliceTestSuite : public ::testing::Test {
    protected:
      Matrix matrix;
      Slice<Range, RightRange, DirectAccessor> slice;

      SliceTestSuite() : matrix(Matrix(4,3)), slice(matrix(Range(1, 3), RightRange(1))) {
        size_t count = 0;
        for (size_t row = 0; row < matrix.NumRows(); row++) {
          for (size_t col = 0; col < matrix.NumCols(); col++) {
            matrix(row, col) = ++count;
          }
        }
      }
  };

  TEST_F(SliceTestSuite, TestGetSet) {
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
    float_t val = slice(1, 1);
    slice(1, 1) += 1001;
    ASSERT_EQ(val + 1001, slice(1, 1));

    for (size_t row = 0; row < slice.NumRows(); row++) {
      for (size_t col = 0; col < slice.NumCols(); col++) {
        ASSERT_EQ(slice(row, col), matrix(1 + row, 1 + col));
      }
    }
  }

  TEST_F(SliceTestSuite, TestCopyConstructor) {
    Slice s = slice;

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        s(row, col) += 100;
      }
    }

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(matrix(1 + row, 1 + col), s(row, col));
        ASSERT_TRUE(matrix(1 + row, 1 + col) > 100);
      }
    }
  }

  TEST_F(SliceTestSuite, TestMoveConstructor) {
    Slice s = std::move(slice);

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        s(row, col) += 100;
        ASSERT_EQ(matrix(1 + row, 1 + col), s(row, col));
      }
    }

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(matrix(1 + row, 1 + col), s(row, col));
      }
    }
  }

  TEST_F(SliceTestSuite, TestCopyAssignment) {
    Matrix m(3,3);
    m(RightRange(0), RightRange(0)) = matrix;

    for (size_t row = 0; row < m.NumRows(); row++) {
      for (size_t col = 0; col < m.NumCols(); col++) {
        ASSERT_EQ(matrix(row, col), m(row, col));
      }
    }

    Slice s = matrix(UnitRange(0), RightRange(0));
    matrix(UnitRange(1), RightRange(0)) = s;

    for (size_t col = 0; col < matrix.NumCols(); col++) {
      ASSERT_EQ(matrix(0, col), matrix(1, col));
    }
  }

  TEST_F(SliceTestSuite, TestMoveAssignment) {
    Slice s = matrix(UnitRange(0), RightRange(0));
    matrix(UnitRange(1), RightRange(0)) = std::move(s);

    for (size_t col = 0; col < matrix.NumCols(); col++) {
      ASSERT_EQ(matrix(0, col), matrix(1, col));
    }
  }

  TEST_F(SliceTestSuite, TestSliceSlice) {
    Slice s = slice(RightRange(0), UnitRange(1));

    ASSERT_EQ(2, s.NumRows());
    ASSERT_EQ(1, s.NumCols());

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(s(row, col), matrix(1 + row, 2 + col));
        ASSERT_EQ(s(row, col), slice(row, 1 + col));
      }
    }

    s(0, 0) += 1000;
    s(1, 0) += 1000;

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(s(row, col), matrix(1 + row, 2 + col));
        ASSERT_EQ(s(row, col), slice(row, 1 + col));
      }
    }
  }

  TEST_F(SliceTestSuite, TestSliceSliceTranspose) {
    Slice s = slice(RightRange(0), UnitRange(1)).Transpose();

    ASSERT_EQ(1, s.NumRows());
    ASSERT_EQ(2, s.NumCols());

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(s(row, col), matrix(1 + col, 2 + row));
        ASSERT_EQ(s(row, col), slice(col, 1 + row));
      }
    }

    s(0, 0) += 1000;
    s(0, 1) += 1000;

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(s(row, col), matrix(1 + col, 2 + row));
        ASSERT_EQ(s(row, col), slice(col, 1 + row));
      }
    }
  }

  TEST_F(SliceTestSuite, TestSliceTransposeSlice) {
    Slice s = slice.Transpose()(RightRange(0), UnitRange(1));

    ASSERT_EQ(2, s.NumRows());
    ASSERT_EQ(1, s.NumCols());

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(s(row, col), matrix(2 + col, 1 + row));
        ASSERT_EQ(s(row, col), slice(1 + col, row));
      }
    }

    s(0, 0) += 1000;
    s(0, 1) += 1000;

    for (size_t row = 0; row < s.NumRows(); row++) {
      for (size_t col = 0; col < s.NumCols(); col++) {
        ASSERT_EQ(s(row, col), matrix(2 + col, 1 + row));
        ASSERT_EQ(s(row, col), slice(1 + col, row));
      }
    }
  }

  TEST_F(SliceTestSuite, TestTransposeSliceSlice) {
    Slice s = matrix.Transpose()(Range(1, 3), RightRange(1))(RightRange(0), UnitRange(1));

    ASSERT_EQ(2, s.NumRows());
    ASSERT_EQ(1, s.NumCols());

    ASSERT_EQ(8, s(0, 0));
    ASSERT_EQ(9, s(1, 0));
  }

  TEST_F(SliceTestSuite, TestMirrorHorizontally) {
    size_t len = matrix.NumCols();
    Matrix copy = matrix;
    matrix(RightRange(0), RightRange(0)) = matrix(RightRange(0), Range(len - 1, -1, -1));
    
    for (size_t row = 0; row < copy.NumRows(); row++) {
      for (size_t col = 0; col < copy.NumCols(); col++) {
        ASSERT_EQ(copy(row, col), matrix(row, len - 1 - col));
      }
    }
  }

  TEST_F(SliceTestSuite, TestMirror) {
    size_t rows = matrix.NumRows();
    size_t cols = matrix.NumCols();
    Matrix copy = matrix;
    matrix({rows - 1, -1, -1}, {}) = matrix({}, {cols - 1, -1, -1});
    
    for (size_t row = 0; row < copy.NumRows(); row++) {
      for (size_t col = 0; col < copy.NumCols(); col++) {
        ASSERT_EQ(copy(row, col), matrix(rows - row - 1, cols - 1 - col));
      }
    }
  }

  TEST_F(SliceTestSuite, TestTransposedMirror) {
    size_t rows = matrix.NumRows();
    size_t cols = matrix.NumCols();
    Matrix copy = matrix;
    matrix({rows - 1, -1, -1}, {}).Transpose() = matrix({}, {cols - 1, -1, -1}).Transpose();
    
    for (size_t row = 0; row < copy.NumRows(); row++) {
      for (size_t col = 0; col < copy.NumCols(); col++) {
        ASSERT_EQ(copy(row, col), matrix(rows - row - 1, cols - 1 - col));
      }
    }
  }

  TEST_F(SliceTestSuite, TestTransposedMirror_GenericAssignment) {
    size_t rows = matrix.NumRows();
    size_t cols = matrix.NumCols();
    Matrix copy = matrix;
    matrix(Range(rows - 1, -1, -1), RightRange(0)).Transpose() = 
        matrix({}, {cols - 1, -1, -1}).Transpose();
    
    for (size_t row = 0; row < copy.NumRows(); row++) {
      for (size_t col = 0; col < copy.NumCols(); col++) {
        ASSERT_EQ(copy(row, col), matrix(rows - row - 1, cols - 1 - col));
      }
    }
  }

} // math
} // mdl