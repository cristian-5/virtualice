
#ifndef ICE_PURE
#define ICE_PURE

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <string>
#include <type_traits>
#include <limits>
#include <bit>
#include <cmath>

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

#define lim std::numeric_limits

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

// =============== fast conversion functions ===============

class invalid_number { };

inline static chr * u2s(u64 u) {
	// max(u64) = 18446744073709551615
	// length(max(u64)) = 20
	chr s[21]; // length(max(u64)) + 1
	chr * p = s;
	if (!u) {
		p = (chr *) malloc(2);
		* p = '0';
		* (p + 1) = 0x00;
		return p;
	}
	u8 len = 0;
	do {
		* p++ = chr('0' + (u % 10));
		u /= 10;
		len++;
	} while (u > 0);
	* p = 0x00;
	chr * n = (chr *) malloc(len + 1);
	u8 i = 0;
	while (p-- > s) n[i++] = * p;
	n[i] = 0x00;
	return n;
}

inline static chr * i2s(i64 i) {
	// max(i64) = 9223372036854775807
	// min(i64) = -9223372036854775808
	// length(max(i64)) = 19
	chr s[21]; // length(max(i64)) + 1 + 1 (sign)
	u64 u; bln neg = i < 0;
	if (neg) u = -i;
	else u = i;
	chr * p = s;
	if (!u) {
		p = (chr *) malloc(2);
		* p = '0';
		* (p + 1) = 0x00;
		return p;
	}
	u8 len = 0;
	do {
		* p++ = chr('0' + (u % 10));
		u /= 10;
		len++;
	} while (u > 0);
	if (neg) * p++ = '-';
	* p = 0x00;
	chr * n = (chr *) malloc(len + 1 + (neg ? 1 : 0));
	u8 j = 0;
	while (p-- > s) n[j++] = * p;
	n[j] = 0x00;
	return n;
}

inline static chr * f2s(f64 f) {
	chr * s = (chr *) malloc(100);
	sprintf(s, "%f", f);
	return s;
}

inline static u64 s2u(chr * s) {
	u64 u = 0;
	for (i8 i = 0; i < 16; i++) {
		if (!std::isdigit(s[i])) {
			throw invalid_number();
		}
		u = u * 10 + (s[i] - '0');
	} return u;
}

inline static i64 s2i(chr * s) {
	if (* s == '-') return - s2u(++s);
	else if (* s == '+') s++;
	return s2u(s);
}

// parses /[+-]?\d+(?:\.\d+)?(?:e[-+]?\d+)?/g
inline static f64 s2f(chr * s) {
	bln negative = (* s == '-');
	bln positive = (* s == '+');
	// + infinity == infinity
	if (!strcmp(s, "infinity\0")) {
		if (negative) return - lim<f64>::infinity();
		return lim<f64>::infinity();
	}
	// + undefined != undefined != - undefined
	if (!strcmp(s, "undefined\0")) {
		if (negative) return - lim<f64>::quiet_NaN();
		if (positive) return + lim<f64>::quiet_NaN();
		return lim<f64>::quiet_NaN();
	}
	try { return std::stod(s); }
	catch (...) { throw invalid_number(); }
}

#endif
