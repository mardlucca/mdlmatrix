#include "../h/typedefs.h"

namespace mdl {
namespace math {

  single_t * ToSingleBuff(const double_t * buff, std::size_t len) {
    single_t * data = new single_t[len];

    for (size_t i = 0; i < len; i++) {
      data[i] = (single_t) buff[i];
    }
    
    return data;
  }

  double_t * ToDoubleBuff(const single_t * buff, std::size_t len) {
    double_t * data = new double_t[len];

    for (size_t i = 0; i < len; i++) {
      data[i] = buff[i];
    }

    return data;
  }
} // math
} // mdl
