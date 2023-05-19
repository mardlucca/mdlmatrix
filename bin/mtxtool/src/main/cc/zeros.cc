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
namespace zeros {

  using util::cli::GetOpts;
  using util::functional::Assign;
  using text::ParseInt;

  const char* stdoutFile = "/dev/stdout";
  bool csv = false;
  bool help = false;
  bool raw = false;
  math::size_t rows = 1;
  math::size_t cols = 1;

  GetOpts opts;

  void PrintUsage() {
    std::cout << 
R"(usage: mtxtool zeros [options]
Generate a matrix with values set to 0.0 (zero)

where:
  --rows     Number of rows. Defaults to 1.
  --cols     From col. Defaults to 1.
  --csv      Outputs in csv format
  --raw      Outputs in raw format (e.g. binary MTX representation)
  --help     Prints this mesage
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption("help", Assign(&help, true));
    opts.AddOption("cols", Assign(&cols, ParseInt));
    opts.AddOption("rows", Assign(&rows, ParseInt));
    opts.AddOption("raw", Assign(&raw, true));
    opts.AddOption("csv", Assign(&csv, true));

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

    if (csv && raw) {
      std::cout << "error: can only specify one of 'csv' and 'raw'" << std::endl;
      return 2;
    }

    Matrix matrix = math::Matrices::Zeros(std::max(1, rows), std::max(1, cols));
    if (raw) {
      math::SaveMtx(stdoutFile, matrix);
    } else if (csv) {
      math::SaveCsv(std::cout, matrix);
    } else {
      std::cout << matrix << endl;
    }

    return 0;
  }

} // namespace zeros
} // namespace tools
} // namespace math
} // namespace mdl

