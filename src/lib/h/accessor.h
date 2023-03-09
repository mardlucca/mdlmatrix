#ifndef _MDL_MATH_ACCESSOR
#define _MDL_MATH_ACCESSOR

#include "typedefs.h"

namespace mdl {
namespace math {
  class TransposedAccessor;

  class DirectAccessor {
    public:
      inline static float_t Get(float_t * data, size_t rowLength, size_t row, size_t col) {
        return *(data + rowLength * row + col);
      }
      inline static float_t& GetLRef(float_t * data, size_t rowLength, size_t row, size_t col) {
        return *(data + rowLength * row + col);
      }
      inline static size_t NumRows(size_t rows, size_t cols)  { return rows; }
      inline static size_t NumCols(size_t rows, size_t cols)  { return cols; }

      template<typename R, typename C>
      inline static const R& GetRow(const R& row, const C& col) {
        return row;
      }

      template<typename R, typename C>
      inline static const C& GetCol(const R& row, const C& col) {
        return col;
      }

      typedef DirectAccessor type;
      typedef TransposedAccessor transposedType;
  };

  class TransposedAccessor {
    public:
      inline static float_t Get(float_t * data, size_t rowLength, size_t row, size_t col) {
        return *(data + rowLength * col + row);
      }
      inline static float_t& GetLRef(float_t * data, size_t rowLength, size_t row, size_t col) {
        return *(data + rowLength * col + row);
      }
      inline static size_t NumRows(size_t rows, size_t cols) { return cols; }
      inline static size_t NumCols(size_t rows, size_t cols) { return rows; }

      template<typename R, typename C>
      inline static const C& GetRow(const R& row, const C& col) {
        return col;
      }

      template<typename R, typename C>
      inline static const R& GetCol(const R& row, const C& col) {
        return row;
      }

      typedef TransposedAccessor type;
      typedef DirectAccessor transposedType;
  };

} // math
} // mdl

#endif // _MDL_MATH_ACCESSOR