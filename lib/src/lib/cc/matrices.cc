#include "../h/matrices.h"

#include "../h/basematrix_operator_overload.h"
#include "../h/misc.h"

#include <mdl/text.h>
#include <mdl/util.h>

#include <random>
#include <vector>

namespace mdl {
namespace math {

  Matrix Matrices::Identity(size_t length) {
    return Matrix(
      length, length, 
      [](size_t row, size_t col) { return static_cast<float_t>(row == col); });
  }

  Matrix Matrices::Ones(size_t rows, size_t cols) {
    return Matrix(rows, cols,
        [](size_t row, size_t col) { return 1.0; });
  }

  Matrix Matrices::Zeros(size_t rows, size_t cols) {
    return Matrix(rows, cols);
  }

  Matrix Matrices::Random(size_t rows, size_t cols) {
    return Matrix(rows, cols,
        [](size_t row, size_t col) { 
          return NextRand(); 
        });
  }

  Matrix Matrices::Sequence(size_t rows, size_t cols, size_t length) {
    return Sequence(rows, cols, Range(0, length));
  }

  Matrix Matrices::Sequence(size_t rows, size_t cols, Range range) {
    return Matrix(rows, cols,
        [cols, range](size_t row, size_t col) {
          size_t index = row * cols + col;
          return index < range.Length() ? range.Get(index) : 0; 
        });
  }  

  Matrix Matrices::Default(size_t rows, size_t cols, float_t def) {
    return Matrix(rows, cols, [def](size_t row, size_t col) { return def; });
  }

  Matrix Matrices::WithValues(size_t cols, const std::vector<float_t>& values) {
    size_t rows = (cols + values.size() - 1) / cols;
 
    return Matrix(rows, cols,
        [cols, &values](size_t row, size_t col) {
          size_t index = row * cols + col;
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
      
      void OnValue(const std::string& val) override {
        try {
        if (!ignoreFirstLine || line > 1) {
            values.push_back(std::stod(val));
        }
        } catch (const std::invalid_argument& e) {
          throw mdl::util::exceptionstream()
              .Append("Cannot covert '").Append(val).Append("' to a float_t.")
              .Build<mdl::text::parse_exception>();
        }
      }
      
      void OnTerminate() override {
        if (line == 1) {
          cols = values.size();
        }

        if (cols > 0) {
          matrix = Matrices::WithValues(cols, values);
        }
        
        values.clear();
      }

    private:
      Matrix matrix;
      size_t line;
      bool ignoreFirstLine;
      std::vector<float_t> values;
      size_t cols;

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