#ifndef _MDL_MATH_MATRIX
#define _MDL_MATH_MATRIX

#include <sstream>

#include "float_t.h"
#include "basematrix.h"
#include "slice.h"
#include "range.h"
#include "accessor.h"
#include "operation.h"

namespace mdl {
namespace math {

namespace impl {
  class SingleThreaded;
  class MultiThreaded;
}

class Matrix : public BaseMatrix {
  public:
    Matrix();
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, float_t data[]);
    Matrix(int rows, int cols, std::function<float_t (int, int)> initFn);
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

    inline float_t& operator()(int row, int col) override {
      return data.get()[row * cols + col];
    }
    inline float_t operator()(int row, int col) const override {
      return data.get()[row * cols + col];
    }
    
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

    inline int NumRows() const override { return rows; }
    inline int NumCols() const override { return cols; }
    
    std::ostream& operator<<(std::ostream& os);

    template <typename Operation> 
    void ReflexiveOperate(float_t scalar) {
      int length = rows * cols;
      float_t * cell = data.get();
      for (int i = 0; i < length; i++) {
        Operation::operate(*cell, scalar);
        cell++;
      }
    }

    template <typename Operation>
    void ReflexiveOperate(const Matrix& other) {
      int rows2 = other.rows;
      int cols2 = other.cols;

      if ((rows == rows2 && (cols == cols2 || cols2 == 1))
          || (cols == cols2 && rows2 == 1)
          || (rows2 == 1 && cols2 == 1)) {


        float_t * thisBuffer = data.get();
        float_t * otherBuffer = other.data.get();
        for (int row = 0; row < rows; row++) {
          for (int col = 0; col < cols; col++) {
            Operation::operate(
                thisBuffer[row * cols + col], 
                otherBuffer[(rows2 == 1 ? 0 : row) * cols2 + (cols2 == 1 ? 0 : col)]);
          }
        }
      } else {
        std::ostringstream os;
        os << "Cannot operate on matrices of different dimensions: " << rows << 'x' << cols
            << " and " << rows2 << 'x' << cols2;
        throw std::invalid_argument(os.str());
      }
    }

    template <typename Operation>
    void ReflexiveUnaryOperate() {
      int length = rows * cols;
      float_t * cell = data.get();
      for (int i = 0; i < length; i++) {
        Operation::operate(*cell);
        cell++;
      }
    }    

  protected:
    int rows;
    int cols;
    std::shared_ptr<float_t[]> data;

    friend class impl::MultiThreaded;
    friend class impl::SingleThreaded;
    friend Matrix operator*(const Matrix& matrix1, const Matrix& matrix2);

    friend Matrix Pack(const std::vector<Matrix>& matrices);
    friend std::vector<Matrix> Unpack(
        const Matrix& matrix, 
        const std::vector<std::pair<int, int>>& sizes);
    friend void SaveMtx(const char* fileName, const std::vector<Matrix>& mats);
};

typedef std::function<Matrix (const Matrix&)> mtxtransf;

} // math
} // mdl

#endif