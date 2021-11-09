
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

	compare.i.l  => 0x2D
	compare.i.g  => 0x2E
	compare.i.e  => 0x2F
	compare.i.ne => 0x30
	compare.i.le => 0x31
	compare.i.ge => 0x32

	compare.f.l  => 0x33
	compare.f.g  => 0x34
	compare.f.e  => 0x35
	compare.f.ne => 0x36
	compare.f.le => 0x37
	compare.f.ge => 0x38

	; ==========================================

	swap => 0x39

	; ==========================================

	call {address: u32} => 0x3A @ address`32
	call.l => 0x3B
	call.k {code: u8} => 0x3C @ code`8

	return => 0x3D

	arity {number: u8} => 0x3E @ number`8

	; ==========================================

	negate.i => 0x25   ; for completeness
	negate.f => 0x3F

	; ==========================================

	get.g {index: u16} => 0x40 @ index`16
	get.l {index: u8} => 0x41 @ index`8
	get.a {index: u8} => 0x42 @ index`8

	set.g {index: u16} => 0x43 @ index`16
	set.l {index: u8} => 0x44 @ index`8
	set.a {index: u8} => 0x45 @ index`8

	; ==========================================

	raise => 0x46
	flag  => 0x47

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
