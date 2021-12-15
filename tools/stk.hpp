
#ifndef STK_PURE
#define STK_PURE

#include "ice.hpp"

template <typename TYP>
class stk {

	private:

	TYP * stack = nullptr;
	
	siz count = 0;
	siz maxCount = 16;

	inline void increase() {
		maxCount = maxCount << 1;
		stack = (TYP *) realloc(
			stack, maxCount * sizeof(TYP)
		);
	}

	public:

	stk() {
		maxCount = 16;
		stack = (TYP *) malloc(
			maxCount * sizeof(TYP)
		);
	}

	stk(siz dimension, TYP data) {
		maxCount = dimension ? dimension : 16;
		stack = (TYP *) malloc(
			maxCount * sizeof(TYP)
		);
	}

	~stk() { free(stack); }

	TYP at(siz index) {
		return stack[index];
	}

	void edit(siz index, TYP node) {
		stack[index] = node;
	}

	void push(TYP node) {
		if (count == maxCount) increase();
		stack[count++] = node;
	}

	TYP & top() { return stack[count - 1]; }

	TYP pop() { return stack[--count]; }
	
	void decrease() { count--; }

	void decreaseBy(siz number) { count -= number; }

	void decreaseTo(siz index) { count = index; }

	bln is_empty() { return !count; }

	siz size() { return count; }

	void clear() { count = 0; }

	TYP * iterator() { return stack; }
	
	TYP & operator [] (siz index) {
		return stack[index];
	}

};

#endif
