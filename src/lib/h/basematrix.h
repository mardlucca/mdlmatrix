#ifndef _MDL_MATH_BASEMATRIX
#define _MDL_MATH_BASEMATRIX

#include <iostream>
#include <sstream>

#include "typedefs.h"

namespace mdl {
namespace math {

class BaseMatrix {
  public:
    virtual ~BaseMatrix();
    virtual float_t& operator()(size_t row, size_t col) = 0;
    virtual float_t operator()(size_t row, size_t col) const = 0;
#ifdef _MDL_MATH_LARGE_MATRICES
    virtual float_t& operator()(size_t row, int col) = 0;
    virtual float_t operator()(size_t row, int col) const = 0;
    virtual float_t& operator()(int row, size_t col) = 0;
    virtual float_t operator()(int row, size_t col) const = 0;
    virtual float_t& operator()(int row, int col) = 0;
    virtual float_t operator()(int row, int col) const = 0;
#endif
    virtual size_t NumRows() const = 0;
    virtual size_t NumCols() const = 0;
    virtual size_t NumCells() const = 0;
    virtual bool Equals(const BaseMatrix& other) const;
    virtual void ForEach(std::function<void (size_t, size_t, float_t&)> fn);
    virtual void RowSwap(size_t row1, size_t row2);
    virtual void Shuffle();

    static bool Equals(const BaseMatrix& matrix1, const BaseMatrix& matrix2);
  protected:
    friend std::ostream& operator<<(std::ostream&, const BaseMatrix&);

    void Copy(const BaseMatrix& other);
    void BufferedCopy(const BaseMatrix& other);
};

std::ostream& operator<<(std::ostream& os, const BaseMatrix& matrix);

} // math
} // mdl

#endif // _MDL_MATH_BASEMATRIX