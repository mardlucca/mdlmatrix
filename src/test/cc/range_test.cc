#include <gtest/gtest.h>

#include <mdl/matrix.h>

namespace mdl {
namespace math {

  TEST(RangeTest, TestLeftRange) {
    LeftRange range(10);
    ASSERT_EQ(10, range.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(i, range.Get(i));
    }
  }

  TEST(RangeTest, TestLeftRangeFitToBounds) {
    LeftRange range(10);
    ASSERT_EQ(10, range.FitToBounds(11).Length());
    ASSERT_EQ(10, range.FitToBounds(10).Length());
    ASSERT_EQ(9, range.FitToBounds(9).Length());
    ASSERT_EQ(1, range.FitToBounds(1).Length());
    ASSERT_EQ(0, range.FitToBounds(0).Length());

    LeftRange range2 = LeftRange(-2).FitToBounds(2);
    ASSERT_EQ(0, range2.Length());
    ASSERT_EQ(2, range.Get(2));
  }

  TEST(RangeTest, TestRightRange) {
    RightRange range(10);
    ASSERT_EQ(0, range.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(10 + i, range.Get(i));
    }

    range = RightRange(5).FitToBounds(10);
    ASSERT_EQ(5, range.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(5 + i, range.Get(i));
    }
  }

  TEST(RangeTest, TestRightRangeFitToBounds) {
    RightRange range(10);
    ASSERT_EQ(0, range.Length());
    ASSERT_EQ(10, range.FitToBounds(20).Length());
    ASSERT_EQ(1, range.FitToBounds(11).Length());
    ASSERT_EQ(0, range.FitToBounds(10).Length());
    ASSERT_EQ(0, range.FitToBounds(9).Length());  
    ASSERT_EQ(0, range.FitToBounds(0).Length());  

    RightRange range2 = RightRange(-2).FitToBounds(5);
    ASSERT_EQ(5, range2.Length());
    ASSERT_EQ(2, range2.Get(2));
  }

  TEST(RangeTest, TestUnitRange) {
    UnitRange range(10);
    ASSERT_EQ(1, range.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(10, range.Get(i));
    }
  }

  TEST(RangeTest, TestUnitRangeFitToBounds) {
    UnitRange range(10);
    ASSERT_EQ(1, range.Length());
    ASSERT_EQ(1, range.FitToBounds(20).Length());
    ASSERT_EQ(1, range.FitToBounds(11).Length());
    ASSERT_EQ(0, range.FitToBounds(10).Length());
    ASSERT_EQ(0, range.FitToBounds(9).Length());  
    ASSERT_EQ(0, range.FitToBounds(0).Length());

    UnitRange range2 = UnitRange(-2).FitToBounds(5);
    ASSERT_EQ(0, range2.Length());
    ASSERT_EQ(-1, range2.Get(0));
  }


  TEST(RangeTest, TestRangeIncrementing) {
    Range range(5, 10);
    ASSERT_EQ(5, range.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(5 + i, range.Get(i));
    }

    Range range2(5, 10, 2);
    ASSERT_EQ(3, range2.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(5 + i * 2, range2.Get(i));
    }

    ASSERT_EQ(0, Range(5,3).Length());
    ASSERT_EQ(0, Range(5,4).Length());
    
    ASSERT_EQ(0, Range(5,5,2).Length());
    ASSERT_EQ(1, Range(5,6,2).Length());
    ASSERT_EQ(1, Range(5,7,2).Length());
    ASSERT_EQ(2, Range(5,8,2).Length());
  }


  TEST(RangeTest, TestRangeDecrementing) {
    Range range(10, 5, -1);
    ASSERT_EQ(5, range.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(10 - i, range.Get(i));
    }

    Range range2(10, 5, -2);
    ASSERT_EQ(3, range2.Length());
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(10 - i * 2, range2.Get(i));
    }

    ASSERT_EQ(0, Range(3, 5, -1).Length());
    ASSERT_EQ(0, Range(4, 5, -1).Length());

    ASSERT_EQ(0, Range(5,5,-2).Length());
    ASSERT_EQ(1, Range(6,5,-2).Length());
    ASSERT_EQ(1, Range(7,5,-2).Length());
    ASSERT_EQ(2, Range(8,5,-2).Length());
  }

