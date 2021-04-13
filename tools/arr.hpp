
#ifndef ARR_PURE
#define ARR_PURE

#include "ice.hpp"

template <typename TYP>
struct arr {
	TYP * data = nullptr;
	siz count = 0;
};

#endif
