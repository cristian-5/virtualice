
#ifndef ICE_PURE
#define ICE_PURE

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <string>
#include <type_traits>
#include <bit>

using byt = char;
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

#define map std::unordered_map
#define str std::string
#define vec std::vector

using std::fputc;
using std::fputs;
using std::perror;

static_assert(sizeof(bln) == 1, "Please check the (bln) type alias!");
static_assert(sizeof(chr) == 1, "Please check the (chr) type alias!");
static_assert(sizeof(f32) == 4, "Please check the (f32) type alias!");
static_assert(sizeof(f64) == 8, "Please check the (f64) type alias!");

static_assert(sizeof(time_t) == 8, "Please check the (time) type size!");

template <typename TYP>
static constexpr TYP rotateL(TYP v, u8 n = 1) {
	static_assert(std::is_unsigned<TYP>::value);
	return (v << n) | (v >> (sizeof(TYP) * 8 - n));
}

template <typename TYP>
static constexpr TYP rotateR(TYP v, u8 n = 1) {
	static_assert(std::is_unsigned<TYP>::value);
	return (v >> n) | (v << (sizeof(TYP) * 8 - 1));
}

static constexpr bln little_endian = std::endian::native == std::endian::little;
static constexpr bln big_endian = std::endian::native == std::endian::big;

[[gnu::always_inline]]
static inline void e_stream(void * p) {
	if (fputs(static_cast<const chr *>(p), stderr) == EOF) {
		perror("fputs()");
	}
}
[[gnu::always_inline]]
static inline void o_stream(void * p) {
	if (fputs(static_cast<const chr *>(p), stdout) == EOF) {
		perror("fputs()");
	}
}

[[gnu::always_inline]]
static inline chr * sign(u64 n) {
	static const chr * a = "-IRNYSGTBPABCDEF";
	chr * s = (chr *)malloc(17);
	for (i8 i = 15; i >= 0; i--) {
		s[15 - i] = a[(n >> (4 * i)) & 0xF];
	} s[16] = 0x00; return s;
}

[[gnu::always_inline]]
static inline str chr_2_str(chr c) {
	str s; s.push_back(c); return s;
}

#endif
