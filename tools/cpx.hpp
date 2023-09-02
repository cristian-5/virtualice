

#ifndef COMPLEX_PURE
#define COMPLEX_PURE

#include "ice.hpp"

struct cpx { f32 r, i; };

static_assert(sizeof(cpx) == 8, "Please check the (cpx) type!");

inline u64 cpx2u64(cpx z) {
	return (static_cast<u64>(z.r) << 32) | static_cast<u64>(z.i);
}

inline constexpr cpx operator ""_i (long double i) {
	return cpx { 0, static_cast<f32>(i) };
}

inline constexpr cpx operator ""_i (unsigned long long i) {
	return cpx { 0, static_cast<f32>(i) };
}

inline constexpr cpx operator + (cpx a, cpx b) {
	return cpx { a.r + b.r, a.i + b.i };
}

inline constexpr cpx operator - (cpx a, cpx b) {
	return cpx { a.r - b.r, a.i - b.i };
}

inline constexpr cpx operator * (cpx a, cpx b) {
	return cpx { a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r };
}

inline constexpr cpx operator / (cpx a, cpx b) {
	return cpx {
		(a.r * b.r + a.i * b.i) / (b.r * b.r + b.i * b.i),
		(a.i * b.r - a.r * b.i) / (b.r * b.r + b.i * b.i)
	};
}

inline constexpr cpx operator + (f32 a, cpx b) {
	return cpx { a + b.r, b.i };
}

inline constexpr cpx operator - (f32 a, cpx b) {
	return cpx { a - b.r, -b.i };
}

inline constexpr cpx operator * (f32 a, cpx b) {
	return cpx { a * b.r, a * b.i };
}

inline constexpr cpx operator / (f32 a, cpx b) {
	return cpx {
		( a * b.r) / (b.r * b.r + b.i * b.i),
		(-a * b.i) / (b.r * b.r + b.i * b.i)
	};
}

inline constexpr cpx operator + (cpx a, f32 b) {
	return cpx { a.r + b, a.i };
}

inline constexpr cpx operator - (cpx a, f32 b) {
	return cpx { a.r - b, a.i };
}

inline constexpr cpx operator * (cpx a, f32 b) {
	return cpx { a.r * b, a.i * b };
}

inline constexpr cpx operator / (cpx a, f32 b) {
	return cpx { a.r / b, a.i / b };
}

inline constexpr cpx operator - (cpx a) {
	return cpx { -a.r, -a.i };
}

inline cpx operator += (cpx & a, cpx b) {
	return a = a + b;
}

inline cpx operator -= (cpx & a, cpx b) {
	return a = a - b;
}

/// complex conjugate
inline constexpr cpx operator ~ (cpx z) {
	return cpx { z.r, -z.i };
}

/// field norm (absolute square)
inline f32 norm(cpx z) {
	return z.r * z.r + z.i * z.i;
}

/// euclidean norm
inline f32 absolute(cpx z) {
	return sqrt(norm(z));
}

/// normalization (cpx / |cpx|)
inline cpx operator ! (cpx z) {
	return z / absolute(z);
}

#endif
