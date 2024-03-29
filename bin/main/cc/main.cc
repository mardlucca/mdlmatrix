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
  
  Matrix examples;
  
  void doLoadData() {
    std::cout << "Loading Metal..." << std::endl;
    {
      auto g = profiler::probe("Loading metal...");
      math::metal::Engine::Init();
    }

    std::cout << "Loading up data..." << std::endl;
    {
      auto g0 = profiler::probe("Loading data");
      examples = FromMtx(
          "../mdlml/src/test/resources/CharacterData.mtx")[0];
    }

    std::cout << "Loaded data: " << examples.NumRows() << 'x' << examples.NumCols() << std::endl;
  }

  void doTransposeTest(size_t count) {
    std::cout << "Transposing..." << std::endl;
    auto g = profiler::probe("Transpose loop");
    for (size_t i = 0; i < count; i++) {
      auto g = profiler::probe("Transposing");
      Matrix m = examples.Transpose();
    }
  }

  void doAddittionTest(size_t count) {
    std::cout << "Adding..." << std::endl;
    auto g = profiler::probe("Addition Loop");
    Slice slice = examples();
    for (size_t i = 0; i < count; i++) {
      auto g = profiler::probe("Adding");
      slice += 1.0;
    }
  }

  void doSliceToMatrixTest(size_t count) {
    std::cout << "SliceToMatrix..." << std::endl;
    auto g = profiler::probe("SliceToMatrix Loop");
    for (size_t i = 0; i < count; i++) {
      auto g = profiler::probe("Materializing");
      Matrix m = examples();
    }
  }
}

int main() {
  {
    auto g = mdl::profiler::probe("Total");
    mdl::doLoadData();
    mdl::doTransposeTest(1000);
    // mdl::doAddittionTest(10000);
    // mdl::doSliceToMatrixTest(1000);
  }
  std::ofstream out("prof-report.xml");
  mdl::profiler::save(out);
  return 0;
}