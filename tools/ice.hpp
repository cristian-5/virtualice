
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
#define mtx std::mutex
#define str std::string
#define vec std::vector

#define lim std::numeric_limits

using std::fputc;
using std::fputs;
using std::perror;

class exc { };

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

template< typename... Args >
str format(const chr * format, Args ... args) {
	siz length = snprintf(nullptr, 0, format, args ...);
	assert(length >= 0);
	chr * buf = new chr[length + 1];
	snprintf(buf, length + 1, format, args ...);
	str result(buf);
	delete [] buf;
	return result;
}

inline static u64 factorial(u64 n) {
	static const u64 factorials[] = {
		1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800,
		479001600, 6227020800, 87178291200, 1307674368000, 20922789888000,
		355687428096000, 6402373705728000, 121645100408832000,
		2432902008176640000
	};
	if (n < sizeof(factorials) / sizeof(u64)) return factorials[n];
	u64 result = n;
	while (--n) result *= n;
	return result;
}

// =============== fast conversion functions ===============

class invalid_format: exc { };

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
	while (* s) {
		if (!std::isdigit(* s)) {
			throw invalid_format();
		}
		u = u * 10 + (* s - '0');
		s++;
	} return u;
}

inline static u64 d2u(chr * s) {
	try { return s2u(s); }
	catch (invalid_format &) { throw; }
}

inline static u64 b2u(chr * s) {
	u64 u = 0;
	while (* s) {
		if (* s != '1' && * s != '0') {
			throw invalid_format();
		}
		u = (u << 1) | (* s - '0');
		s++;
	} return u;
}

inline static u64 o2u(chr * s) {
	u64 u = 0;
	while (* s) {
		if (* s < '0' || * s > '7') {
			throw invalid_format();
		}
		u = (u << 3) | (* s - '0');
		s++;
	} return u;
}

inline static u64 x2u(chr * s) {
	u64 u = 0;
	while (* s) {
		if (!std::isxdigit(* s)) {
			throw invalid_format();
		}
		u = (u << 4) | (
			* s <= '9' ?
			* s - '0' :
			(* s & 0xDF) - 'A' + 10
		); s++;
	} return u;
}

inline static i64 s2i(chr * s) {
	try {
		if (* s == '-') return - s2u(++s);
		else if (* s == '+') s++;
		return s2u(s);
	} catch (invalid_format &) { throw; }
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
	catch (...) { throw invalid_format(); }
}

inline static chr * u2x(u64 u) {
	str result = format("0x%llX", u);
	chr * copy = (chr *) malloc(result.length() + 1);
	strcpy(copy, result.c_str());
	return copy;
}

inline static chr * f2x(f64 f) {
	u64 * u = (u64 *) & f;
	str result = format("0x%016llX", * u);
	chr * copy = (chr *) malloc(result.length() + 1);
	strcpy(copy, result.c_str());
	return copy;
}

#endif
