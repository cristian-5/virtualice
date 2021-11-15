
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

	; === PADDING OF 10 INSTRUCTIONS ===========
	
	arity {number: u8} => 0x6A @ number`8

	call {address: u32} => 0x6B @ address`32
	call.l => 0x6C
	call.k {code: u8} => 0x6D @ code`8

	return => 0x6E

	; ==========================================

	negate.i => 0x25   ; for completeness
	negate.f => 0x6F

	; ==========================================

	get.g {index: u16} => 0x70 @ index`16
	get.l {index: u8} => 0x71 @ index`8
	get.a {index: u8} => 0x72 @ index`8

	set.g {index: u16} => 0x73 @ index`16
	set.l {index: u8} => 0x74 @ index`8
	set.a {index: u8} => 0x75 @ index`8

	; ==========================================

}

estream = 0x66
ostream = 0x55
istream = 0x44

fork    = 0xF0
join    = 0xEF
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
