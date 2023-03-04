#include <gtest/gtest.h>

#include <mdl/matrix.h>

namespace mdl {
namespace math {

  class AccessorTestSuite : public ::testing::Test {
    protected:
      float_t * data;

      void SetUp() override {
        data = new float_t[20];
        for (int i = 0; i < 20; i++) {
          data[i] = i;
        }
      }

      void TearDown() override {
        delete[] data;
      }
  }; 

  TEST_F(AccessorTestSuite, TestGetDirect) {
    // Expect equality.
    ASSERT_EQ(0, DirectAccessor::Get(data, 1, 0, 0));
    ASSERT_EQ(7, DirectAccessor::Get(data, 8, 0, 7));
    ASSERT_EQ(7, DirectAccessor::Get(data, 7, 1, 0));

    int rows = 4;
    int cols = 5;
    int count = 0;
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        ASSERT_EQ(count++,  DirectAccessor::Get(data, cols, row, col));
      }
    }
  }

  TEST_F(AccessorTestSuite, TestGetLREFDirect) {
    int rows = 4;
    int cols = 5;
    int count = 0;
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        DirectAccessor::GetLRef(data, cols, row, col) = -count;
        ASSERT_EQ(- count++,  DirectAccessor::Get(data, cols, row, col));
      }
    }
  }

  TEST_F(AccessorTestSuite, TestGetNumRowsNumColsDirect) {
    ASSERT_EQ(1, DirectAccessor::NumRows(1, 2));
    ASSERT_EQ(2, DirectAccessor::NumCols(1, 2));
  }

  TEST_F(AccessorTestSuite, TestGetRowColDirect) {
    ASSERT_EQ(1, DirectAccessor::GetRow(1, 2));
    ASSERT_EQ(2, DirectAccessor::GetCol(1, 2));
  }

  TEST_F(AccessorTestSuite, TestGetTransposed) {
    // Expect equality.
    ASSERT_EQ(0, TransposedAccessor::Get(data, 1, 0, 0));
    ASSERT_EQ(7, TransposedAccessor::Get(data, 8, 7, 0));
    ASSERT_EQ(7, TransposedAccessor::Get(data, 7, 0, 1));

    int rows = 4;
    int cols = 5;
    int count = 0;
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        ASSERT_EQ(count++,  TransposedAccessor::Get(data, cols, col, row));
      }
    }
  }

  TEST_F(AccessorTestSuite, TestGetLREFTransposed) {
    int rows = 4;
    int cols = 5;
    int count = 0;
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        TransposedAccessor::GetLRef(data, cols, col, row) = -count;
        ASSERT_EQ(- count++,  TransposedAccessor::Get(data, cols, col, row));
      }
    }
  }

  TEST_F(AccessorTestSuite, TestGetNumRowsNumColsTransposed) {
    ASSERT_EQ(2, TransposedAccessor::NumRows(1, 2));
    ASSERT_EQ(1, TransposedAccessor::NumCols(1, 2));
  }

  TEST_F(AccessorTestSuite, TestGetRowColTransposed) {
    ASSERT_EQ(2, TransposedAccessor::GetRow(1, 2));
    ASSERT_EQ(1, TransposedAccessor::GetCol(1, 2));
  }

  TEST_F(AccessorTestSuite, TestGet) {
    int rows = 4;
    int cols = 5;
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        ASSERT_EQ(
            DirectAccessor::Get(data, cols, row, col), 
            TransposedAccessor::Get(data, cols, col, row));
      }
    }
  }

} // math
} // mdl
