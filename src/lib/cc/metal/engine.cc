#include <mdl/compute.h>

#include "../../h/metal/engine.h"
#include "../../h/metal/shaders.h"

#include <iostream>
#include <mdl/profiler.h>

namespace mdl {
namespace math {
namespace metal {

  Engine Engine::instance;

  Engine::Engine() {}

  void Engine::Init() {
    if (!Engine::instance.engine) {
      auto g = profiler::probe("Initing Metal");
      Engine::instance.engine.reset(new compute::MetalComputeEngine());
      Get()->LoadLibrary(shader::MatrixImpl());
    }
  }


} // namespace metal
} // namespace math
} // namespace mdl