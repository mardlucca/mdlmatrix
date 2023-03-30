#ifndef _MDL_MATH_METAL_MATRIX_IMPL
#define _MDL_MATH_METAL_MATRIX_IMPL

#include <algorithm>
#include <mdl/compute.h>
#include <mdl/profiler.h>

#include "../matrix.h"
#include "engine.h"

namespace mdl {
namespace math {
namespace metal {
  using mdl::compute::in;
  using mdl::compute::out;
  using mdl::compute::priv;

  class LibraryLoader;
  extern LibraryLoader libraryLoader;

  class MatrixImpl {
    public:
      inline static Matrix Multiply(const Matrix& matrix1, const Matrix& matrix2);

      inline static Matrix Transpose(const Matrix& matrix);
  };

  inline Matrix MatrixImpl::Transpose(const Matrix& matrix) {
    size_t rows = matrix.NumRows();
    size_t cols = matrix.NumCols();
    size_t buffSize = rows * cols * sizeof(float_t);

    float_t * sdata = matrix.data.get();
    float_t * data = new float_t[rows * cols];

    Engine::Get()->NewBatch()
        .WithGrid(matrix.NumRows(), matrix.NumCols(), 2, matrix.NumCols())
        .Call("Transpose", in(sdata, buffSize), out(data, buffSize), in(rows))
        .Dispatch()
        .Wait();

    return Matrix(cols, rows, data);
  }

  inline Matrix MatrixImpl::Multiply(const Matrix& matrix1, const Matrix& matrix2) {
    size_t rows1 = matrix1.NumRows();
    size_t cols2 = matrix2.NumCols();

    float_t * m1Data = matrix1.data.get();
    float_t * m2Data = matrix2.data.get();
    float_t * data = new float_t[rows1 * cols2];
    auto transp = priv(matrix2.NumCells() * sizeof(float_t));

    Engine::Get()->NewBatch()
        .WithGrid(matrix2.NumRows(), cols2, 1, std::min(512, cols2))
        .Call("Transpose", in(m2Data, transp.size), transp, matrix2.NumRows())
        .WithGrid(rows1, cols2, 1, std::min(512, cols2))
        .Call("Multiply", 
            in(m1Data, matrix1.NumCells() * sizeof(float_t)), 
            transp, 
            matrix1.NumCols(), 
            out(data, rows1 * cols2 * sizeof(float_t)))
        .Dispatch().Wait();

    return Matrix(rows1, cols2, data);
  }

} // namespace metal
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_METAL_MATRIX_IMPL