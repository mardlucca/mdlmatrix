#include <gtest/gtest.h>

#include <mdl/matrix.h>

namespace mdl {
namespace math {
namespace op {

  TEST(OperationTest, TestAbs) {
    double out;
    Abs::operate(-2.5, out);
    ASSERT_EQ(2.5, out);

    out = -3.5;
    Abs::operate(out);
    ASSERT_EQ(3.5, out);
  }

  TEST(OperationTest, TestAddition) {
    double out;
    Addition::operate(2, 3, out);
    ASSERT_EQ(5.0, out);

    Addition::operate(out, 4);
    ASSERT_EQ(9.0, out);
  }

  TEST(OperationTest, TestNegate) {
    double out;
    Negate::operate(-2.1, out);
    ASSERT_EQ(2.1, out);

    Negate::operate(out);
    ASSERT_EQ(-2.1, out);
  }

  TEST(OperationTest, TestMax) {
    double out;
    Max::operate(1, 2, out);
    ASSERT_EQ(2, out);
    Max::operate(2, 1, out);
    ASSERT_EQ(2, out);

    Max::operate(out, 1);
    ASSERT_EQ(2, out);

    Max::operate(out, 3);
    ASSERT_EQ(3, out);
  }

  TEST(OperationTest, TestMin) {
    double out;
    Min::operate(1, 2, out);
    ASSERT_EQ(1, out);
    Min::operate(2, 1, out);
    ASSERT_EQ(1, out);

    Min::operate(out, 3);
    ASSERT_EQ(1, out);

    Min::operate(out, 0);
    ASSERT_EQ(0, out);
  }
} // op
} // math
} // mdl
