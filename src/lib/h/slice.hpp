#ifndef _MDL_MATH_SLICE
#define _MDL_MATH_SLICE

#include <memory>
#include <iostream>

#include "basematrix.h"
#include "range.h"
#include "accessor.hpp"
#include "operation.hpp"


namespace mdl {
namespace math {

  using std::cout;
  using std::endl;

  template<typename RowRange, typename ColRange, typename Accessor>
  class Slice : public BaseMatrix {
    public:
      Slice(std::shared_ptr<double[]> data,
            int rowLength, 
            const RowRange& rowRange, 
            const ColRange& colRange) 
          : data(data), rowLength(rowLength), rowRange(rowRange), colRange(colRange) {
      }

      Slice(const Slice& other) 
          : data(other.data), rowLength(other.rowLength),
            rowRange(other.rowRange), colRange(other.colRange) {
          // The reason why this looks like a move constructor is because we want the new
          // slice (copy) to also modify the original matrix pointed to by the slice.
      }

      Slice(Slice&& other) 
          : data(other.data), rowLength(other.rowLength),
            rowRange(other.rowRange), colRange(other.colRange) {
      }

      Slice(Slice<RowRange, ColRange, typename Accessor::transposedType>&& other) 
          : Slice<RowRange, ColRange, Accessor>(
            other.data, other.rowLength, other.rowRange, other.colRange) {
      }

      template<typename RR, typename CR>
      Slice(Slice<RR, CR, Accessor>&& other, RowRange rowRange, ColRange colRange) 
          : Slice<RowRange, ColRange, Accessor>(
            other.data, other.rowLength, rowRange, colRange) {
      }
      
      Slice& operator=(const Slice& other) {
        BaseMatrix::BufferedCopy(other);
        return *this;
      }

      Slice& operator=(const BaseMatrix& other) {
        BaseMatrix::BufferedCopy(other);
        return *this;
      }

      inline double& operator()(int row, int col) {
        return Accessor::GetLRef(
            data.get(), 
            rowLength, 
            Accessor::GetRow(rowRange, colRange).Get(row), 
            Accessor::GetCol(rowRange, colRange).Get(col));
      }
      
      inline double operator()(int row, int col) const {
        return Accessor::Get(
            data.get(), 
            rowLength, 
            Accessor::GetRow(rowRange, colRange).Get(row), 
            Accessor::GetCol(rowRange, colRange).Get(col));
      }

      auto operator()(const Range& slicedRows, const Range& slicedCols) {
        auto rowRng = Ranges::FitAndCompose(rowRange, Accessor::GetRow(slicedRows, slicedCols));
        auto colRng = Ranges::FitAndCompose(colRange, Accessor::GetCol(slicedRows, slicedCols));
        return Slice<decltype(rowRng), decltype(colRng), Accessor>(
            std::move(*this), rowRng, colRng); 
      }

      template<typename FRR, typename FCR>
      auto operator()(const FRR& slicedRows, const FCR& slicedCols) {
        auto rowRng = Ranges::FitAndCompose(rowRange, Accessor::GetRow(slicedRows, slicedCols));
        auto colRng = Ranges::FitAndCompose(colRange, Accessor::GetCol(slicedRows, slicedCols));
        return Slice<decltype(rowRng), decltype(colRng), Accessor>(
            std::move(*this), rowRng, colRng); 
      }

      template<typename RR, typename CR, typename AC>
      Slice& operator+=(const Slice<RR, CR, AC>& other) {
        ReflexiveOperate<op::Addition>(other);
        return *this;
      }
      
      Slice& operator+=(const BaseMatrix& other) {
        ReflexiveOperate<op::Addition>(other);
        return *this;
      }

      Slice& operator+=(double scalar) {
        ReflexiveOperate<op::Addition>(scalar);
        return *this;
      }

      template<typename RR, typename CR, typename AC>
      Slice& operator-=(const Slice<RR, CR, AC>& other) {
        ReflexiveOperate<op::Subtraction>(other);
        return *this;
      }
      
      Slice& operator-=(const BaseMatrix& other) {
        ReflexiveOperate<op::Subtraction>(other);
        return *this;
      }

      Slice& operator-=(double scalar) {
        ReflexiveOperate<op::Subtraction>(scalar);
        return *this;
      }

      template<typename RR, typename CR, typename AC>
      Slice& operator*=(const Slice<RR, CR, AC>& other) {
        ReflexiveOperate<op::Multiplication>(other);
        return *this;
      }

      Slice& operator*=(const BaseMatrix& other) {
        ReflexiveOperate<op::Multiplication>(other);
        return *this;
      }

      Slice& operator*=(double scalar) {
        ReflexiveOperate<op::Multiplication>(scalar);
        return *this;
      }

      template<typename RR, typename CR, typename AC>
      Slice& operator/=(const Slice<RR, CR, AC>& other) {
        ReflexiveOperate<op::Division>(other);
        return *this;
      }

      Slice& operator/=(const BaseMatrix& other) {
        ReflexiveOperate<op::Division>(other);
        return *this;
      }
      
      Slice& operator/=(double scalar) {
        ReflexiveOperate<op::Division>(scalar);
        return *this;
      }
      
      int NumRows() const { return Accessor::GetRow(rowRange.Length(), colRange.Length()); }
      int NumCols() const { return Accessor::GetCol(rowRange.Length(), colRange.Length()); }

      Slice<RowRange, ColRange, typename Accessor::transposedType> Transpose() {
        return Slice<RowRange, ColRange, typename Accessor::transposedType>(std::move(*this));
      }

      template <typename Operation>
      void ReflexiveOperate(double scalar) {
        int rows = NumRows();
        int cols = NumCols();

        for (int row = 0; row < rows; row++) {
          for (int col = 0; col < cols; col++) {
            Operation::operate((*this)(row, col), scalar);
          }
        }
      }

      template <typename Operation>
      void ReflexiveOperate(const BaseMatrix& other) {
        int rows1 = NumRows();
        int cols1 = NumCols();
        int rows2 = other.NumRows();
        int cols2 = other.NumCols();

        if ((rows1 == rows2 && (cols1 == cols2 || cols2 == 1))
            || (cols1 == cols2 && rows2 == 1)
            || (rows2 == 1 && cols2 == 1)) {

          for (int row = 0; row < rows1; row++) {
            for (int col = 0; col < cols1; col++) {
              Operation::operate(
                  (*this)(row, col), 
                  other(rows2 == 1 ? 0 : row, cols2 == 1 ? 0 : col));
            }
          }
        } else {
          std::ostringstream os;
          os << "Cannot operate on matrices of different dimensions: " << rows1 << 'x' << cols1
              << " and " << rows2 << 'x' << cols2;
          throw std::invalid_argument(os.str());
        }
      }

      template <typename Operation>
      void ReflexiveUnaryOperate() {
        int rows = NumRows();
        int cols = NumCols();

        for (int row = 0; row < rows; row++) {
          for (int col = 0; col < cols; col++) {
            Operation::operate((*this)(row, col));
          }
        }
      }

    protected:
      std::shared_ptr<double[]> data;
      int rowLength;
      RowRange rowRange;
      ColRange colRange;

      template<typename RR, typename CR, typename AC>
      friend class Slice;
  };

} // math
} // mdl

#endif // _MDL_MATH_SLICE