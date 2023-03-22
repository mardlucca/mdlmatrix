#include "../../h/metal/shaders.h"

namespace mdl {
namespace math {
namespace metal {
namespace shader {

  std::string MatrixImpl() {
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

        kernel void Multiply(constant float* m1 [[buffer(0)]],
                        constant float* m2 [[buffer(1)]],
                        constant int& cols1  [[buffer(2)]],
                        device float* out [[buffer(3)]],
                        uint2 index [[ thread_position_in_grid ]],
                        uint2 gridSize [[ threads_per_grid ]]) {
          float sum = 0.0;
          constant float* m1p = m1 + index.y * cols1;
          constant float* m2p = m2 + index.x * cols1;
          for (int col = 0; col < cols1; col++) {
            sum += m1p[col] * m2p[col];
          }
          out[index.y * gridSize.x + index.x] = sum;
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