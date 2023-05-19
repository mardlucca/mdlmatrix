#include "../h/functions.h"

#include <climits>
#include <vector>
#include <fstream>

#include "../h/matrix.h"
#include "../h/matrices.h"
#include "../h/operation.h"
#include "../h/singlethread/basematrix_impl.h"
#include "../h/multithread/basematrix_impl.h"
#include <mdl/io.h>
#include <mdl/util.h>

namespace mdl {
namespace math {
  using multithread::BaseMatrixImpl;

  Matrix Abs(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Abs>(matrix);
  }
  
  Matrix Ceil(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Ceil>(matrix);
  }

  Matrix Floor(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Floor>(matrix);
  }

  Matrix Round(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Round>(matrix);
  }


  Matrix Exp(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Exp>(matrix);
  }

  Matrix Log(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Log>(matrix);
  }

  Matrix Log2(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Log2>(matrix);
  }

  Matrix Log10(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Log10>(matrix);
  }


  Matrix Sin(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Sin>(matrix);
  }

  Matrix Cos(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Cos>(matrix);
  }

  Matrix Tan(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Tan>(matrix);
  }

  Matrix Asin(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Asin>(matrix);
  }

  Matrix Acos(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Acos>(matrix);
  }

  Matrix Atan(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Atan>(matrix);
  }


  Matrix Sqr(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Sqr>(matrix);
  }

  Matrix Sqrt(const BaseMatrix& matrix) {
    return BaseMatrixImpl::UnaryOperate<op::Sqrt>(matrix);
  }


  Matrix Sum(const BaseMatrix& matrix) {
    return BaseMatrixImpl::Reduce<op::Addition>(matrix);
  }

  Matrix Mean(const BaseMatrix& matrix) {
    float_t div = matrix.NumRows() > 1 ? matrix.NumRows() : matrix.NumCols();
    return Sum(matrix) / div;
  }

  Matrix Max(const BaseMatrix& matrix) {
    return BaseMatrixImpl::Reduce<op::Max>(matrix, -std::numeric_limits<float_t>::infinity());
  }

  Matrix Min(const BaseMatrix& matrix) {
    return BaseMatrixImpl::Reduce<op::Min>(matrix, std::numeric_limits<float_t>::infinity());
  }

  Matrix Variance(const BaseMatrix& matrix) {
    float_t div = matrix.NumRows() > 1 ? matrix.NumRows() : matrix.NumCols();
    return Sum((matrix - Mean(matrix)) ^ 2) / (div > 1 ? div - 1 : 1);
  }

  Matrix StdDev(const BaseMatrix& matrix) {
    return Sqrt(Variance(matrix));
  }

  Matrix Prod(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return BaseMatrixImpl::Operate<op::Multiplication>(matrix1, matrix2);
  }

  float_t DotProd(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    size_t rows = matrix1.NumRows();
    size_t cols = matrix1.NumCols();
    
    if (rows != matrix2.NumRows() || cols != matrix2.NumCols()) {
      std::ostringstream os;
      os << "Cannot dot product matrices of different dimensions: " 
          << rows << 'x' << cols
          << " and " << matrix2.NumCols() << 'x' << matrix2.NumRows();
      throw std::invalid_argument(os.str());
    }

    float_t result = 0.0;
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        result += matrix1(row, col) * matrix2(row, col);
      }
    }

