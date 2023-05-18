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

#include <mdl/text.h>
#include <mdl/util.h>
#include <mdl/matrix.h>


namespace mdl {
namespace math {
namespace tools {
namespace split {

  using mdl::text::ParseInt;
  using util::cli::GetOpts;
  using util::functional::Assign;
  using util::functional::SupplierIterable;

  const char* inputFileName = nullptr;
  bool help = false;
  math::size_t height = math::kMaxSizeT;
  math::size_t width = math::kMaxSizeT;
  GetOpts opts(Assign(&inputFileName));

  void PrintUsage() {
    std::cout << 
R"(usage: mtxtool split [<file>]
where:
  <file>       Default to stdin if not provided
  -w,--width   Grid width
  -h,--height  Grid height
  --help       Shows this help message.
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption("help", Assign(&help, true));
    opts.AddOption('h', "height", Assign(&height, ParseInt));
    opts.AddOption('w', "width", Assign(&width, ParseInt));

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

    height = std::max(1, height);
    width = std::max(1, width);

    std::string outputFileName("/dev/stdout");
    if (inputFileName) {
      auto file = std::filesystem::path(inputFileName);
      if (!std::filesystem::exists(file)) {
        std::cout << "error: file not found: " << inputFileName << std::endl;
        return 2;
      }
      if (!std::filesystem::is_regular_file(file)) {
        std::cout << "error: not a regular file: " << inputFileName << std::endl;
        return 3;
      }

      auto inputFile = std::filesystem::path(inputFileName);
      outputFileName = inputFile.parent_path().string() 
          + "/." + inputFile.filename().c_str() + ".tmp";
    }

    std::unique_ptr<Matrix> mat;
    Matrix split;

    int idx = 0;
    size_t row = 0;
    size_t col = 0;
    SaveMtx(
        outputFileName.c_str(), 
        [supplier = FromMtxStream(inputFileName ? inputFileName : "/dev/stdin"), 
            &mat, &split, &idx, &row, &col]() mutable {
      if (idx == 0) {
        if (!mat) { mat = supplier(); }

        if (mat && row < mat->NumRows()) {
          split = (*mat)(Range(row, row + height), Range(col, col + width));
          col += width;
          if (col >= mat->NumCols()) {
            col = 0;
            row += height;
          }
          return &split;
        } else {
          idx++;
        }
      }

      mat = supplier();
      if (mat) {
        return mat.get();
      }
      return static_cast<Matrix *>(nullptr);
    });

    if (inputFileName) {
      std::filesystem::rename(outputFileName, inputFileName);
    }

    return 0;
  }
} // namespace split
} // namespace tools
} // namespace math
} // namespace mdl

