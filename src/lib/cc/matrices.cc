#include "../h/matrices.h"

#include "../h/basematrix_operator_overload.h"
#include <mdl/text.h>
#include <mdl/util.h>

#include <random>
#include <vector>

namespace mdl {
namespace math {

  std::random_device Matrices::rd;
  std::mt19937 Matrices::gen(rd());
  std::uniform_real_distribution<double> Matrices::distribution;

  Matrix Matrices::Identity(int length) {
    return Matrix(
      length, length, 
      [](int row, int col) { return static_cast<double>(row == col); });
  }

  Matrix Matrices::Ones(int rows, int cols) {
    return Matrix(rows, cols,
        [](int row, int col) { return 1.0; });
  }

  Matrix Matrices::Zeros(int rows, int cols) {
    return Matrix(rows, cols);
  }

  Matrix Matrices::Random(int rows, int cols) {
    return Matrix(rows, cols,
        [](int row, int col) { 
          return Matrices::distribution(Matrices::rd); 
        });
  }

  Matrix Matrices::Sequence(int rows, int cols, int length) {
    return Sequence(rows, cols, Range(0, length));
  }

  Matrix Matrices::Sequence(int rows, int cols, Range range) {
    return Matrix(rows, cols,
        [cols, range](int row, int col) {
          int index = row * cols + col;
          return index < range.Length() ? range.Get(index) : 0; 
        });
  }  

  Matrix Matrices::Default(int rows, int cols, double def) {
    return Matrix(rows, cols, [def](int row, int col) { return def; });
  }

  Matrix Matrices::WithValues(int cols, const std::vector<double>& values) {
    int rows = (cols + values.size() - 1) / cols;
 
    return Matrix(rows, cols,
        [cols, &values](int row, int col) {
          int index = row * cols + col;
          return index < values.size() ? values[index] : 0; 
        });
  }

  class MatrixCsvParseListener : public mdl::text::ICsvParseListener {
    public:
      MatrixCsvParseListener(bool ignoreFirstLine) 
          : line(1), ignoreFirstLine(ignoreFirstLine), cols(0) {
      }

      void OnNewLine() override {
        if (ignoreFirstLine && line == 1) {
          values.clear();
          ignoreFirstLine = false;
          return;
        } 
        
        if (line == 1) {
          cols = values.size();
        }

        line++;
      }
      
      void OnValue(const std::wstring& val) override {
        try {
          values.push_back(std::stod(val));
        } catch (const std::invalid_argument& e) {
          throw mdl::util::wexceptionstream()
              .Append("Cannot covert '").Append(val).Append("' to a double.")
              .Build<mdl::text::parse_exception>();
        }
      }
      
      void OnTerminate() override {
        if (line == 1) {
          cols = values.size();
        }

        matrix = Matrices::WithValues(cols, values);
        values.clear();
      }

    private:
      Matrix matrix;
      int line;
      bool ignoreFirstLine;
      std::vector<double> values;
      int cols;

      friend Matrix Matrices::FromCsv(const char*, bool);
      Matrix& GetMatrix() { return matrix; }
  };

  Matrix Matrices::FromCsv(const char* fileName, bool ignoreFirstLine) {
    MatrixCsvParseListener listener(ignoreFirstLine);
    mdl::text::CsvParse(fileName, listener);
    return std::move(listener.GetMatrix());
  }

  Matrix Matrices::Biased(const BaseMatrix& matrix) {
    return Matrices::Ones(matrix.NumRows(), 1) | matrix;
  }


} // math
} // mdl