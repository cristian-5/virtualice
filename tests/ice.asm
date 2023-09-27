
#subruledef REGISTER {
	b => 0b00
	w => 0b01
	d => 0b10
	q => 0b11
}

#subruledef TYPE {
	b => 0b00
	n => 0b00
	i => 0b01
	r => 0b10
	c => 0b11
}

#subruledef ACCESS {
	r => 0x00
	w => 0x01
}

#ruledef {

	virtualice {version: u8} => 0x696365 @ version`8

	halt => 0x00 ; or 0xFF for completeness
	rest => 0x01

	const.b {value} => 0x02 @ value`8
	const.w {value} => 0x03 @ value`16
	const.d {value} => 0x04 @ value`32 ; equivalent to `address`
	const.q {value} => 0x05 @ value`64

	; for completeness, do not use as they're not optimized
	const.{t: TYPE} {value} => 0x05 @ value`64
	;const.n {value} => 0x05 @ value`64
	;const.i {value} => 0x05 @ value`64
	;const.r {value} => 0x05 @ value`64
	;const.c {value} => 0x05 @ value`64

	const.0 => 0x06
	const.f => 0x06
	const.l => 0x06
	const.1 => 0x07
	const.t => 0x07
	const.h => 0x07

	swap => 0x08

	; empty instruction 0x09

	clone.n {number} => 0x0A @ number`8
	drop.n  {number} => 0x0B @ number`8
	clone => 0x0C
	drop  => 0x0D

	; empty instructions 0x0E, 0x0F

	; ==========================================

	add.{t: TYPE} => (0x10 + t)`8
	sub.{t: TYPE} => (0x14 + t)`8
	mul.{t: TYPE} => (0x18 + t)`8
	div.{t: TYPE} => (0x1C + t)`8
	mod.{t: TYPE} => (0x20 + t)`8 ; mod.c => 0x23 ; not implemented
	pow.{t: TYPE} => (0x24 + t)`8 ; pow.c => 0x27 ; not implemented
	inc.{t: TYPE} => (0x28 + t)`8 ; inc.c => 0x31 ; not implemented
	dec.{t: TYPE} => (0x2C + t)`8 ; dec.c => 0x30 ; not implemented

	; empty instructions 0x31

	; ==========================================

	load.{r: REGISTER}   {pointer: u32}  => (0x32 + r)`8 @ pointer`32
	load.{r: REGISTER}  [{pointer: u32}] => (0x32 + r)`8 @ pointer`32
	store.{r: REGISTER}  {pointer: u32}  => (0x36 + r)`8 @ pointer`32
	store.{r: REGISTER} [{pointer: u32}] => (0x36 + r)`8 @ pointer`32

	; ==========================================

	project   => 0x3A
	project.r => 0x3B
	project.i => 0x3C
	magnitude => 0x3D
	conjugate => 0x3E
	combine   => 0x3F

	; ==========================================

	mask.{r: REGISTER} => (0x40 + r)`8

	bit   => 0x44
	bit.g => 0x44 ; for completeness
	bit.r => 0x45
	bit.0 => 0x45 ; for completeness
	bit.s => 0x46
	bit.1 => 0x46 ; for completeness
	
	nibble.l => 0x47
	nibble.h => 0x48
	nibble.s => 0x49

	buffer => 0x01 ; for completeness
	and    => 0x4A
	or     => 0x4B
	not    => 0x4C
	nor    => 0x4D
	nand   => 0x4E
	xor    => 0x4F
	xnor   => 0x50
	xand   => 0x50

	invert     => 0x51
	complement => 0x52
	reverse    => 0x53

	; 64 = 2^6 so 5 bits are enough for rotate

	rotate.r {value: u5} => 0x54 @ value`8
	rotate.l {value: u5} => 0x55 @ value`8
	shift.r  {value: u6} => 0x56 @ value`8
	shift.l  {value: u6} => 0x57 @ value`8

	; empty instructions 0x58, 0x59

	; ==========================================

	jump {address: u32} => 0x5A @ address`32

	jump.0 {address: u32} => 0x5B @ address`32
	jump.z {address: u32} => 0x5B @ address`32 ; for completeness
	jump.f {address: u32} => 0x5B @ address`32 ; for completeness
	jump.1 {address: u32} => 0x5C @ address`32
	jump.o {address: u32} => 0x5C @ address`32 ; for completeness
	jump.t {address: u32} => 0x5C @ address`32 ; for completeness
	jump.nz {address: u32} => 0x5D @ address`32

	jump.e  {address: u32} => 0x5E @ address`32
	jump.ne {address: u32} => 0x5F @ address`32

	; complex jumps are not implemented:

	jump.l.{ t: TYPE} {address: u32} => (0x60 + t)`8 @ address`32
	jump.le.{t: TYPE} {address: u32} => (0x64 + t)`8 @ address`32
	jump.ge.{t: TYPE} {address: u32} => (0x68 + t)`8 @ address`32
	jump.g.{ t: TYPE} {address: u32} => (0x6C + t)`8 @ address`32

	compare.e  {address: u32} => 0x70 @ address`32
	compare.ne {address: u32} => 0x71 @ address`32

	compare.l.{ t: TYPE} {address: u32} => (0x72 + t)`8 @ address`32
	compare.le.{t: TYPE} {address: u32} => (0x76 + t)`8 @ address`32
	compare.ge.{t: TYPE} {address: u32} => (0x80 + t)`8 @ address`32
	compare.g.{ t: TYPE} {address: u32} => (0x84 + t)`8 @ address`32

	isnan => 0x88
	isinf => 0x89

	; ==========================================

	global.{a: ACCESS} {index} => {
		assert(index >= 0)
		assert(index <= 0xFF)
		(0x8A | a)`8 @ index`8
	}
	global.{a: ACCESS} {index} => {
		assert(index > 0xFF)
		(0x8C | a)`8 @ index`16
	}

	; 2 byte access of local variables
	local.{a: ACCESS} {index: u8} => {
		assert(index >= 8)
		(0x8E | a)`8 @ index`8
	}
	; 1 byte shorthand read of local variables
	local.r {index: u8} => {
		assert(index < 8)
		(0x90 + index)`8
	}
	; 1 byte shorthand write of local variables
	local.w {index: u8} => {
		assert(index < 8)
		(0x98 + index)`8
	}

	; ==========================================

	; 2 byte arity set
	arity {code: u8} => {
		assert(code > 8)
		0xA0 @ code`8
	}
	; 1 byte shorthand arity set
	arity {code: u8} => {
		assert(code > 0)
		assert(code < 8)
		(0xA0 + index)`8 @ code`8
	}

	arity 0 => 0x01 ; just in case, for completeness

	; empty instructions 0xA9 ... 0xAD

	; ==========================================

	; 2 byte access of function parameters
	param.{a: ACCESS} {index: u8} => {
		assert(index >= 8)
		(0xAE | a)`8 @ index`8
	}
	; 1 byte shorthand read of function parameters
	param.r {index: u8} => {
		assert(index < 8)
		(0xB0 + index)`8
	}
	; 1 byte shorthand write of function parameters
	param.w {index: u8} => {
		assert(index < 8)
		(0xB8 + index)`8
	}

	; ==========================================

	address {address: u32} => 0x04 @ address`32 ; equivalent to `const.d`

	call.k {code: u8} => 0xC0 @ code`8
	; empty instructions 0xC1 ... 0xC6
	call.l => 0xC7
	; empty instructions 0xC8 ... 0xC9
	call {address: u32} => 0xCA @ address`32

	return   => 0xCB ; return (Call Back) a value
	return.v => 0xCC ; return void

	; ==========================================

	convert.n2r => 0xCD
	convert.i2r => 0xCE
	convert.r2i => 0xCF

	; === MATH FUNCTIONS =======================

	e       => 0xD0
	euler   => 0xD0 ; for completeness
	ln10    => 0xD1
	ln2     => 0xD2
	log10e  => 0xD3
	log2e   => 0xD4
	pi      => 0xD5
	sqrt1_2 => 0xD6
	sqrt2   => 0xD7
	egamma  => 0xD8
	phi	    => 0xD9

	abs   => 0xDA
	acos  => 0xDB
	acosh => 0xDC
	asin  => 0xDD
	asinh => 0xDE
	atan  => 0xDF
	atan2 => 0xE0
	atanh => 0xE1
	cbrt  => 0xE2
	ceil  => 0xE3
	cos   => 0xE4
	cosh  => 0xE5
	exp   => 0xE6
	expm1 => 0xE7
	floor => 0xE8
	hypot => 0xE9
	log   => 0xEA
	log1p => 0xEB
	log10 => 0xEC
	log2  => 0xED
	max   => 0xEE
	min   => 0xEF
	round => 0xF0
	sign  => 0xF1
	sin   => 0xF2
	sinh  => 0xF3
	sqrt  => 0xF4
	tan   => 0xF5
	tanh  => 0xF6
	tri   => 0xF7
	trunc => 0xF8

	imaginary => 0xF9

	; ==========================================

	; halt => 0xFF ; or 0x00 for completeness

}

; memory management

#const memory_grow    = 0x50
#const memory_shrink  = 0x54
#const memory_size    = 0x51
#const memory_page    = 0x2A
#const memory_pages   = 0x25
#const memory_copy    = 0xC0
#const memory_load    = 0x70
#const memory_zeros   = 0x00
#const memory_fill    = 0xF1
#const memory_compare = 0xC2
