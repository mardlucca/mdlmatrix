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

namespace mdl {
namespace math {
namespace tools {
  namespace append {
    int Main(const char** args, int argc);
  }
  namespace cat {
    int Main(const char** args, int argc);
  }
  namespace empty {
    int Main(const char** args, int argc);
  }
  namespace head {
    int Main(const char** args, int argc);
  }
  namespace push {
    int Main(const char** args, int argc);
  }
  namespace pop {
    int Main(const char** args, int argc);
  }
  namespace ones {
    int Main(const char** args, int argc);
  }
  namespace rand {
    int Main(const char** args, int argc);
  }
  namespace shuffle {
    int Main(const char** args, int argc);
  }
  namespace tail {
    int Main(const char** args, int argc);
  }
  namespace zeros {
    int Main(const char** args, int argc);
  }

  int PrintUsage(int result = 0) {
    std::cout << R"(Matrix file (e.g. mtx) manipulation tool.

Usage: 
  mtxtool <command> [<command-options>]

Commands:
  append
  cat
  empty
  head
  push
  pop
  opnes
  rand
  shuffle
  tail
  zeros

Use "mtxtool <command> --help for more information about a given command"
)" << std::endl;
    return result;
  }

  int DoMain(const char** args, int argc) {
    mdl::util::cli::CommandSwitch commands([](const char* command) {
      if (!command) {
        return PrintUsage();
      }

      std::cout << "error: unknown command '" << command << "' for mtxtool";
      return 100;
    });

    return commands
      .AddCommand("append", mdl::math::tools::append::Main)
      .AddCommand("cat", mdl::math::tools::cat::Main)
      .AddCommand("empty", mdl::math::tools::empty::Main)
      .AddCommand("head", mdl::math::tools::head::Main)
      .AddCommand("push", mdl::math::tools::push::Main)
      .AddCommand("pop", mdl::math::tools::pop::Main)
      .AddCommand("ones", mdl::math::tools::ones::Main)
      .AddCommand("rand", mdl::math::tools::rand::Main)
      .AddCommand("shuffle", mdl::math::tools::shuffle::Main)
      .AddCommand("tail", mdl::math::tools::tail::Main)
      .AddCommand("zeros", mdl::math::tools::zeros::Main)
      .Go(args, argc);
  }
} // namespace tools
} // namespace math
} // namespace mdl



int main(int argc, const char** args) {

  return mdl::math::tools::DoMain(args, argc);
}
