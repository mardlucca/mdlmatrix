#ifndef _MDL_MATH_METAL_MATRIXIMPL
#define _MDL_MATH_METAL_MATRIXIMPL

#include <mdl/compute.h>
#include <mdl/profiler.h>

#include "../matrix.h"
#include "engine.h"

namespace mdl {
namespace math {
namespace metal {
  using mdl::compute::in;
  using mdl::compute::out;

  using std::cout;
  using std::endl;

  class LibraryLoader;
  extern LibraryLoader libraryLoader;

  class MatrixImpl {
    public:

      inline static Matrix Transpose(const Matrix& matrix) {
        size_t rows = matrix.NumRows();
        size_t cols = matrix.NumCols();
        size_t buffSize = rows * cols * sizeof(float_t);

        float_t * sdata = matrix.data.get();
        float_t * data = new float_t[rows * cols];

        Engine::Get()->NewBatch()
            .WithGrid(matrix.NumRows(), matrix.NumCols(), 2, matrix.NumCols())
            .Call("Transpose", in(sdata, buffSize), out(data, buffSize), rows)
            .Dispatch()
            .Wait();

        return Matrix(cols, rows, data);
      }

  };
} // namespace metal
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_METAL_MATRIXIMPL