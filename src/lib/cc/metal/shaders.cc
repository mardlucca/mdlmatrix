#include "../../h/metal/shaders.h"

namespace mdl {
namespace math {
namespace metal {
namespace shader {

  std::string Transpose() {
    return std::string(R"(
        #include <metal_stdlib>
        using namespace metal;

        // grid is based off m1 and may not contain all rows
        kernel void Transpose(
            device const float* m1,
            device float* out,
            device const int& rows,
            uint2 index [[ thread_position_in_grid ]],
            uint2 gridSize [[ threads_per_grid ]]) {
          out[index.x * rows + index.y] = m1[index.y * gridSize.x + index.x];
        }

      kernel void Copy(const device float* inA [[buffer(0)]], 
                       device float* outB [[buffer(1)]],
                       uint2 index [[ thread_position_in_grid ]],
                       uint2 gridSize [[ threads_per_grid ]])
      {
          outB[index.y * gridSize.x + index.x] = inA[index.y * gridSize.x + index.x];
      }
    )");
  }

} // namespace shader
} // namespace metal
} // namespace math
} // namespace mdl