    return result;
  }

  Matrix Sigmoid(const BaseMatrix& matrix) {
    return 1.0 / (1 + mdl::math::Exp(-matrix));
  }

  Matrix SigmoidGradient(const Matrix& matrix) {
    Matrix sigmoid = Sigmoid(matrix);
    return Prod(sigmoid, 1 - sigmoid);
  }

  Matrix ReLU(const Matrix& matrix) {
    return Prod(matrix, (matrix > 0));
  }  

  Matrix ReLUGradient(const Matrix& matrix) {
    return matrix > 0;
  }

  Matrix Identity(const Matrix& matrix) {
    return matrix;
  }  

  Matrix IdentityGradient(const Matrix& matrix) {
    return Matrices::Ones(matrix.NumRows(), matrix.NumCols());
  }

  Matrix Pack(const std::vector<Matrix>& matrices) {
    size_t size = 0;
    for (auto it = matrices.begin(); it != matrices.end(); it++) {
      size += it->NumRows() * it->NumCols();
    }

    float_t * data = new float_t[size];

    float_t * runner = data;
    for (auto it = matrices.begin(); it != matrices.end(); it++) {
      size_t numCells = it->NumRows() * it->NumCols();
      std::memcpy(runner, it->data.get(), numCells * sizeof(float_t));
      runner += numCells;
    }

    return Matrix(1, size, data);
  }

  std::vector<Matrix> Unpack(
      const Matrix& matrix, 
      const std::vector<std::pair<size_t, size_t>>& sizes) {

    std::vector<Matrix> matrices;

    float_t * runner = matrix.data.get();
    for (auto it = sizes.begin(); it != sizes.end(); it++) {
      size_t numCells = it->first * it->second;
      float_t * data = new float_t[numCells];
      std::memcpy(data, runner, numCells * sizeof(float_t));
      matrices.push_back(Matrix(it->first, it->second, data));
      runner += numCells;
    }

    return matrices;
  }

  std::vector<Matrix> FromMtx(const char* fileName) {
    std::vector<Matrix> vec;
    FromMtx(fileName, [&vec](Matrix&& mat) {
      vec.push_back(std::move(mat));
      return true;
    });

    return vec;
  }

  void FromMtx(const char* fileName, std::function<bool (Matrix&&)> consumer) {
    auto supplier = FromMtxStream(fileName);
    for (auto pMatrix = supplier(); pMatrix.get(); pMatrix = supplier()) {
      consumer(std::move(*pMatrix));
    }
  }

  std::function<std::unique_ptr<Matrix> ()> FromMtxStream(const char* fileName) {
    // std::function's must be copy constructible, so whatever lambdas they point to must also be
    // copy constructible. Ideally we'd want to use move semantics on the input stream and simply
    // move the object around in-between lambdas. This doesn't work because ifstream is not copy
    // constructible, and capturing an ifstream by value makes the lambda not copy constructible 
    // too. This is a no no. C++23 now introduces std::move_only_function, which presumably solves
    // this. Not also that capturing ifstream by reference doesn't work either as the reference 
    // becomes dangling after this method returns.

    std::shared_ptr<std::ifstream> in(new std::ifstream(fileName, std::ios_base::binary));

    if (!in || !in->is_open()) {
      throw mdl::util::exceptionstream()
        .Append("Could not open file: ").Append(fileName)
        .Build<mdl::io::file_not_found_exception>();
    }
    
    in->exceptions(std::ios::badbit);

    int controlReg = 0;
    try {
      int mask;
      in->read(reinterpret_cast<char *>(&mask), sizeof(int));
      if (mask != kMtxFileMark) {
        throw mdl::util::exceptionstream()
            .Append("File does not appear to contain matrices: ").Append(fileName)
            .Build();
      }

      in->read(reinterpret_cast<char *>(&controlReg), sizeof(int));
    } catch (const std::ios_base::failure& e) {
      throw mdl::io::io_exception(e.what());
    }

    return [in, controlReg]() {
      try {
        if (in->eof()) { return std::unique_ptr<Matrix>(); }

        int rows;
        int cols;
        in->read(reinterpret_cast<char *>(&rows), sizeof(int));
        if (in->eof()) { return std::unique_ptr<Matrix>(); }
        in->read(reinterpret_cast<char *>(&cols), sizeof(int));


        Matrix matrix;
        if (rows * cols > 0) {
          if (controlReg & kDoublePrecisionBit) {
            std::unique_ptr<double_t> buffer(new double_t[((size_t) rows) * cols]);
            in->read(reinterpret_cast<char *>(buffer.get()), sizeof(double_t) * rows * cols);

            if (kDoublePrecision) {
              matrix = Matrix(rows, cols, reinterpret_cast<float_t *>(buffer.release()));
            } else {
              matrix = Matrix(rows, cols, 
                reinterpret_cast<float_t *>(
                  ToSingleBuff(buffer.get(), ((size_t) rows) * cols)));
            }
          } else {
            std::unique_ptr<single_t> buffer(new single_t[((size_t) rows) * cols]);
            in->read(reinterpret_cast<char *>(buffer.get()), sizeof(single_t) * rows * cols);

            if (kDoublePrecision) {
              matrix = Matrix(rows, cols, 
                reinterpret_cast<float_t *>(
                  ToDoubleBuff(buffer.get(), ((size_t) rows) * cols)));
            } else {
              matrix = Matrix(rows, cols, reinterpret_cast<float_t *>(buffer.release()));
            }
          }
        } else {
          matrix = Matrix(0, 0);
        }

        return std::unique_ptr<Matrix>(new Matrix(std::move(matrix)));
      } catch (const std::ios_base::failure& e) {
        throw mdl::io::io_exception(e.what());
      }
    };
  }

  void SaveMtx(const char* fileName, std::function<const Matrix* ()> supplier) {
    mdl::util::functional::SupplierIterable<const Matrix> iterable(supplier);
    SaveMtx(fileName, iterable.begin(), iterable.end());
  }

  void SaveMtx(const char* fileName, const Matrix& matrix) {
    SaveMtx(fileName, mdl::util::functional::AsSupplier(&matrix));
  }

  void SaveCsv(const char* fileName, const Matrix& matrix) {
    try {
      std::ofstream out(fileName, std::ios_base::out);
      out.exceptions(std::ios::badbit | std::ios::failbit);

      SaveCsv(out, matrix);      
    } catch (std::ios_base::failure& ex) {
      throw mdl::util::exceptionstream()
        .Append("Cannot write to file: ")
        .Append(fileName)
        .Build<mdl::io::io_exception>();
    }
  }
  void SaveCsv(std::ostream& out, const Matrix& matrix) {
    for (size_t row = 0; row < matrix.NumRows(); row++) {
      for (size_t col = 0; col < matrix.NumCols(); col++) {
        if (col > 0) { out << ','; }
        out << matrix(row, col);
      }
      out << std::endl;
    }
  }

} // math
} // mdl