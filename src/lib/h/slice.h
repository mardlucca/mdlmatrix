#ifndef _MDL_MATH_SLICE
#define _MDL_MATH_SLICE

#include <memory>
#include <iostream>

#include "typedefs.h"
#include "basematrix.h"
#include "range.h"
#include "accessor.h"
#include "operation.h"
#include "singlethread/basematrix_reflexive_impl.h"
#include "multithread/basematrix_reflexive_impl.h"

#include <mdl/profiler.h>

namespace mdl {
namespace math {
  // using singlethread::BaseMatrixReflexiveImpl;
  using multithread::BaseMatrixReflexiveImpl;
  using std::cout;
  using std::endl;

  template<typename RowRange, typename ColRange, typename Accessor>
  class Slice : public BaseMatrix {
    public:
      Slice(std::shared_ptr<float_t[]> data,
            size_t rowLength, 
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

      inline float_t& operator()(size_t row, size_t col) {
        return Accessor::GetLRef(
            data.get(), 
            rowLength, 
            Accessor::GetRow(rowRange, colRange).Get(row), 
            Accessor::GetCol(rowRange, colRange).Get(col));
      }
      
      inline float_t operator()(size_t row, size_t col) const {
        return Accessor::Get(
            data.get(), 
            rowLength, 
            Accessor::GetRow(rowRange, colRange).Get(row), 
            Accessor::GetCol(rowRange, colRange).Get(col));
      }

#ifdef _MDL_MATH_LARGE_MATRICES
      inline float_t& operator()(size_t row, int col) {
        return (*this)(row, (size_t) col);
      }
      inline float_t operator()(size_t row, int col) const {
        return (*this)(row, (size_t) col);
      }
      inline float_t& operator()(int row, size_t col) {
        return (*this)((size_t) row, col);
      }
      inline float_t operator()(int row, size_t col) const {
        return (*this)((size_t) row, col);
      }
      inline float_t& operator()(int row, int col) {
        return (*this)((size_t) row, (size_t) col);
      }
      inline float_t operator()(int row, int col) const {
        return (*this)((size_t) row, (size_t) col);
      }
#endif

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
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(this, other);
        return *this;
      }
      
      Slice& operator+=(const BaseMatrix& other) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(this, other);
        return *this;
      }

      Slice& operator+=(float_t scalar) {
        auto g = profiler::probe("Slice add");
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Addition>(this, scalar);
        return *this;
      }

      template<typename RR, typename CR, typename AC>
      Slice& operator-=(const Slice<RR, CR, AC>& other) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Subtraction>(this, other);
        return *this;
      }
      
      Slice& operator-=(const BaseMatrix& other) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Subtraction>(this, other);
        return *this;
      }

      Slice& operator-=(float_t scalar) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Subtraction>(this, scalar);
        return *this;
      }

      template<typename RR, typename CR, typename AC>
      Slice& operator*=(const Slice<RR, CR, AC>& other) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Multiplication>(this, other);
        return *this;
      }

      Slice& operator*=(const BaseMatrix& other) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Multiplication>(this, other);
        return *this;
      }

      Slice& operator*=(float_t scalar) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Multiplication>(this, scalar);
        return *this;
      }

      template<typename RR, typename CR, typename AC>
      Slice& operator/=(const Slice<RR, CR, AC>& other) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Division>(this, other);
        return *this;
      }

      Slice& operator/=(const BaseMatrix& other) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Division>(this, other);
        return *this;
      }
      
      Slice& operator/=(float_t scalar) {
        BaseMatrixReflexiveImpl::ReflexiveOperate<op::Division>(this, scalar);
        return *this;
      }
      
      size_t NumRows() const { return Accessor::GetRow(rowRange.Length(), colRange.Length()); }
      size_t NumCols() const { return Accessor::GetCol(rowRange.Length(), colRange.Length()); }
      size_t NumCells() const { return NumRows() * NumCols(); }


      Slice<RowRange, ColRange, typename Accessor::transposedType> Transpose() {
        return Slice<RowRange, ColRange, typename Accessor::transposedType>(std::move(*this));
      }

    protected:
      std::shared_ptr<float_t[]> data;
      size_t rowLength;
      RowRange rowRange;
      ColRange colRange;

      template<typename RR, typename CR, typename AC>
      friend class Slice;
  };

} // math
} // mdl

#endif // _MDL_MATH_SLICE