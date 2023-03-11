#include <mdl/compute.h>

#include "../../h/metal/engine.h"
#include "../../h/metal/shaders.h"

#include <iostream>

namespace mdl {
namespace math {
namespace metal {

  Engine Engine::instance;

  Engine::Engine() {}

  void Engine::Init() {
    if (!Engine::instance.engine) {
      Engine::instance.engine.reset(new compute::MetalComputeEngine());
      Get()->LoadLibrary(shader::Transpose());
    }
  }


} // namespace metal
} // namespace math
} // namespace mdl