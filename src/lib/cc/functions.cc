#include "../h/functions.h"

#include <climits>
#include <vector>
#include <fstream>

#include "../h/matrix.h"
#include "../h/matrices.h"
#include "../h/operation.h"
#include "../h/basematrix_operate.h"
#include <mdl/io.h>
#include <mdl/util.h>

namespace mdl {
namespace math {
  const int kMtxFileMark = 0x11080101;

  Matrix Abs(const BaseMatrix& matrix) {
    return UnaryOperate<op::Abs>(matrix);
  }
  
  Matrix Ceil(const BaseMatrix& matrix) {
    return UnaryOperate<op::Ceil>(matrix);
  }

  Matrix Floor(const BaseMatrix& matrix) {
    return UnaryOperate<op::Floor>(matrix);
  }

  Matrix Round(const BaseMatrix& matrix) {
    return UnaryOperate<op::Round>(matrix);
  }


  Matrix Exp(const BaseMatrix& matrix) {
    return UnaryOperate<op::Exp>(matrix);
  }

  Matrix Log(const BaseMatrix& matrix) {
    return UnaryOperate<op::Log>(matrix);
  }

  Matrix Log2(const BaseMatrix& matrix) {
    return UnaryOperate<op::Log2>(matrix);
  }

  Matrix Log10(const BaseMatrix& matrix) {
    return UnaryOperate<op::Log10>(matrix);
  }


  Matrix Sin(const BaseMatrix& matrix) {
    return UnaryOperate<op::Sin>(matrix);
  }

  Matrix Cos(const BaseMatrix& matrix) {
    return UnaryOperate<op::Cos>(matrix);
  }

  Matrix Tan(const BaseMatrix& matrix) {
    return UnaryOperate<op::Tan>(matrix);
  }

  Matrix Asin(const BaseMatrix& matrix) {
    return UnaryOperate<op::Asin>(matrix);
  }

  Matrix Acos(const BaseMatrix& matrix) {
    return UnaryOperate<op::Acos>(matrix);
  }

  Matrix Atan(const BaseMatrix& matrix) {
    return UnaryOperate<op::Atan>(matrix);
  }


  Matrix Sqr(const BaseMatrix& matrix) {
    return UnaryOperate<op::Sqr>(matrix);
  }

  Matrix Sqrt(const BaseMatrix& matrix) {
    return UnaryOperate<op::Sqrt>(matrix);
  }


  Matrix Sum(const BaseMatrix& matrix) {
    return Reduce<op::Addition>(matrix);
  }

  Matrix Mean(const BaseMatrix& matrix) {
    double div = matrix.NumRows() > 1 ? matrix.NumRows() : matrix.NumCols();
    return Sum(matrix) / div;
  }

  Matrix Max(const BaseMatrix& matrix) {
    return Reduce<op::Max>(matrix, -std::numeric_limits<double>::infinity());
  }

  Matrix Min(const BaseMatrix& matrix) {
    return Reduce<op::Min>(matrix, std::numeric_limits<double>::infinity());
  }

  Matrix Variance(const BaseMatrix& matrix) {
    double div = matrix.NumRows() > 1 ? matrix.NumRows() : matrix.NumCols();
    return Sum((matrix - Mean(matrix)) ^ 2) / (div > 1 ? div - 1 : 1);
  }

  Matrix StdDev(const BaseMatrix& matrix) {
    return Sqrt(Variance(matrix));
  }

  Matrix Prod(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    return Operate<op::Multiplication>(matrix1, matrix2);
  }

  double DotProd(const BaseMatrix& matrix1, const BaseMatrix& matrix2) {
    int rows = matrix1.NumRows();
    int cols = matrix1.NumCols();
    
    if (rows != matrix2.NumRows() || cols != matrix2.NumCols()) {
      std::ostringstream os;
      os << "Cannot dot product matrices of different dimensions: " 
          << rows << 'x' << cols
          << " and " << matrix2.NumCols() << 'x' << matrix2.NumRows();
      throw std::invalid_argument(os.str());
    }

    double result = 0.0;
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
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
    int size = 0;
    for (auto it = matrices.begin(); it != matrices.end(); it++) {
      size += it->NumRows() * it->NumCols();
    }

    double * data = new double[size];

    double * runner = data;
    for (auto it = matrices.begin(); it != matrices.end(); it++) {
      int numCells = it->NumRows() * it->NumCols();
      std::memcpy(runner, it->data.get(), numCells * sizeof(double));
      runner += numCells;
    }

    return Matrix(1, size, data);
  }

  std::vector<Matrix> Unpack(
      const Matrix& matrix, 
      const std::vector<std::pair<int, int>>& sizes) {

    std::vector<Matrix> matrices;

    double * runner = matrix.data.get();
    for (auto it = sizes.begin(); it != sizes.end(); it++) {
      int numCells = it->first * it->second;
      double * data = new double[numCells];
      std::memcpy(data, runner, numCells * sizeof(double));
      matrices.push_back(Matrix(it->first, it->second, data));
      runner += numCells;
    }

    return matrices;
  }

  std::vector<Matrix> FromMtx(const char* fileName) {
    std::ifstream in(fileName, std::ios_base::binary);

    if (!in || !in.is_open()) {
      throw mdl::util::exceptionstream()
        .Append("Could not open file: ").Append(fileName)
        .Build<mdl::io::file_not_found_exception>();
    }
    
    std::vector<Matrix> matrices;
    in.exceptions(std::ios::badbit);

    try {
      int mask;
      in.read(reinterpret_cast<char *>(&mask), sizeof(int));
      if (mask != kMtxFileMark) {
        throw mdl::util::exceptionstream()
            .Append("File does not appear to contain matrices: ").Append(fileName)
            .Build();
      }

     do {
        int rows;
        int cols;
        in.read(reinterpret_cast<char *>(&rows), sizeof(int));
        in.read(reinterpret_cast<char *>(&cols), sizeof(int));

        if (!in.eof()) {
          if (rows * cols > 0) {
            std::unique_ptr<double> buffer(new double[rows * cols]);
            in.read(reinterpret_cast<char *>(buffer.get()), sizeof(double) * rows * cols);
            matrices.push_back(Matrix(rows, cols, buffer.release()));
          } else {
            matrices.push_back(Matrix(0, 0));
          }
        }
      } while (!in.eof());

    } catch (const std::ios_base::failure& e) {
      throw mdl::io::io_exception(e.what());
    }

    return matrices;
  }


  void SaveMtx(const char* fileName, const Matrix& matrix) {
    SaveMtx(fileName, std::vector<Matrix>({matrix}));
  }


  void SaveMtx(const char* fileName, const std::vector<Matrix>& mats) {
    std::ofstream out(fileName, std::ios_base::binary | std::ios_base::out);
    out.write(reinterpret_cast<const char *>(&kMtxFileMark), sizeof(int));

    for (auto mat = mats.begin(); mat != mats.end(); mat++) {
      out.write(reinterpret_cast<const char *>(&mat->rows), sizeof(int));
      out.write(reinterpret_cast<const char *>(&mat->cols), sizeof(int));
      if (mat->rows * mat->cols > 0) {
        out.write(reinterpret_cast<const char *>(mat->data.get()), 
            sizeof(double) * mat->rows * mat->cols);
      }
    }
  }


} // math
} // mdl