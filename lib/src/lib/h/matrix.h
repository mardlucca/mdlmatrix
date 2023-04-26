#ifndef _MDL_MATH_MATRIX
#define _MDL_MATH_MATRIX

#include <sstream>

#include "typedefs.h"
#include "basematrix.h"
#include "slice.h"
#include "range.h"
#include "accessor.h"
#include "operation.h"

namespace mdl {
namespace math {

namespace metal {
  class MatrixImpl;
  class MatrixReflexiveImpl;
}
namespace multithread {
  class MatrixImpl;
  class MatrixReflexiveImpl;
}
namespace singlethread {
  class MatrixImpl;
  class MatrixReflexiveImpl;
}

class Matrix : public BaseMatrix {
  public:
    Matrix();
    explicit Matrix(float_t value);
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, float_t data[]);
    Matrix(size_t rows, size_t cols, std::function<float_t (size_t, size_t)> initFn);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    Matrix(const BaseMatrix& other);

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    Matrix& operator=(const BaseMatrix& other);

    Matrix& operator+=(const Matrix& other);
    Matrix& operator+=(float_t scalar);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator-=(float_t scalar);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator*=(float_t scalar);
    Matrix& operator/=(const Matrix& other);
    Matrix& operator/=(float_t scalar);

    inline float_t& operator()(size_t row, size_t col) override {
      return data.get()[row * cols + col];
    }
    inline float_t operator()(size_t row, size_t col) const override {
      return data.get()[row * cols + col];
    }
    
#ifdef _MDL_MATH_LARGE_MATRICES
    inline float_t& operator()(size_t row, int col) override {
      return data.get()[row * cols + col];
    }
    inline float_t operator()(size_t row, int col) const override {
      return data.get()[row * cols + col];
    }
    inline float_t& operator()(int row, size_t col) override {
      return data.get()[row * cols + col];
    }
    inline float_t operator()(int row, size_t col) const override {
      return data.get()[row * cols + col];
    }
    inline float_t& operator()(int row, int col) override {
      return data.get()[row * cols + col];
    }
    inline float_t operator()(int row, int col) const override {
      return data.get()[row * cols + col];
    }
#endif

    template<typename FRR, typename FCR>
    auto operator()(const FRR& slicedRows, const FCR& slicedCols) {
      // Indidentaly this compiles because FitAndCompose(LeftRange, X) always results in 
      //   type X being returned. 
      return Slice<FRR, FCR, DirectAccessor>(
          data, cols,
          Ranges::FitAndCompose(LeftRange(rows), slicedRows), 
          Ranges::FitAndCompose(LeftRange(cols), slicedCols)); 
    }

    template<typename FRR, typename FCR>
    auto operator()(const FRR& slicedRows, const FCR& slicedCols) const {
      // Indidentaly this compiles because FitAndCompose(LeftRange, X) always results in 
      //   type X being returned.
      return Slice<FRR, FCR, DirectAccessor>(
          data, cols,
          Ranges::FitAndCompose(LeftRange(rows), slicedRows), 
          Ranges::FitAndCompose(LeftRange(cols), slicedCols)); 
    }

    auto operator()(const Range& slicedRows, const Range& slicedCols) {
      return Slice<Range, Range, DirectAccessor>(
          data, cols,
          Ranges::FitAndCompose(LeftRange(rows), slicedRows), 
          Ranges::FitAndCompose(LeftRange(cols), slicedCols)); 
    }

    Slice<LeftRange, LeftRange, DirectAccessor> operator()();
    const Slice<LeftRange, LeftRange, DirectAccessor> operator()() const;

    Matrix Transpose() const;

    inline size_t NumRows() const override { return rows; }
    inline size_t NumCols() const override { return cols; }
    inline size_t NumCells() const override { return rows * cols; }

    
    std::ostream& operator<<(std::ostream& os);

  protected:
    size_t rows;
    size_t cols;
    std::shared_ptr<float_t[]> data;

    friend class metal::MatrixImpl;
    friend class multithread::MatrixImpl;
    friend class singlethread::MatrixImpl;
    friend class metal::MatrixReflexiveImpl;
    friend class multithread::MatrixReflexiveImpl;
    friend class singlethread::MatrixReflexiveImpl;
    friend Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);

    friend Matrix Pack(const std::vector<Matrix>& matrices);
    friend std::vector<Matrix> Unpack(
        const Matrix& matrix, 
        const std::vector<std::pair<size_t, size_t>>& sizes);
    
    template <class It>
    friend void SaveMtx(const char* fileName, It begin, It end);
};

typedef std::function<Matrix (const Matrix&)> mtxtransf;

} // math
} // mdl

#endif