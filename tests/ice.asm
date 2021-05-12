
#ruledef {

	halt => 0x00
	rest => 0x01

	push.b {value} => 0x02 @ value`8
	push.w {value} => 0x03 @ value`16
	push.d {value} => 0x04 @ value`32
	push.q {value} => 0x05 @ value`64

	push.z => 0x06
	push.o => 0x07

	pop => 0x08
	pop.n {number: u16} => 0x09 @ number`16

	top => 0x0A

	cast.b => 0x0B
	cast.w => 0x0C
	cast.d => 0x0D
	cast.q => 0x0E

	; padding of 0x00

	raise => 0x0F

	; padding of 0x00

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
	inc.i => 0x1A
	inc.f => 0x1B
	dec.i => 0x1C
	dec.f => 0x1D

	; padding of 0x00

	convert.to.i => 0x1E
	convert.to.f => 0x1F

	; padding of 0x00

	and => 0x20
	or  => 0x21
	not => 0x22
	xor => 0x23

	invert => 0x24
	negate => 0x25

	; 64 = 2^6, 6 bits are enough.

	shift.r {value: u6} => 0x26 @ value`8
	shift.l {value: u6} => 0x27 @ value`8
	rotate.r {value: u6} => 0x28 @ value`8
	rotate.l {value: u6} => 0x29 @ value`8

	; padding of 0x00

	jump {address: u32} => 0x2A @ address`32

	jump.t {address: u32} => 0x2B @ address`32
	jump.f {address: u32} => 0x2C @ address`32

	jump.i.l {address: u32} => 0x2D @ address`32
	jump.i.g {address: u32} => 0x2E @ address`32
	jump.i.e {address: u32} => 0x2F @ address`32
	jump.i.ne {address: u32} => 0x30 @ address`32
	jump.i.le {address: u32} => 0x31 @ address`32
	jump.i.ge {address: u32} => 0x32 @ address`32

	jump.f.l {address: u32} => 0x33 @ address`32
	jump.f.g {address: u32} => 0x34 @ address`32
	jump.f.e {address: u32} => 0x35 @ address`32
	jump.f.ne {address: u32} => 0x36 @ address`32
	jump.f.le {address: u32} => 0x37 @ address`32
	jump.f.ge {address: u32} => 0x38 @ address`32

	jump.ex {address: u32} => 0x39 @ address`32

	; padding of 0x00

	call {address: u32} => 0x3A @ address`32
	call.l => 0x3B
	call.k {code: u8} => 0x3C @ code`8

	return => 0x3D

	arity {number: u16} => 0x3E @ number`16

	; padding of 0x01

	get.g {index: u32} => 0x40 @ index`32
	get.l {index: u16} => 0x41 @ index`16
	get.a {index: u16} => 0x42 @ index`16

	set.g {index: u32} => 0x43 @ index`32
	set.l {index: u16} => 0x44 @ index`16
	set.a {index: u16} => 0x45 @ index`16

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
