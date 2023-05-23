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
  using util::functional::Assign;
  using text::ParseInt;

  const char* inputFile = nullptr;
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
    std::cerr << 
R"(usage: mtxtool cat [options] <file>
where:
  --fr       From row. Defaults to 0.
  --fc       From col. Defaults to 0.
  --tr       To row, not including last. Defaults to last row in matrix.
  --tc       To col, not including last. Defaults to last col in matrix.
  --row      Outputs a specific row. Equivalent to --fr <row> --tr <row + 1>
  --col      Outputs a specific column. Equivalent to --fc <col> --tc <col + 1>
  --csv      Outputs in csv format
  --raw      Outputs in raw format (e.g. binary MTX representation)
  -s,--sizes Prints matrix sizes only 
  --help     Prints this mesage
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption("help", Assign(&help, true));
    opts.AddOption("fr", Assign(&fr, ParseInt));
    opts.AddOption("fc", Assign(&fc, ParseInt));
    opts.AddOption("tr", Assign(&tr, ParseInt));
    opts.AddOption("tc", Assign(&tc, ParseInt));
    opts.AddOption("raw", Assign(&raw, true));
    opts.AddOption("csv", Assign(&csv, true));
    opts.AddOption("row", [](const char* val) {
      int row = ParseInt(val);
      fr = row;
      tr = row + 1;
    });
    opts.AddOption("col", [](const char* val) {
      int col = ParseInt(val);
      fc = col;
      tc = col + 1;
    });
    opts.AddOption('s', "sizes", Assign(&sizesOnly, true));

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
      std::cerr << "error: can only specify one of 'csv' and 'raw'" << std::endl;
      return 2;
    }

    if (sizesOnly) {
      if (raw) {
        std::function<std::unique_ptr<Matrix> ()> stream = inputFile
            ? FromMtxStream(inputFile)
            : FromMtxStream(&std::cin);
        Matrix mat(1, 2);
        SaveMtx(std::cout, [&stream, &mat]() -> const Matrix* {
          auto pMatrix = stream();
          if (!pMatrix) { return nullptr; }
          auto slice = (*pMatrix)(Range(fr, tr), Range(fc, tc));
          mat(0, 0) = slice.NumRows();
          mat(0, 1) = slice.NumCols();
          return &mat;
        });
      } else {
        auto consumer = [](Matrix&& matrix) {
          auto slice = matrix(Range(fr, tr), Range(fc, tc));
          if (csv) {
            std::cout << slice.NumRows() << "," << slice.NumCols() << std::endl;
          } else {
            std::cout << slice.NumRows() << " x " << slice.NumCols() << std::endl;
          }
          return true;
        };

        if (inputFile) {
          FromMtx(inputFile, consumer);
        } else {
          FromMtx(std::cin, consumer);
        }
      }
    } else {    // !sizesOnly
      if (raw) {
        std::function<std::unique_ptr<Matrix> ()> stream = inputFile
            ? FromMtxStream(inputFile)
            : FromMtxStream(&std::cin);
        Matrix mat;
        SaveMtx(std::cout, [&stream, &mat]() -> const Matrix* {
          auto pMatrix = stream();
          if (!pMatrix) { return nullptr; }
          mat = (*pMatrix)(Range(fr, tr), Range(fc, tc));
          return &mat;
        });
      } else {
        auto consumer = [](Matrix&& matrix) {
          if (csv) {
            SaveCsv(std::cout, matrix(Range(fr, tr), Range(fc, tc)));
          } else {
            std::cout << matrix(Range(fr, tr), Range(fc, tc)) << std::endl;
          }
          return true;
        };

        if (inputFile) {
          FromMtx(inputFile, consumer);
        } else {
          FromMtx(std::cin, consumer);
        }
      }
    }

    return 0;
  }

} // namespace cat
} // namespace tools
} // namespace math
} // namespace mdl

