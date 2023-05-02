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
namespace cat {

  using util::cli::GetOpts;
  using util::functional::AsFunction;
  using util::functional::Assign;
  using util::functional::Set;
  using text::ParseInt;

  const char* inputFile = "/dev/stdin";
  const char* stdoutFile = "/dev/stdout";
  bool csv = false;
  bool help = false;
  bool raw = false;
  bool sizesOnly = false;
  math::size_t fr = 0;
  math::size_t fc = 0;
  math::size_t tr = mdl::math::kMaxSizeT;
  math::size_t tc = mdl::math::kMaxSizeT;

  GetOpts opts(Assign(&inputFile));

  void PrintUsage() {
    std::cout << 
R"(usage: mtxtool cat [options] <file>
where:
  --fr       From row. Defaults to 0.
  --fc       From col. Defaults to 0.
  --tr       To row, not including last. Defaults to last row in matrix.
  --tc       To col, not including last. Defaults to last col in matrix.
  --csv      Outputs in csv format
  --raw      Outputs in raw format (e.g. binary MTX representation)
  -s,--sizes Prints matrix sizes only 
  --help     Prints this mesage
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption("help", Set(&help, true));
    opts.AddOption("fr", Assign(&fr, AsFunction(ParseInt)));
    opts.AddOption("fc", Assign(&fc, AsFunction(ParseInt)));
    opts.AddOption("tr", Assign(&tr, AsFunction(ParseInt)));
    opts.AddOption("tc", Assign(&tc, AsFunction(ParseInt)));
    opts.AddOption("raw", Set(&raw, true));
    opts.AddOption("csv", Set(&csv, true));
    opts.AddOption('s', "sizes", Set(&sizesOnly, true));

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

    if (sizesOnly) {
      FromMtx(inputFile, [](Matrix&& matrix) {
        std::cout << matrix.NumRows() << " x " << matrix.NumCols() << std::endl;
        return true;
      });
    } else if (csv) {
      FromMtx(inputFile, [](Matrix&& matrix) {
        for (size_t row = std::max(0, fr); row < std::min(tr, matrix.NumRows()); row++) {
          for (size_t col = std::max(0, fc); col < std::min(tc, matrix.NumCols()); col++) {
            if (col > 0) { std::cout << ','; }
            std::cout << matrix(row, col);
          }
          std::cout << std::endl;
        }
        return true;
      });
    } else if (raw) {
      SaveMtx(stdoutFile, mdl::util::functional::AsSupplier(FromMtxStream(inputFile)));
    } else {
      FromMtx(inputFile, [](Matrix&& matrix) {
        std::cout << matrix(Range(fr, tr), Range(fc, tc)) << std::endl;
        return true;
      });
    }

    return 0;
  }

} // namespace cat
} // namespace tools
} // namespace math
} // namespace mdl

