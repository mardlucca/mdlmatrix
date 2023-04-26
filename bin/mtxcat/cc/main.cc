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

#include <mdl/text.h>
#include <mdl/util.h>
#include <mdl/matrix.h>

namespace mdl {
namespace math {
namespace tools {
  using util::GetOpts;
  using util::functional::AsFunction;
  using util::functional::Assign;
  using util::functional::Set;
  using text::ParseInt;

  const char* inputFile = "/dev/stdin";
  bool help = false;
  int fr = 0;
  int fc = 0;
  int tr = 10;
  int tc = 10;

  GetOpts opts(Assign(&inputFile));

  void PrintUsage() {
    std::cout << 
R"(usage: mtxcat [-h] file
where:
  --fr  From row. Defaults to 0.
  --fc  From col. Defaults to 0.
  --tr  To row, not including last. Defaults to 10.
  --tc  To col, not including last. Defaults to 10.
)" << std::endl;
  }

  bool ParseArgs(const char** args, int argc) {
    opts.AddOption('h', Set(&help, true));
    opts.AddOption("fr", Assign(&fr, AsFunction(ParseInt)));
    opts.AddOption("fc", Assign(&fc, AsFunction(ParseInt)));
    opts.AddOption("tr", Assign(&tr, AsFunction(ParseInt)));
    opts.AddOption("tc", Assign(&tc, AsFunction(ParseInt)));

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

    FromMtx(inputFile, [](Matrix&& matrix) {
      std::cout << matrix(Range(fr, tr), Range(fc, tc)) << std::endl;
      return true;
    });
    return 0;
  }

} // namespace tools
} // namespace math
} // namespace mdl

int main(int argc, const char** args) {
  return mdl::math::tools::DoMain(args, argc);
}

