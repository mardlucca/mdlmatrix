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

#ifndef _MDL_MATH_TYPEDEFS
#define _MDL_MATH_TYPEDEFS

#include <climits>
#include <cstddef>
#include <memory>

namespace mdl {
namespace math {

  #ifdef _MDL_MATH_LARGE_MATRICES
    typedef long size_t;
    const long kMaxSizeT = LONG_MAX;
  #else
    typedef int size_t;
    const int kMaxSizeT = INT_MAX;
  #endif  

  #define _MDL_MATH_SINGLE_PRECISION
  #ifdef _MDL_MATH_SINGLE_PRECISION
    typedef float float_t;
    const float_t kFloatPrecision = 1e-7;
    const bool kDoublePrecision = false;
  #else
    typedef double float_t;
    const float_t kFloatPrecision = 1e-16;
    const bool kDoublePrecision = true;
  #endif

  inline float_t GetVal(const float_t * matrix, size_t cols, size_t row, size_t col) {
    return *(matrix + row * cols + col);
  }

  inline float_t& GetRef(float_t * matrix, size_t cols, size_t row, size_t col) {
    return *(matrix + row * cols + col);
  }

  typedef float single_t;
  typedef double double_t;

  single_t * ToSingleBuff(const double_t * buff, std::size_t size);
  double_t * ToDoubleBuff(const single_t * buff, std::size_t size);


} // namespace math
} // namespace mdl

#endif // TYPEDEFS