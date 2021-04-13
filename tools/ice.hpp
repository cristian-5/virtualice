
#ifndef ICE_PURE
#define ICE_PURE

#include <cstdint>
#include <cstdlib>
#include <cstdio>

using chr = char;
using bln = bool;
using u8  = uint_fast8_t;
using u16 = uint_fast16_t;
using u32 = uint_fast32_t;
using u64 = uint_fast64_t;
using siz = uint_fast64_t;
using i8  = int_fast8_t;
using i16 = int_fast16_t;
using i32 = int_fast32_t;
using i64 = int_fast64_t;
using f32 = float;
using f64 = double;

static_assert(sizeof(bln) == 1, "Please adjust the (bln) type alias!");
static_assert(sizeof(chr) == 1, "Please adjust the (chr) type alias!");
static_assert(sizeof(f32) == 4, "Please adjust the (f32) type alias!");
static_assert(sizeof(f64) == 8, "Please adjust the (f64) type alias!");

#endif
