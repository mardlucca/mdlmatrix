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

#include <filesystem>
#include <iostream>
#include <vector>

#include <mdl/util.h>
#include <mdl/matrix.h>


namespace mdl {
namespace math {
namespace tools {
namespace pop {

  using util::cli::GetOpts;
  using util::functional::Assign;

  const char* inputFileName = nullptr;
  bool csv = false;
  bool help = false;
  bool raw = false;
  GetOpts opts(Assign(&inputFileName));

  void PrintUsage() {
    std::cerr << 
R"(usage: mtxtool pop <input-file>
where:
  <input-file>  Matrix file, in MTX format, to pop from.
  --csv         Outputs in csv format
  --raw         Outputs in raw format (e.g. binary MTX representation)
  --help        Shows this help message.
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption("help", Assign(&help, true));
    opts.AddOption("raw", Assign(&raw, true));
    opts.AddOption("csv", Assign(&csv, true));

    return opts.Parse(args, argc);
  }

  int Main(const char** args, int argc) {
    if (!ParseArgs(args, argc)) {
      PrintUsage();
      return 1;
    }

    if (help) {
      PrintUsage();
      return 0;
    }

    if (!inputFileName) {
      std::cerr << "error: input file name not provided" << std::endl;
      return 2;
    }

    if (csv && raw) {
      std::cerr << "error: can only specify one of 'csv' and 'raw'" << std::endl;
      return 3;
    }

    std::unique_ptr<Matrix> popped;
    std::unique_ptr<Matrix> mat;
    int idx = 0;
    auto stream = FromMtxStream(inputFileName);

    SaveMtx(inputFileName, [&popped, &idx, &stream, &mat]() {
      if (idx == 0) {
        popped = stream();
        idx++;
      }
      mat = stream();
      return mat.get();
    });

    if (raw) {
      if (!popped) {
        math::SaveMtx(std::cout, []() -> const Matrix* { return nullptr; });
      } else {
        math::SaveMtx(std::cout, *popped);
      }
    } else if (csv) {
      if (popped) {
        math::SaveCsv(std::cout, *popped);
      }
    } else if (popped) {
      std::cout << *popped << endl;
    }

    return 0;
  }
} // namespace pop
} // namespace tools
} // namespace math
} // namespace mdl

