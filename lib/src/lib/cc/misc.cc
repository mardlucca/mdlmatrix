#include "../h/misc.h"

#include <random>

namespace mdl {
namespace math {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float_t> distribution;

  float_t NextRand() {
    return distribution(gen);
  }

  size_t NextRand(size_t max) {
    return (size_t)(max * NextRand());
  }


} // math
} // mdl