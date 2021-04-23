
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
	pop.n {value: u64} => 0x09 @ value

	top => 0x0A

	cast.b => 0x0B
	cast.w => 0x0C
	cast.d => 0x0D
	cast.q => 0x0E

	; padding of 0x02

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

	; padding of 0x03

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

	jump {address: u64} => 0x2A @ address`64

	jump.t {address: u64} => 0x2B @ address`64
	jump.f {address: u64} => 0x2C @ address`64

	jump.i.l {address: u64} => 0x2D @ address`64
	jump.i.g {address: u64} => 0x2E @ address`64
	jump.i.e {address: u64} => 0x2F @ address`64
	jump.i.ne {address: u64} => 0x30 @ address`64
	jump.i.le {address: u64} => 0x31 @ address`64
	jump.i.ge {address: u64} => 0x32 @ address`64

	jump.f.l {address: u64} => 0x33 @ address`64
	jump.f.g {address: u64} => 0x34 @ address`64
	jump.f.e {address: u64} => 0x35 @ address`64
	jump.f.ne {address: u64} => 0x36 @ address`64
	jump.f.le {address: u64} => 0x37 @ address`64
	jump.f.ge {address: u64} => 0x38 @ address`64

}