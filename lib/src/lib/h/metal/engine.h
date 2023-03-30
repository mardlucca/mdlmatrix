#ifndef _MDL_MATH_METAL_ENGINE
#define _MDL_MATH_METAL_ENGINE

#include <mdl/compute.h>
#include <memory>

namespace mdl {
namespace math {
namespace metal {

  class Engine {
    public:
      inline static compute::MetalComputeEngine * Get() {
        Init();
        return instance.engine.get();
      }

      static void Init();

    private:
      static Engine instance;
      std::unique_ptr<compute::MetalComputeEngine> engine;

      Engine();
  };

} // namespace metal
} // namespace math
} // namespace mdl

#endif // _MDL_MATH_METAL_ENGINE