#ifndef _MDL_MATH_MATRIX_FUNCTIONS
#define _MDL_MATH_MATRIX_FUNCTIONS

#include "typedefs.h"
#include "matrix.h"
#include "slice.h"
#include "basematrix_operator_overload.h"
#include "matrix_operator_overload.h"

#include <memory>
#include <vector>
#include <fstream>
#include <mdl/io.h>

namespace mdl {
namespace math {
  const int kMtxFileMark = 0x11080101;
  const int kDoublePrecisionBit = 0x01;

  Matrix Abs(const BaseMatrix& matrix);
  Matrix Ceil(const BaseMatrix& matrix);
  Matrix Floor(const BaseMatrix& matrix);
  Matrix Round(const BaseMatrix& matrix);

  Matrix Exp(const BaseMatrix& matrix);
  Matrix Log(const BaseMatrix& matrix);
  Matrix Log2(const BaseMatrix& matrix);
  Matrix Log10(const BaseMatrix& matrix);

  Matrix Sin(const BaseMatrix& matrix);
  Matrix Cos(const BaseMatrix& matrix);
  Matrix Tan(const BaseMatrix& matrix);
  Matrix Asin(const BaseMatrix& matrix);
  Matrix Acos(const BaseMatrix& matrix);
  Matrix Atan(const BaseMatrix& matrix);

  Matrix Sqr(const BaseMatrix& matrix);
  Matrix Sqrt(const BaseMatrix& matrix);

  Matrix Sum(const BaseMatrix& matrix);
  Matrix Mean(const BaseMatrix& matrix);
  Matrix Min(const BaseMatrix& matrix);
  Matrix Max(const BaseMatrix& matrix);
  Matrix Variance(const BaseMatrix& matrix);
  Matrix StdDev(const BaseMatrix& matrix);

  Matrix Prod(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  float_t DotProd(const BaseMatrix& matrix1, const BaseMatrix& matrix2);

  Matrix Sigmoid(const BaseMatrix& matrix);
  Matrix SigmoidGradient(const Matrix& matrix);
  Matrix ReLU(const Matrix& matrix);
  Matrix ReLUGradient(const Matrix& matrix);
  Matrix Identity(const Matrix& matrix);
  Matrix IdentityGradient(const Matrix& matrix);

  Matrix Pack(const std::vector<Matrix>& matrices);
  std::vector<Matrix> Unpack(
      const Matrix& matrix, 
      const std::vector<std::pair<size_t, size_t>>& sizes);

  std::vector<Matrix> FromMtx(std::istream& in);
  void FromMtx(std::istream& in, std::function<bool (Matrix&&)> consumer);
  std::function<std::unique_ptr<Matrix> ()> FromMtxStream(std::istream* in);
  std::vector<Matrix> FromMtx(const char* fileName);
  void FromMtx(const char* fileName, std::function<bool (Matrix&&)> consumer);
  std::function<std::unique_ptr<Matrix> ()> FromMtxStream(const char* fileName);

  template <class It>
  void SaveMtx(std::ostream& out, It begin, It end);  
  void SaveMtx(std::ostream& out, std::function<const Matrix* ()> supplier);
  void SaveMtx(std::ostream& out, const Matrix& matrix);
  template <class It>
  void SaveMtx(const char* fileName, It begin, It end);
  void SaveMtx(const char* fileName, std::function<const Matrix* ()> supplier);
  void SaveMtx(const char* fileName, const Matrix& matrix);

  void SaveCsv(const char* fileName, const Matrix& matrix);
  void SaveCsv(std::ostream& out, const Matrix& matrix);


  // IMPLEMENTATIONS

  template <class It>
  void SaveMtx(const char* fileName, It begin, It end) {
    try {
      std::ofstream out(fileName, std::ios_base::binary | std::ios_base::out);
      out.exceptions(std::ios::badbit | std::ios::failbit);

      SaveMtx(out, begin, end);
    } catch (std::ios_base::failure& ex) {
      throw mdl::util::exceptionstream()
        .Append("Cannot write to file: ")
        .Append(fileName)
        .Build<mdl::io::io_exception>();
    }
  }

  template <class It>
  void SaveMtx(std::ostream& out, It begin, It end) {
    out.write(reinterpret_cast<const char *>(&kMtxFileMark), sizeof(int));

    int controlReg = 0;
    if (kDoublePrecision) {
      controlReg |= kDoublePrecisionBit;
    }

    out.write(reinterpret_cast<const char *>(&controlReg), sizeof(controlReg));

    for (It mat = begin; mat != end; ++mat) {
      int rows = mat->rows;
      int cols = mat->cols;
      out.write(reinterpret_cast<const char *>(&rows), sizeof(int));
      out.write(reinterpret_cast<const char *>(&cols), sizeof(int));
      if (mat->rows * mat->cols > 0) {
        out.write(reinterpret_cast<const char *>(mat->data.get()), 
            sizeof(float_t) * mat->rows * mat->cols);
      }
    }
  }
} // math
} // mdl

#endif //_MDL_MATH_MATRIX_FUNCTIONS