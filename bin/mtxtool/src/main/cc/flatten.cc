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
#include <memory>

#include <mdl/text.h>
#include <mdl/util.h>
#include <mdl/matrix.h>

namespace mdl {
namespace math {
namespace tools {
namespace flatten {

  using util::cli::GetOpts;
  using util::functional::Assign;

  const char* intputFileName;
  bool help = false;

  GetOpts opts(Assign(&intputFileName));

  void PrintUsage() {
    std::cout << 
R"(usage: mtxtool flatten <matrix-file>
Joins all matrices in an MTX file into one single matrix, appending them horizontally.

where:
  <matrix-file> Matrix file, in MTX format, to flatten. Defaults to stdin.
  --help     Prints this mesage
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption("help", Assign(&help, true));

    return opts.Parse(args, argc);
  }

  int Main(const char** args, int argc) {
    if (!ParseArgs(args, argc)) {
      return 1;
    }

    if (help) {
      PrintUsage();
      return 0;
    }

    Matrix matrix;
    bool empty = true;
    if (intputFileName) {
      FromMtx(intputFileName, [&matrix, &empty](Matrix&& mat) {
        matrix = matrix | mat;
        empty = false;
        return true;
      });
    } else {
      FromMtx(std::cin, [&matrix, &empty](Matrix&& mat) {
        matrix = matrix | mat;
        empty = false;
        return true;
      });
    }

    if (!empty) {
      if (intputFileName) {
        math::SaveMtx(intputFileName, matrix);
      } else {
        math::SaveMtx(std::cout, matrix);
      }
    }

    return 0;
  }

} // namespace flatten
} // namespace tools
} // namespace math
} // namespace mdl

