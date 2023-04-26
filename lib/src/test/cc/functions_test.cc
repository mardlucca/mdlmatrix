#include <gtest/gtest.h>

#include <mdl/matrix.h>
#include <mdl/io.h>

namespace mdl {
namespace math {

  TEST(MatrixFunctionsTest, AbsTest) {
    Matrix mat = Matrices::Sequence(2,2,Range(-1, -5, -1));
    mat.ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0);
    });
    Abs(mat).ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val > 0);
    });
    mat.ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0);
    });
  }

  TEST(MatrixFunctionsTest, CeilTest) {
    ASSERT_TRUE(BaseMatrix::Equals(
      Ceil(Matrices::WithValues(1, {1, 1.49, 1.5, 1.51})),
      Matrices::WithValues(1, {1, 2, 2, 2})));
  }

  TEST(MatrixFunctionsTest, FloorTest) {
    ASSERT_TRUE(BaseMatrix::Equals(
      Floor(Matrices::WithValues(1, {1, 1.49, 1.5, 1.51, 2})),
      Matrices::WithValues(1, {1, 1, 1, 1, 2})));
  }

  TEST(MatrixFunctionsTest, RoundTest) {
    ASSERT_TRUE(BaseMatrix::Equals(
      Round(Matrices::WithValues(1, {1, 1.49, 1.5, 1.51, 2})),
      Matrices::WithValues(1, {1, 1, 2, 2, 2})));
  }

  TEST(MatrixFunctionsTest, ExpTest) {
    float_t e = 2.71828;
    Matrix m1 = Exp(Matrices::WithValues(1, {0,1,2}));
    Matrix expected = Matrices::WithValues(1, {1, e, e * e});

    (m1 - expected).ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0.001);
    });
  }

  TEST(MatrixFunctionsTest, LogTest) {
    Matrix expected = Matrices::WithValues(1, {0,1,2});
    ASSERT_EQ(3.0, Sum(expected == Log(Exp(expected)))(0,0));
  }

  TEST(MatrixFunctionsTest, Log2Test) {
    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {0,1,2}),
      Log2(Matrices::WithValues(1, {1,2,4}))));
  }

  TEST(MatrixFunctionsTest, Log10Test) {
    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {0,1,2}),
      Log10(Matrices::WithValues(1, {1,10,100}))));
  }

  TEST(MatrixFunctionsTest, SinTest) {}
  TEST(MatrixFunctionsTest, CosTest) {}
  TEST(MatrixFunctionsTest, TanTest) {}
  TEST(MatrixFunctionsTest, AsinTest) {}
  TEST(MatrixFunctionsTest, AcosTest) {}
  TEST(MatrixFunctionsTest, AtanTest) {}

  TEST(MatrixFunctionsTest, SqrTest) {
    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {1,4,9}),
      Sqr(Matrices::WithValues(1, {1,2,3}))));
  }

  TEST(MatrixFunctionsTest, SqrtTest) {
    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {1,2,3}),
      Sqrt(Matrices::WithValues(1, {1,4,9}))));
  }

  TEST(MatrixFunctionsTest, SumTest) {
    Matrix data = Matrices::WithValues(4,
      {16, 2, 3,13,
        5,11,10, 8,
        9, 7, 6,12,
        4,14,15, 1});

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(4, {34, 34, 34, 34}),
      Sum(data)));

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {34 * 4}),
      Sum(Sum(data))));
  }

  TEST(MatrixFunctionsTest, MeanTest)  {
    Matrix data = Matrices::WithValues(4,
      {16, 2, 3,13,
        5,11,10, 8,
        9, 7, 6,12,
        4,14,15, 1});

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(4, {8.5, 8.5, 8.5, 8.5}),
      Mean(data)));

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {8.5}),
      Mean(Mean(data))));
  }

  TEST(MatrixFunctionsTest, MaxTest)  {
    Matrix data = Matrices::WithValues(4,
      {16, 2, 3,13,
        5,11,10, 8,
        9, 7, 6,12,
        4,14,15, 1});

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(4, {16, 14, 15, 13}),
      Max(data)));

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {16}),
      Max(Max(data))));
  }

  TEST(MatrixFunctionsTest, MinTest)  {
    Matrix data = Matrices::WithValues(4,
      {16, 2, 3,13,
        5,11,10, 8,
        9, 7, 6,12,
        4,14,15, 1});

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(4, {4, 2, 3, 1}),
      Min(data)));

    ASSERT_TRUE(BaseMatrix::Equals(
      Matrices::WithValues(1, {1}),
      Min(Min(data))));
  }

  TEST(MatrixFunctionsTest, VarianceTest) {
    Matrix data = Matrices::WithValues(4,
      {16, 2, 3,13,
        5,11,10, 8,
        9, 7, 6,12,
        4,14,15, 1});

    Matrix expected = Matrices::WithValues(4, {29.6667, 27, 27, 29.6667});
    (Variance(data) - expected).ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0.001);
    });

    expected = Matrices::WithValues(1, {2.3704});
    (Variance(Variance(data)) - expected).ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0.001);
    });

    expected = Matrices::WithValues(1, {0});
    (Variance(Variance(Variance(data))) - expected).ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0.001);
    });
  }

  TEST(MatrixFunctionsTest, StdDevTest) {
    Matrix data = Matrices::WithValues(4,
      {16, 2, 3,13,
        5,11,10, 8,
        9, 7, 6,12,
        4,14,15, 1});

    Matrix expected = Matrices::WithValues(4, {5.4467, 5.1962, 5.1962, 5.4467});
    (StdDev(data) - expected).ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0.001);
    });

    expected = Matrices::WithValues(1, {0.1447});
    (StdDev(StdDev(data)) - expected).ForEach([](size_t row, size_t col, float_t& val) {
      ASSERT_TRUE(val < 0.001);
    });
  }

  TEST(MatrixFunctionsTest, ProdTest) {
     Matrix data = Matrices::Sequence(1, 4, Range(1));
     Matrix result = Prod(data, data);
     ASSERT_TRUE(result.Equals(data ^ 2));
  }

  TEST(MatrixFunctionsTest, DotProdTest) {
     Matrix data = Matrices::Sequence(1, 4, Range(1));
     ASSERT_EQ(30, DotProd(data, data));
  }

  TEST(MatrixFunctionsTest, DotProdTest_Fail) {
     Matrix data = Matrices::Sequence(1, 4, Range(1));
     Matrix data2 = Matrices::Sequence(1, 5, Range(1));
     ASSERT_THROW(DotProd(data, data2), std::invalid_argument);
  }

  TEST(MatrixFunctionsTest, Sigmoid) {
     Matrix data = Matrices::WithValues(5, {-1000, -1, 0, 1, 1000});
     Matrix expected = Matrices::WithValues(5, {0, 269, 500, 731, 1000});
     ASSERT_TRUE(expected.Equals(Round(Sigmoid(data) * 1000)));
  }

  TEST(MatrixFunctionsTest, SigmoidGradient) {
    Matrix values = Matrices::WithValues(1, {-1, -0.5, 0, 0.5, 1});
    Matrix grad = SigmoidGradient(values);
    Matrix expected = Matrices::WithValues(1, {0.196612, 0.235004, 0.25, 0.235004, 0.196612});

    ASSERT_EQ(5, Sum(mdl::math::Abs(expected - grad) < 0.0001) (0,0));
  }

  TEST(MatrixFunctionsTest, TestPackUnpack) {
    std::vector<Matrix> matrices;
    matrices.push_back(Matrices::Sequence(3, 3, Range(1)));
    matrices.push_back(Matrices::Sequence(2, 2, Range(10)));
    matrices.push_back(Matrices::Sequence(1, 1, Range(14)));

    Matrix result = Pack(matrices);
    ASSERT_EQ(1, result.NumRows());
    ASSERT_EQ(14, result.NumCols());

    ASSERT_TRUE(result.Equals(Matrices::Sequence(1, 14, Range(1))));

    std::vector<Matrix> unpacked =
        Unpack(result, {{3,3}, {2,2}, {1,1}});

    ASSERT_TRUE(matrices[0].Equals(unpacked[0]));
    ASSERT_TRUE(matrices[1].Equals(unpacked[1]));
    ASSERT_TRUE(matrices[2].Equals(unpacked[2]));
  }

  TEST(MatrixFunctionsTest, TestFromMtx_DoublePrecision) {
    Matrix m = FromMtx("lib/src/test/resources/matrix/mat_seq_4_5_double.mtx")[0];
    Matrix expected = Matrices::Sequence(4, 5, Range(1));
    ASSERT_TRUE(m.Equals(expected));
  }

  TEST(MatrixFunctionsTest, TestFromMtx_SinglePrecision) {
    Matrix m = FromMtx("lib/src/test/resources/matrix/mat_seq_4_5_single.mtx")[0];
    Matrix expected = Matrices::Sequence(4, 5, Range(1));
    ASSERT_TRUE(m.Equals(expected));
  }

  TEST(MatrixFunctionsTest, TestSaveMtx) {
    Matrix m = Matrices::Sequence(10, 10, 100);
    SaveMtx("/tmp/MatricesTestSuite_TestSaveMtx.mtx", m);
    std::vector<Matrix> matrices = FromMtx("/tmp/MatricesTestSuite_TestSaveMtx.mtx");
    ASSERT_EQ(1, matrices.size());
    ASSERT_TRUE(m.Equals(matrices[0]));
  }

  TEST(MatrixFunctionsTest, TestSaveMtx_Vector) {
    Matrix m = Matrices::Sequence(10, 10, 100);
    Matrix m2 = Matrices::Ones(5000, 400);
    std::vector<Matrix> v({m, m2});
    SaveMtx("/tmp/MatricesTestSuite_TestSaveMtx.mtx", v.begin(), v.end());
    std::vector<Matrix> matrices = FromMtx("/tmp/MatricesTestSuite_TestSaveMtx.mtx");
    ASSERT_EQ(2, matrices.size());
    ASSERT_TRUE(m.Equals(matrices[0]));
    ASSERT_TRUE(m2.Equals(matrices[1]));
  }

  TEST(MatrixFunctionsTest, TestFromMtx_WrongFormat) {
    ASSERT_THROW(
      FromMtx("lib/src/test/resources/matrix/MatricesTestSuite_FromCsv.csv"),
      std::runtime_error);
  }

  TEST(MatrixFunctionsTest, TestFromMtx_NotFound) {
    ASSERT_THROW(
      FromMtx("lib/src/test/resources/matrix/bogus.mtx"),
      mdl::io::file_not_found_exception);
  }

  // TEST(MatrixFunctionsTest, TestSaveMtx_Empty) {
  //   Matrix m = Matrices::Sequence(10, 10, 100);
  //   Matrix m2;
  //   SaveMtx("/tmp/MatricesTestSuite_TestSaveMtx.mtx", std::vector<Matrix>({m, m2, m}));
  //   std::vector<Matrix> matrices = FromMtx("/tmp/MatricesTestSuite_TestSaveMtx.mtx");
  //   ASSERT_EQ(3, matrices.size());
  //   ASSERT_TRUE(m.Equals(matrices[0]));
  //   ASSERT_TRUE(m2.Equals(matrices[1]));
  //   ASSERT_TRUE(m.Equals(matrices[2]));
  // }

} // math
} // mdl