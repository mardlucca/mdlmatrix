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

#include <mdl/util.h>
#include <mdl/matrix.h>
#include <vector>

namespace mdl {
namespace math {
namespace tools {
  using util::GetOpts;
  using util::functional::AsFunction;
  using util::functional::Assign;
  using util::functional::Set;
  using util::functional::SupplierIterable;

  GetOpts opts;
  const char* inputFiles = "/dev/stdin";
  const char* outputFile = "/dev/stdout";
  bool help = false;

  void PrintUsage() {
    std::cout << 
R"(usage: mtxshuffle [-i <file>|-o <file>|-h]
where:
  -i  Input file name. Default to stdin.
  -o  Output file name. Defaults to stdout.
  -h  Shows this help message.
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption('h', Set(&help, true));
    opts.AddOption("-i", Assign(&inputFiles));
    opts.AddOption("-o", Assign(&outputFile));

    return opts.Parse(args, argc);
  }

  int DoMain(const char** args, int argc) {
    if (!ParseArgs(args, argc)) {
      PrintUsage();
      return 1;
    }

    if (help) {
      PrintUsage();
      return 0;
    }

    std::unique_ptr<Matrix> mat;
    SaveMtx(outputFile, [supplier = FromMtxStream(inputFiles), &mat]() {
      mat = supplier();
      if (mat) {
        mat->Shuffle();
        return mat.get();
      }
      return static_cast<Matrix *>(nullptr);
    });

    return 0;
  }

} // namespace tools
} // namespace math
} // namespace mdl

int main(int argc, const char** args) {
  return mdl::math::tools::DoMain(args, argc);
}

