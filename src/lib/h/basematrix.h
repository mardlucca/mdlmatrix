#ifndef _MDL_MATH_BASEMATRIX
#define _MDL_MATH_BASEMATRIX

#include <iostream>
#include <sstream>

#include "float_t.h"

namespace mdl {
namespace math {

class BaseMatrix {
  public:
    virtual ~BaseMatrix();
    virtual float_t& operator()(int row, int col) = 0;
    virtual float_t operator()(int row, int col) const = 0;
    virtual int NumRows() const = 0;
    virtual int NumCols() const = 0;
    virtual bool Equals(const BaseMatrix& other) const;
    virtual void ForEach(std::function<void (int, int, float_t&)> fn);

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