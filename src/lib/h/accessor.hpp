#ifndef _MDL_MATH_ACCESSOR
#define _MDL_MATH_ACCESSOR

namespace mdl {
namespace math {
  class TransposedAccessor;

  class DirectAccessor {
    public:
      inline static double Get(double * data, int rowLength, int row, int col) {
        return *(data + rowLength * row + col);
      }
      inline static double& GetLRef(double * data, int rowLength, int row, int col) {
        return *(data + rowLength * row + col);
      }
      inline static int NumRows(int rows, int cols)  { return rows; }
      inline static int NumCols(int rows, int cols)  { return cols; }

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
      inline static double Get(double * data, int rowLength, int row, int col) {
        return *(data + rowLength * col + row);
      }
      inline static double& GetLRef(double * data, int rowLength, int row, int col) {
        return *(data + rowLength * col + row);
      }
      inline static int NumRows(int rows, int cols) { return cols; }
      inline static int NumCols(int rows, int cols) { return rows; }

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