  TEST(RangeTest, TestIncrementingRangeFitToBounds) {
      Range range(5, 10);
      ASSERT_EQ(5, range.FitToBounds(10).Length());
      ASSERT_EQ(4, range.FitToBounds(9).Length());
      ASSERT_EQ(3, range.FitToBounds(8).Length());
      ASSERT_EQ(2, range.FitToBounds(7).Length());
      ASSERT_EQ(1, range.FitToBounds(6).Length());
      ASSERT_EQ(0, range.FitToBounds(5).Length());
      ASSERT_EQ(0, range.FitToBounds(4).Length());

      Range range2(5, 10, 2);
      ASSERT_EQ(3, range2.FitToBounds(10).Length());
      ASSERT_EQ(2, range2.FitToBounds(9).Length());
      ASSERT_EQ(2, range2.FitToBounds(8).Length());
      ASSERT_EQ(1, range2.FitToBounds(7).Length());
      ASSERT_EQ(1, range2.FitToBounds(6).Length());
      ASSERT_EQ(0, range2.FitToBounds(5).Length());
      ASSERT_EQ(0, range2.FitToBounds(4).Length());

      Range range3 = Range(-5, 20).FitToBounds(5);
      ASSERT_EQ(5, range3.FitToBounds(5).Length());
      ASSERT_EQ(0, range3.Get(0));
      ASSERT_EQ(1, range3.Get(1));

      Range range4 = Range(-5, 20, 2).FitToBounds(5);
      ASSERT_EQ(3, range4.FitToBounds(5).Length());
      ASSERT_EQ(0, range4.Get(0));
      ASSERT_EQ(2, range4.Get(1));
  }

  TEST(RangeTest, TestDecrementingRangeFitToBounds) {
      Range range(10, 5, -1);
      ASSERT_EQ(5, range.FitToBounds(15).Length());
      ASSERT_EQ(4, range.FitToBounds(10).Length());
      ASSERT_EQ(3, range.FitToBounds(9).Length());
      ASSERT_EQ(2, range.FitToBounds(8).Length());
      ASSERT_EQ(1, range.FitToBounds(7).Length());
      ASSERT_EQ(0, range.FitToBounds(6).Length());
      ASSERT_EQ(0, range.FitToBounds(5).Length());
      ASSERT_EQ(0, range.FitToBounds(4).Length());

      Range range2(10, 5, -2);
      ASSERT_EQ(3, range2.FitToBounds(15).Length());
      ASSERT_EQ(2, range2.FitToBounds(10).Length());
      ASSERT_EQ(2, range2.FitToBounds(9).Length());
      ASSERT_EQ(1, range2.FitToBounds(8).Length());
      ASSERT_EQ(1, range2.FitToBounds(7).Length());
      ASSERT_EQ(0, range2.FitToBounds(6).Length());
      ASSERT_EQ(0, range2.FitToBounds(5).Length());
      ASSERT_EQ(0, range2.FitToBounds(4).Length());

      Range range3 = Range(20, -5, -1).FitToBounds(5);
      ASSERT_EQ(5, range3.FitToBounds(5).Length());
      ASSERT_EQ(4, range3.Get(0));
      ASSERT_EQ(3, range3.Get(1));

      Range range4 = Range(20, -5, -2).FitToBounds(5);
      ASSERT_EQ(3, range4.FitToBounds(5).Length());
      ASSERT_EQ(4, range4.Get(0));
      ASSERT_EQ(2, range4.Get(1));
      ASSERT_EQ(0, range4.Get(2));
  }

