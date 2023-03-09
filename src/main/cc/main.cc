// Copyright (c) 2023, Marcio Lucca
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include <fstream>

#include <mdl/matrix.h>
#include <mdl/profiler.h>

namespace mdl {
  using namespace mdl::math;
  using mdl::math::float_t;

  void doTransposeTest() {
    std::cout << "Loading up data..." << std::endl;
    Matrix examples;
    {
      auto g0 = profiler::probe("Loading data");
      examples = FromMtx(
          "../mdlml/src/test/resources/CharacterData.mtx")[0];
    }
    examples = examples | examples;
    std::cout << "Loaded data: " << examples.NumRows() << 'x' << examples.NumCols() << std::endl;
    std::cout << "Transposing..." << std::endl;
    for (long i = 0; i < 500; i++) {
      auto g = profiler::probe("Transpose loop");
      Matrix m = examples.Transpose();
    }
  }
}

int main() {
  mdl::doTransposeTest();
  std::ofstream out("prof-report.xml");
  mdl::profiler::save(out);
  return 0;
}

  // const char * shader = R"(
  //     #include <metal_stdlib>
  //     using namespace metal;

  //     kernel void Mult(constant float* m1 [[buffer(0)]],
  //                      constant float* m2 [[buffer(1)]],
  //                      constant size_t& cols  [[buffer(2)]],
  //                      device float* out [[buffer(3)]],
  //                      uint2 index [[ thread_position_in_grid ]],
  //                      uint2 gridSize [[ threads_per_grid ]]) {
  //       float sum = 0.0;
  //       for (size_t col = 0; col < cols; col++) {
  //         size_t idx = index.y * cols + col;
  //         sum += m1[idx] * m2[idx];
  //       }
  //       out[index.y * gridSize.x + index.x] = sum;
  //     }

  //     kernel void Sum(device float* vector [[buffer(0)]],
  //              constant float& scalar [[buffer(1)]],
  //              uint index [[ thread_position_in_grid ]]) {
  //       vector[index] += scalar;
  //     }
  // )";

  // TEST(MetaldMatrixOpsTestSuite, TestMult) {
  //   using mdl::compute::in;
  //   using mdl::compute::out;
  //   using mdl::compute::inout;

  //   mdl::compute::MetalComputeEngine engine;
  //   engine.LoadLibrary(shader);
  //   const size_t kSize = 2000;

  //   float * m1 = Sequence(kSize, kSize);
  //   // float * m2 = Sequence(kSize, kSize, 20.0);
  //   // float * m3 = new float[kSize*kSize];

  //   auto start = mdl::util::Now();

  //   for (size_t i = 0; i < 1; i++)
  //   {
  //     engine.NewBatch()
  //         .WithGrid(1, kSize * kSize, 1, 64)
  //         // .Call("Mult", in(m1, kSize*kSize), in(m2, kSize*kSize), in(kSize), out(m3, kSize*kSize))
  //         .Call("Sum", inout(m1, kSize*kSize), 2.0)
  //         .Dispatch().Wait();
  //   }
  //   auto end = mdl::util::Now();

  //   cout << "Ellapsed: " << mdl::util::EllapsedTime(start, end) << endl;


  //   delete[] m1;
  //   // delete[] m2;
  //   // delete[] m3;
  // }