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
  using util::cli::GetOpts;

  GetOpts opts;
  const char* inputFiles = "/dev/stdin";
  const char* outputFile = "/dev/stdout";
  bool skipFirstLine = false;
  bool help = false;

  void PrintUsage() {
    std::cout << 
R"(usage: csv2mtx [-i <file>|-o <file>|-s|-h]
where:
  -i  Input file name. Default to stdin.
  -o  Output file name. Defaults to stdout.
  -s  Whether or not to skip the first line in the CSV file. Defaults to false.
  -h  Shows this help message.
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption('h', []() {
      help = true;
    });
    opts.AddOption('i', [](const char* value) {
      inputFiles = value;
    });
    opts.AddOption('o', [](const char* value) {
      outputFile = value;
    });
    opts.AddOption('s', []() {
      skipFirstLine = true;
    });

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

    Matrix m = mdl::math::Matrices::FromCsv(inputFiles, skipFirstLine);
    mdl::math::SaveMtx(outputFile, m);

    return 0;
  }

} // namespace tools
} // namespace math
} // namespace mdl

int main(int argc, const char** args) {
  return mdl::math::tools::DoMain(args, argc);
}

