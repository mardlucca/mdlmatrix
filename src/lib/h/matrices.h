#ifndef _MDL_MATH_MATRICES
#define _MDL_MATH_MATRICES

#include <random>
#include <vector>

#include "typedefs.h"
#include "matrix.h"

namespace mdl {
namespace math {

  class Matrices {
    public:
      static Matrix Identity(size_t length);
      static Matrix Ones(size_t rows, size_t cols);
      static Matrix Zeros(size_t rows, size_t cols);
      static Matrix Random(size_t rows, size_t cols);
      static Matrix Sequence(size_t rows, size_t cols, size_t length);
      static Matrix Sequence(size_t rows, size_t cols, Range range);
      static Matrix Default(size_t rows, size_t cols, float_t def);
      static Matrix WithValues(size_t cols, const std::vector<float_t>& values);

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