  TEST(RangeTest, TestFitAndCompose_FromLeftRange_SliceLeftRange) {
    LeftRange current(10);

    auto newRange = Ranges::FitAndCompose(current, LeftRange(9));
    ASSERT_EQ(9, newRange.Length());
    ASSERT_EQ(0, newRange.Get(0));
    auto newRange2 = Ranges::FitAndCompose(current, LeftRange(10));
    ASSERT_EQ(10, newRange2.Length());
    ASSERT_EQ(1, newRange2.Get(1));
    auto newRange3 = Ranges::FitAndCompose(current, LeftRange(11));
    ASSERT_EQ(10, newRange3.Length());
    ASSERT_EQ(2, newRange3.Get(2));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, LeftRange(0)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, LeftRange(-1)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromLeftRange_SliceRightRange) {
    LeftRange current(10);

    auto newRange = Ranges::FitAndCompose(current, RightRange(8));
    ASSERT_EQ(2, newRange.Length());
    ASSERT_EQ(8, newRange.Get(0));
    ASSERT_EQ(9, newRange.Get(1));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, RightRange(10)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, RightRange(11)).Length());

    auto newRange2 = Ranges::FitAndCompose(current, RightRange(5));
    ASSERT_EQ(5, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    ASSERT_EQ(6, newRange2.Get(1));

    auto newRange3 = Ranges::FitAndCompose(current, RightRange(-2));
    ASSERT_EQ(10, newRange3.Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromLeftRange_SliceUnitRange) {
    LeftRange current(10);

    auto newRange = Ranges::FitAndCompose(current, UnitRange(9));
    ASSERT_EQ(1, newRange.Length());
    ASSERT_EQ(9, newRange.Get(0));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, UnitRange(10)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, UnitRange(11)).Length());

    auto newRange2 = Ranges::FitAndCompose(current, UnitRange(0));
    ASSERT_EQ(1, newRange2.Length());
    ASSERT_EQ(0, newRange2.Get(0));

    auto newRange3 = Ranges::FitAndCompose(current, UnitRange(-1));
    ASSERT_EQ(0, newRange3.Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(LeftRange(0), UnitRange(0)).Length());
    ASSERT_EQ(1, Ranges::FitAndCompose(LeftRange(1), UnitRange(0)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromLeftRange_SliceRange) {
    LeftRange current(10);

    auto newRange = Ranges::FitAndCompose(current, Range(2, 5));
    ASSERT_EQ(3, newRange.Length());
    ASSERT_EQ(2, newRange.Get(0));
    ASSERT_EQ(4, newRange.Get(2));

    auto newRange2 = Ranges::FitAndCompose(current, Range(-2, 15));
    ASSERT_EQ(10, newRange2.Length());
    ASSERT_EQ(0, newRange2.Get(0));
    ASSERT_EQ(2, newRange2.Get(2));
    ASSERT_EQ(9, newRange2.Get(9));

    auto newRange3 = Ranges::FitAndCompose(current, Range(15, -1, -2));
    ASSERT_EQ(5, newRange3.Length());
    ASSERT_EQ(9, newRange3.Get(0));
    ASSERT_EQ(1, newRange3.Get(4));

    ASSERT_EQ(0, Ranges::FitAndCompose(LeftRange(0), Range(-5, 10)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromRightRange_SliceLeftRange) {
    RightRange current = RightRange(5).FitToBounds(10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, LeftRange(6));
    ASSERT_EQ(5, newRange.Length());
    ASSERT_EQ(5, newRange.Get(0));
    ASSERT_EQ(9, newRange.Get(4));
    auto newRange2 = Ranges::FitAndCompose(current, LeftRange(5));
    ASSERT_EQ(5, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    ASSERT_EQ(9, newRange2.Get(4));
    auto newRange3 = Ranges::FitAndCompose(current, LeftRange(4));
    ASSERT_EQ(4, newRange3.Length());
    ASSERT_EQ(5, newRange3.Get(0));
    ASSERT_EQ(8, newRange3.Get(3));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, LeftRange(0)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, LeftRange(-1)).Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(RightRange(1).FitToBounds(1), LeftRange(10)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromRightRange_SliceRightRange) {
    RightRange current = RightRange(5).FitToBounds(10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, RightRange(2));
    ASSERT_EQ(3, newRange.Length());
    ASSERT_EQ(7, newRange.Get(0));
    ASSERT_EQ(9, newRange.Get(2));

    auto newRange2 = Ranges::FitAndCompose(current, RightRange(-1));
    ASSERT_EQ(5, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    ASSERT_EQ(9, newRange2.Get(4));

    ASSERT_EQ(1, Ranges::FitAndCompose(current, RightRange(4)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, RightRange(5)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, RightRange(6)).Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(RightRange(1).FitToBounds(1), RightRange(0)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromRightRange_SliceUnitRange) {
    RightRange current = RightRange(5).FitToBounds(10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, UnitRange(1));
    ASSERT_EQ(1, newRange.Length());
    ASSERT_EQ(6, newRange.Get(0));

    auto newRange2 = Ranges::FitAndCompose(current, UnitRange(4));
    ASSERT_EQ(1, newRange2.Length());
    ASSERT_EQ(9, newRange2.Get(0));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, UnitRange(-1)).Length());
    ASSERT_EQ(1, Ranges::FitAndCompose(current, UnitRange(0)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, UnitRange(5)).Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(RightRange(1).FitToBounds(1), UnitRange(0)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromRightRange_SliceRange) {
    RightRange current = RightRange(5).FitToBounds(10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, Range(1,3));
    ASSERT_EQ(2, newRange.Length());
    ASSERT_EQ(6, newRange.Get(0));
    ASSERT_EQ(8, newRange.Get(2));

    auto newRange2 = Ranges::FitAndCompose(current, Range(-2,7));
    ASSERT_EQ(5, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    ASSERT_EQ(9, newRange2.Get(4));

    auto newRange3 = Ranges::FitAndCompose(current, Range(6,-3, -2));
    ASSERT_EQ(3, newRange3.Length());
    ASSERT_EQ(9, newRange3.Get(0));
    ASSERT_EQ(7, newRange3.Get(1));
    ASSERT_EQ(5, newRange3.Get(2));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, Range(1,1)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(RightRange(1).FitToBounds(1), Range(0, 10)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromUnitRange_SliceLeftRange) {
    UnitRange current = UnitRange(5);
    ASSERT_EQ(1, current.Length());

    auto newRange = Ranges::FitAndCompose(current, LeftRange(1));
    ASSERT_EQ(1, newRange.Length());
    ASSERT_EQ(5, newRange.Get(0));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, LeftRange(0)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(UnitRange(-1), LeftRange(10)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromUnitRange_SliceRightRange) {
    UnitRange current = UnitRange(5);
    ASSERT_EQ(1, current.Length());

    auto newRange = Ranges::FitAndCompose(current, RightRange(0));
    ASSERT_EQ(1, newRange.Length());
    ASSERT_EQ(5, newRange.Get(0));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, RightRange(1)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(UnitRange(-1), RightRange(0)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromUnitRange_SliceUnitRange) {
    UnitRange current = UnitRange(5);
    ASSERT_EQ(1, current.Length());

    auto newRange = Ranges::FitAndCompose(current, UnitRange(0));
    ASSERT_EQ(1, newRange.Length());
    ASSERT_EQ(5, newRange.Get(0));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, UnitRange(1)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(UnitRange(-1), UnitRange(0)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromUnitRange_SliceRange) {
    UnitRange current = UnitRange(5);
    ASSERT_EQ(1, current.Length());

    auto newRange = Ranges::FitAndCompose(current, Range(0,1));
    ASSERT_EQ(1, newRange.Length());
    ASSERT_EQ(5, newRange.Get(0));

    auto newRange2 = Ranges::FitAndCompose(current, Range(-2,2));
    ASSERT_EQ(1, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));

    ASSERT_EQ(0, Ranges::FitAndCompose(current, Range(1,10)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, Range(0,0,2)).Length());
    ASSERT_EQ(1, Ranges::FitAndCompose(current, Range(0,1,2)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(UnitRange(-1), Range(0,10)).Length());
  }

  TEST(RangeTest, TestFitAndCompose_FromRange_SliceLeftRange) {
    Range current = Range(5, 10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, LeftRange(6));
    ASSERT_EQ(5, newRange.Length());
    ASSERT_EQ(5, newRange.Get(0));
    ASSERT_EQ(9, newRange.Get(4));
    auto newRange2 = Ranges::FitAndCompose(current, LeftRange(5));
    ASSERT_EQ(5, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    ASSERT_EQ(9, newRange2.Get(4));
    auto newRange3 = Ranges::FitAndCompose(current, LeftRange(4));
    ASSERT_EQ(4, newRange3.Length());
    ASSERT_EQ(5, newRange3.Get(0));
    ASSERT_EQ(8, newRange3.Get(3));

    ASSERT_EQ(1, Ranges::FitAndCompose(current, LeftRange(1)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, LeftRange(0)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, LeftRange(-1)).Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(Range(1, 1), LeftRange(10)).Length());

    auto newRange4 = Ranges::FitAndCompose(Range(10, 5, -2), LeftRange(2));
    ASSERT_EQ(2, newRange4.Length());
    ASSERT_EQ(10, newRange4.Get(0));
    ASSERT_EQ(8, newRange4.Get(1));
  }

  TEST(RangeTest, TestFitAndCompose_FromRange_SliceRightRange) {
    Range current = Range(5, 10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, RightRange(-1));
    ASSERT_EQ(5, newRange.Length());
    ASSERT_EQ(5, newRange.Get(0));
    ASSERT_EQ(9, newRange.Get(4));
    auto newRange2 = Ranges::FitAndCompose(current, RightRange(0));
    ASSERT_EQ(5, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    ASSERT_EQ(9, newRange2.Get(4));
    auto newRange3 = Ranges::FitAndCompose(current, RightRange(1));
    ASSERT_EQ(4, newRange3.Length());
    ASSERT_EQ(6, newRange3.Get(0));
    ASSERT_EQ(9, newRange3.Get(3));

    ASSERT_EQ(1, Ranges::FitAndCompose(current, RightRange(4)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, RightRange(5)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, RightRange(6)).Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(Range(1, 1), RightRange(0)).Length());

    auto newRange4 = Ranges::FitAndCompose(Range(10, 5, -2), RightRange(1));
    ASSERT_EQ(2, newRange4.Length());
    ASSERT_EQ(8, newRange4.Get(0));
    ASSERT_EQ(6, newRange4.Get(1));
  }

  TEST(RangeTest, TestFitAndCompose_FromRange_SliceUnitRange) {
    Range current = Range(5, 10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, UnitRange(-1));
    ASSERT_EQ(0, newRange.Length());
    auto newRange2 = Ranges::FitAndCompose(current, UnitRange(0));
    ASSERT_EQ(1, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    auto newRange3 = Ranges::FitAndCompose(current, UnitRange(1));
    ASSERT_EQ(1, newRange3.Length());
    ASSERT_EQ(6, newRange3.Get(0));

    ASSERT_EQ(1, Ranges::FitAndCompose(current, UnitRange(4)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, UnitRange(5)).Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(Range(1, 1), UnitRange(0)).Length());
    ASSERT_EQ(1, Ranges::FitAndCompose(Range(10, 5, -2), UnitRange(2)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(Range(10, 5, -2), UnitRange(3)).Length());

    auto newRange4 = Ranges::FitAndCompose(Range(10, 5, -2), UnitRange(1));
    ASSERT_EQ(1, newRange4.Length());
    ASSERT_EQ(8, newRange4.Get(0));
  }

  TEST(RangeTest, TestFitAndCompose_FromRange_SliceRange) {
    Range current = Range(5, 10);
    ASSERT_EQ(5, current.Length());

    auto newRange = Ranges::FitAndCompose(current, Range(1, 4));
    ASSERT_EQ(3, newRange.Length());
    ASSERT_EQ(6, newRange.Get(0));
    ASSERT_EQ(8, newRange.Get(2));
    auto newRange2 = Ranges::FitAndCompose(current, Range(-1, 6));
    ASSERT_EQ(5, newRange2.Length());
    ASSERT_EQ(5, newRange2.Get(0));
    ASSERT_EQ(9, newRange2.Get(4));
    auto newRange3 = Ranges::FitAndCompose(current, Range(1, 5, 2));
    ASSERT_EQ(2, newRange3.Length());
    ASSERT_EQ(6, newRange3.Get(0));
    ASSERT_EQ(8, newRange3.Get(1));

    ASSERT_EQ(1, Ranges::FitAndCompose(current, Range(4,5)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, Range(5,6)).Length());
    ASSERT_EQ(0, Ranges::FitAndCompose(current, Range(6,7)).Length());

    ASSERT_EQ(0, Ranges::FitAndCompose(Range(1, 1), Range(0, 10)).Length());

    auto newRange4 = Ranges::FitAndCompose(Range(10, 5, -2), Range(-1, 4, 2));
    ASSERT_EQ(2, newRange4.Length());
    ASSERT_EQ(10, newRange4.Get(0));
    ASSERT_EQ(6, newRange4.Get(1));

    auto newRange5 = Ranges::FitAndCompose(Range(10, 5, -2), Range(1, 4));
    ASSERT_EQ(2, newRange5.Length());
    ASSERT_EQ(8, newRange5.Get(0));
    ASSERT_EQ(6, newRange5.Get(1));
  }

} // math
} // mdl
