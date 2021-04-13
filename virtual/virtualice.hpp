
#ifndef VIRTUALICE_HPP
#define VIRTUALICE_HPP

#include "../tools/ice.hpp"
#include "../tools/arr.hpp"
#include "../tools/stk.hpp"

enum class con: u8 { z = 0b0, o = 0b1 };
enum class typ: u8 { i = 0b0, f = 0b1 };
enum class sid: u8 { r = 0b0, l = 0b1 };
enum class reg: u8 { q = 0b00, d = 0b01, w = 0b11, b = 0b10 };

#define BYTECODE static constexpr u8
#define TYPE template <u8 T = 0>

class op {

	public:

};

class vm {

	public:

	/*!
	 *    + == VM usage ========================= +
	 *    | The code arr<u8> must end with a halt |
	 *    | instruction. The control flow will be |
	 *    | passed to threads when encountering a |
	 *    | fork instruction.                     |
	 *    + ------------------------------------- +
	!*/
	
	void run(arr<u8> code) {
		
	}

};

#undef BYTECODE
#undef TYPE

#endif
