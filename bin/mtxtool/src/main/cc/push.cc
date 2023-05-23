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
namespace push {

  using util::cli::GetOpts;
  using util::functional::Assign;
  using util::functional::SupplierIterable;

  const char* outputFileName = nullptr;
  bool help = false;
  GetOpts opts(Assign(&outputFileName));

  void PrintUsage() {
    std::cerr << 
R"(usage: mtxtool push <output-file>
where:
  stdin          Matrix to push is read from stdin.
  <output-file>  Matrix file, in MTX format, to push to.
  --help         Shows this help message.
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption("help", Assign(&help, true));

    return opts.Parse(args, argc);
  }

  std::string GetTemporaryFileName(const char* fileName) {
    if (!fileName) { return std::string(); }

    auto tempFile = std::filesystem::path(fileName);
    return tempFile.parent_path().string() 
        + "/." + tempFile.filename().c_str() + ".tmp";
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

    if (!outputFileName) {
      std::cerr << "error: output file name not provided" << std::endl;
      return 2;
    }

    std::string tempFileName = GetTemporaryFileName(outputFileName);
    auto toPushStream = FromMtxStream(&std::cin);
    std::unique_ptr<Matrix> mat;
    std::function<std::unique_ptr<Matrix>()> outputStream = []() { return std::unique_ptr<Matrix>(); };
    auto path = std::filesystem::path(outputFileName);
    if (std::filesystem::exists(path)) {
      outputStream = FromMtxStream(outputFileName);
    }

    SaveMtx(tempFileName.c_str(), [&toPushStream, &mat, &outputStream]() -> const Matrix* {
      mat = toPushStream();
      if (mat) {
        return mat.get();
      }
      mat = outputStream();
      return mat.get();
    });    

    std::filesystem::rename(tempFileName, outputFileName);

    return 0;
  }
} // namespace push
} // namespace tools
} // namespace math
} // namespace mdl

