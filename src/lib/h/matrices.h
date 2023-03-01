#ifndef _MDL_MATH_MATRICES
#define _MDL_MATH_MATRICES

#include <random>
#include <vector>

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
      static Matrix Default(int rows, int cols, double def);
      static Matrix WithValues(int cols, const std::vector<double>& values);

      static Matrix FromCsv(const char* fileName, bool ignoreFirstLine = false);
      
      static Matrix Biased(const BaseMatrix& matrix);
    private:
      static std::random_device rd;
      static std::mt19937 gen;
      static std::uniform_real_distribution<double> distribution;
  };

} // math
} // mdl

#endif //_MDL_MATH_MATRICES