
#ifndef ARR_PURE
#define ARR_PURE

#include "ice.hpp"

template <typename TYP>
struct arr {
	TYP * data = nullptr;
	u64 count = 0;
};

#endif
