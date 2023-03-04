#ifndef _MDL_MATH_MATRICES
#define _MDL_MATH_MATRICES

#include <random>
#include <vector>

#include "float_t.h"
#include "matrix.h"

namespace mdl {
namespace math {

  class Matrices {
    public:
      static Matrix Identity(int length);
      static Matrix Ones(int rows, int cols);
      static Matrix Zeros(int rows, int cols);
      static Matrix Random(int rows, int cols);
      static Matrix Sequence(int rows, int cols, int length);
      static Matrix Sequence(int rows, int cols, Range range);
      static Matrix Default(int rows, int cols, float_t def);
      static Matrix WithValues(int cols, const std::vector<float_t>& values);

      static Matrix FromCsv(const char* fileName, bool ignoreFirstLine = false);
      
      static Matrix Biased(const BaseMatrix& matrix);
    private:
      static std::random_device rd;
      static std::mt19937 gen;
      static std::uniform_real_distribution<float_t> distribution;
  };

} // math
} // mdl

#endif //_MDL_MATH_MATRICES