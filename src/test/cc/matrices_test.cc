#include <gtest/gtest.h>

#include <random>

#include <mdl/matrix.h>
#include <mdl/io.h>
#include <mdl/text.h>

namespace mdl {
namespace math {

  TEST(MatricesTestSuite, OnesMatrixTest) {
    Matrix ones = Matrices::Ones(2, 3);
    ASSERT_EQ(2, ones.NumRows());
    ASSERT_EQ(3, ones.NumCols());

    for (int row = 0; row < ones.NumRows(); row++) {
      for (int col = 0; col < ones.NumCols(); col++) {
        ASSERT_EQ(1, ones(row, col));
      }
    }
  }

  TEST(MatricesTestSuite, ZerosMatrixTest) {
    Matrix zeros = Matrices::Zeros(2, 3);
    ASSERT_EQ(2, zeros.NumRows());
    ASSERT_EQ(3, zeros.NumCols());

    for (int row = 0; row < zeros.NumRows(); row++) {
      for (int col = 0; col < zeros.NumCols(); col++) {
        ASSERT_EQ(0, zeros(row, col));
      }
    }
  }

  TEST(MatricesTestSuite, RandomMatrixTest) {
    Matrix rand = Matrices::Random(100, 101);

    ASSERT_EQ(100, rand.NumRows());
    ASSERT_EQ(101, rand.NumCols());

    float_t sum = 0.0;
    for (int row = 0; row < rand.NumRows(); row++) {
      for (int col = 0; col < rand.NumCols(); col++) {
        sum += rand(row, col);
      }
    }

    ASSERT_TRUE(std::abs(0.5 - sum / (100 * 101)) < 0.01);
  }

  TEST(MatricesTestSuite, SequenceMatrixTest) {
    Matrix seq = Matrices::Sequence(2, 10, 10);

    ASSERT_EQ(2, seq.NumRows());
    ASSERT_EQ(10, seq.NumCols());
    
    for (int col = 0; col < seq.NumCols(); col++) {
      ASSERT_EQ(col, seq(0, col));
      ASSERT_EQ(0, seq(1, col));
    }
  }

  TEST(MatricesTestSuite, SequenceMatrixTest_WithRange) {
    Matrix seq = Matrices::Sequence(2, 10, Range(1, 11));

    ASSERT_EQ(2, seq.NumRows());
    ASSERT_EQ(10, seq.NumCols());
    
    for (int col = 0; col < seq.NumCols(); col++) {
      ASSERT_EQ(1 + col, seq(0, col));
      ASSERT_EQ(0, seq(1, col));
    }
  }

  TEST(MatricesTestSuite, SequenceMatrixTest_WithDecrementingRange) {
    Matrix seq = Matrices::Sequence(2, 10, Range(10, 0, -1));

    ASSERT_EQ(2, seq.NumRows());
    ASSERT_EQ(10, seq.NumCols());
    
    for (int col = 0; col < seq.NumCols(); col++) {
      ASSERT_EQ(seq.NumCols() - col, seq(0, col));
      ASSERT_EQ(0, seq(1, col));
    }
  }

  TEST(MatricesTestSuite, DefaultMatrixTest) {
    Matrix def = Matrices::Default(2, 10, -2.5);

    ASSERT_EQ(2, def.NumRows());
    ASSERT_EQ(10, def.NumCols());
    
    for (int row = 0; row < def.NumRows(); row++) {
      for (int col = 0; col < def.NumCols(); col++) {
        ASSERT_EQ(-2.5, def(row, col));
      }
    }
  }

  TEST(MatricesTestSuite, WithValuesMatrixTest) {
    Matrix expected(2,2);
    ASSERT_TRUE(expected.Equals(Matrices::WithValues(2, {0, 0, 0, 0})));

    expected(0,0) = 1;
    ASSERT_TRUE(expected.Equals(Matrices::WithValues(2, {1, 0, 0})));

    expected(0,1) = 2;
    ASSERT_TRUE(expected.Equals(Matrices::WithValues(2, {1, 2, 0})));

    expected(1,0) = 3;
    ASSERT_TRUE(expected.Equals(Matrices::WithValues(2, {1, 2, 3})));
    ASSERT_FALSE(expected.Equals(Matrices::WithValues(2, {1, 2, 3, 4})));

    expected(1,1) = 4;
    ASSERT_TRUE(expected.Equals(Matrices::WithValues(2, {1, 2, 3, 4})));
  }

  TEST(MatricesTestSuite, WithValuesMatrixTest_TestNumRows) {
    Matrix m = Matrices::WithValues(4, {1,2,3,4});
    ASSERT_EQ(1, m.NumRows());

    m = Matrices::WithValues(4, {1,2,3,4,5});
    ASSERT_EQ(2, m.NumRows());
    ASSERT_EQ(5, m(1, 0));
  }

  TEST(MatricesTestSuite, WithValuesMatrixTest_FromCsv) {
    Matrix m = Matrices::FromCsv(
        "src/test/resources/matrix/MatricesTestSuite_FromCsv.csv");

    

    ASSERT_TRUE(m.Equals(
      Matrices::WithValues(4, {1, 2, 0, 0,
                               3, 4, 5, 6,
                               7, 8, 9, 0})));
  }

  TEST(MatricesTestSuite, WithValuesMatrixTest_FromCsv_NotFound) {
    ASSERT_THROW(Matrices::FromCsv("NotFound.csv"), mdl::io::file_not_found_exception);
  }

  TEST(MatricesTestSuite, WithValuesMatrixTest_FromCsv_Invalid) {
    ASSERT_THROW(
        Matrices::FromCsv(
            "src/test/resources/matrix/MatricesTestSuite_FromCsv_Invalid.csv"), 
        mdl::text::parse_exception);
  }

} // math
} // mdl
