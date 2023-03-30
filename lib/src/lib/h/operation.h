#ifndef _MDL_MATH_MATRIX_OPERATION
#define _MDL_MATH_MATRIX_OPERATION

#include <cmath>

#include "typedefs.h"

namespace mdl {
namespace math {
namespace op {

class Abs {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::abs(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::abs(op1);
    }
};

class Acos {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::acos(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::acos(op1);
    }
};

class Addition {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 + op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 += op2;
    }
};

class And {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 && op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 = op1 && op2;
    }
};

class Asin {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::asin(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::asin(op1);
    }
};

class Atan {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::atan(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::tan(op1);
    }
};

class Ceil {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::ceil(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::ceil(op1);
    }
};

class Cos {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::cos(op1);
    }

    inline static void operate(float_t& op1) {
      op1 *= std::cos(op1);
    }
};

class Division {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 / op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 /= op2;
    }
};

class Equals {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      float_t diff = std::abs(op2 - op1);
      out = diff <= kFloatPrecision;
    }

    inline static void operate(float_t& op1, float_t op2) {
      float_t diff = std::abs(op2 - op1);
      op1 = diff <= kFloatPrecision;
    }
};

class Exp {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::exp(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::exp(op1);
    }
};

class Exp2 {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::exp2(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::exp2(op1);
    }
};

class Floor {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::floor(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::floor(op1);
    }
};

class GreaterThan {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 > op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 = op1 > op2;
    }
};

class GreaterThanEquals {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 >= op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 = op1 >= op2;
    }
};

class LessThan {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 < op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 = op1 < op2;
    }
};

class LessThanEquals {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 <= op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 = op1 <= op2;
    }
};

class Log {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::log(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::log(op1);
    }
};

class Log2 {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::log2(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::log2(op1);
    }
};

class Log10 {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::log10(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::log10(op1);
    }
};

class Max {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 > op2 ? op1 : op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      if (op2 > op1) {
        op1 = op2;
      }
    }
};

class Min {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 < op2 ? op1 : op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      if (op2 < op1) {
        op1 = op2;
      }
    }
};

class Mod {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 - op2 * std::floor(op1 / op2);
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 -= op2 * std::floor(op1 / op2);
    }
};

class Multiplication {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 * op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 *= op2;
    }
};

class Negate {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = -op1;
    }

    inline static void operate(float_t& op1) {
      op1 = -op1;
    }
};

class Not {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = !op1;
    }

    inline static void operate(float_t& op1) {
      op1 = !op1;
    }
};

class NotEquals {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 != op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 = op1 != op2;
    }
};

class Or {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 || op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 = op1 || op2;
    }
};

class Pow {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = std::pow(op1, op2);
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 *= std::pow(op1, op2);
    }
};

class Round {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::round(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::round(op1);
    }
};

class Sin {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::sin(op1);
    }

    inline static void operate(float_t& op1) {
      op1 *= std::sin(op1);
    }
};

class Sqr {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = op1 * op1;
    }

    inline static void operate(float_t& op1) {
      op1 *= op1;
    }
};

class Sqrt {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::sqrt(op1);
    }

    inline static void operate(float_t& op1) {
      op1 = std::sqrt(op1);
    }
};

class Subtraction {
  public:
    inline static void operate(float_t op1, float_t op2, float_t& out) {
      out = op1 - op2;
    }

    inline static void operate(float_t& op1, float_t op2) {
      op1 -= op2;
    }
};

class Tan {
  public:
    inline static void operate(float_t op1, float_t& out) {
      out = std::tan(op1);
    }

    inline static void operate(float_t& op1) {
      op1 *= std::tan(op1);
    }
};

} // op
} // math
} // mdl

#endif // _MDL_MATH_MATRIX_OPERATION