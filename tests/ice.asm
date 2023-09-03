
#ruledef {

	halt => 0x00
	rest => 0x01

	push.b {value} => 0x02 @ value`8
	push.w {value} => 0x03 @ value`16
	push.d {value} => 0x04 @ value`32
	push.q {value} => 0x05 @ value`64

	push.a {value} => 0x04 @ value`32

	push.z => 0x06
	push.o => 0x07

	pop => 0x08
	pop.n {number: u16} => 0x09 @ number`16

	top => 0x0A

	cast.b => 0x0B
	cast.w => 0x0C
	cast.d => 0x0D
	cast.q => 0x0E

	; ==========================================

	factorial => 0x0F

	add.i => 0x10
	add.f => 0x11
	sub.i => 0x12
	sub.f => 0x13
	mul.i => 0x14
	mul.f => 0x15
	div.i => 0x16
	div.f => 0x17
	mod.i => 0x18
	mod.f => 0x19
	pow.i => 0x1A
	pow.f => 0x1B

	increment => 0x1C
	decrement => 0x1D

	; ==========================================

	convert.to.i => 0x1E
	convert.to.f => 0x1F

	; ==========================================

	and => 0x20
	or  => 0x21
	not => 0x22
	xor => 0x23

	invert => 0x24
	complement => 0x25

	; 64 = 2^6, 6 bits are enough.

	shift.r {value: u6} => 0x26 @ value`8
	shift.l {value: u6} => 0x27 @ value`8
	rotate.r {value: u6} => 0x28 @ value`8
	rotate.l {value: u6} => 0x29 @ value`8

	; ==========================================

	jump {address: u32} => 0x2A @ address`32

	jump.t {address: u32} => 0x2B @ address`32
	jump.f {address: u32} => 0x2C @ address`32

	; ==========================================

	raise => 0x2D
	flag  => 0x2E

	; ==========================================

	swap => 0x2F

	; === PADDING OF 10 INSTRUCTIONS ===========

	compare.u.e  => 0x3A
	compare.u.ne => 0x3B
	compare.u.l  => 0x3C
	compare.u.le => 0x3D
	compare.u.g  => 0x3E
	compare.u.ge => 0x3F

	; === PADDING OF 10 INSTRUCTIONS ===========

	compare.s.e  => 0x4A
	compare.s.ne => 0x4B 
	compare.s.l  => 0x4C
	compare.s.le => 0x4D
	compare.s.g  => 0x4E
	compare.s.ge => 0x4F

	; === PADDING OF 10 INSTRUCTIONS ===========

	compare.f.e  => 0x5A
	compare.f.ne => 0x5B
	compare.f.l  => 0x5C
	compare.f.le => 0x5D
	compare.f.g  => 0x5E
	compare.f.ge => 0x5F

	; ==========================================

	compare.i.e  => 0x3A    ; for completeness
	compare.i.ne => 0x3B    ; for completeness
	compare.f.e  => 0x5A    ; for completeness
	compare.f.ne => 0x5B    ; for completeness

	; === PADDING OF 10 INSTRUCTIONS ===========

	call.k {code: u8} => 0x6A @ code`8
	call {address: u32} => 0x6B @ address`32
	call.l => 0x6C
	return => 0x6D

	; === PADDING OF 01 INSTRUCTIONS ===========

	negate.i => 0x25   ; for completeness
	negate.f => 0x6F

	; ==========================================

	global.b.c {name: u8} => 0x70 @ name`8
	global.b.d {name: u8} => 0x71 @ name`8
	global.b.g {name: u8} => 0x72 @ name`8
	global.b.s {name: u8} => 0x73 @ name`8

	global.w.c {name: u16} => 0x74 @ name`16
	global.w.d {name: u16} => 0x75 @ name`16
	global.w.g {name: u16} => 0x76 @ name`16
	global.w.s {name: u16} => 0x77 @ name`16

	local.b.c {name: u8} => 0x78 @ name`8
	local.b.d {name: u8} => 0x79 @ name`8
	local.b.g {name: u8} => 0x7A @ name`8
	local.b.s {name: u8} => 0x7B @ name`8

	local.w.c {name: u16} => 0x7C @ name`16
	local.w.d {name: u16} => 0x7D @ name`16
	local.w.g {name: u16} => 0x7E @ name`16
	local.w.s {name: u16} => 0x7F @ name`16

	param.b {name: u8} => 0x78 @ name`8   ; for completeness
	param.w {name: u16} => 0x7C @ name`16 ; for completeness

	; padding of 9 instructions

	scope.c => 0x8C
	scope.d => 0x8D

	; padding of ALOT instructions

	; === COMPLEX INSTRUCTIONS =================

	complex.f => 0xC0
	complex.i => 0xC1
	
	project   => 0xC2 ; project to 2 f values
	project.r => 0xC3 ; project to real part
	project.i => 0xC4 ; project to imaginary part

	magnitude => 0xC5

	; padding of 4 instructions
	
	add.c => 0xCA
	sub.c => 0xCB
	mul.c => 0xCC
	div.c => 0xCD

	conjugate => 0xCE

	; === MATH FUNCTIONS =======================

	math.i       => 0xCF
	math.e       => 0xD0
	math.ln10    => 0xD1
	math.ln2     => 0xD2
	math.log10e  => 0xD3
	math.log2e   => 0xD4
	math.pi      => 0xD5
	math.sqrt1_2 => 0xD6
	math.sqrt2   => 0xD7
	math.egamma  => 0xD8
	math.phi	 => 0xD9

	math.abs   => 0xDA
	math.acos  => 0xDB
	math.acosh => 0xDC
	math.asin  => 0xDD
	math.asinh => 0xDE
	math.atan  => 0xDF
	math.atan2 => 0xE0
	math.atanh => 0xE1
	math.cbrt  => 0xE2
	math.ceil  => 0xE3
	math.cos   => 0xE4
	math.cosh  => 0xE5
	math.exp   => 0xE6
	math.expm1 => 0xE7
	math.floor => 0xE8
	math.hypot => 0xE9
	math.log   => 0xEA
	math.log1p => 0xEB
	math.log10 => 0xEC
	math.log2  => 0xED
	math.max   => 0xEE
	math.min   => 0xEF
	math.pow   => 0xF0
	math.round => 0xF1
	math.sign  => 0xF2
	math.sin   => 0xF3
	math.sinh  => 0xF4
	math.sqrt  => 0xF5
	math.tan   => 0xF6
	math.tanh  => 0xF7
	math.trunc => 0xF8

	; ==========================================

}

estream = 0xE5
ostream = 0x05
edata   = 0xED
odata   = 0x0D
eflush  = 0xEF
oflush  = 0x0F
istream = 0x15

fork    = 0xF0
join    = 0x10
sleep   = 0x57
wait    = 0xA1
lock    = 0x88
release = 0x33

u2s = 0xCA
i2s = 0xCB
s2u = 0xCC
s2i = 0xCD
f2s = 0xCE
s2f = 0xCF

allocate   = 0xA7
deallocate = 0xDA
reallocate = 0x2A
copy       = 0xC0
load       = 0x70
zeros      = 0x00
fill       = 0xF1
compare    = 0xC2

debug  = 0xDE
sign   = 0x51
time   = 0x71
seed   = 0x5E
random = 0x3A
