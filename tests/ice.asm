
#ruledef {

	virtualice {version: u8} => 0x696365 @ version`8

	halt => 0x00
	; rest => 0x01 ; or 0xFF for completeness

	const.b {value} => 0x02 @ value`8
	const.w {value} => 0x03 @ value`16
	const.d {value} => 0x04 @ value`32 ; equivalent to `address`
	const.q {value} => 0x05 @ value`64

	; for completeness, do not use as they're not optimized
	const.n {value} => 0x05 @ value`64
	const.i {value} => 0x05 @ value`64
	const.r {value} => 0x05 @ value`64
	const.c {value} => 0x05 @ value`64

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

	add.n => 0x10
	add.i => 0x11 ; for completeness (same as add.n)
	add.r => 0x12
	add.c => 0x13

	sub.n => 0x14
	sub.i => 0x15 ; for completeness (same as sub.n)
	sub.r => 0x16
	sub.c => 0x17

	mul.n => 0x18
	mul.i => 0x19
	mul.r => 0x1A
	mul.c => 0x1B

	div.n => 0x1C
	div.i => 0x1D
	div.r => 0x1E
	div.c => 0x1F

	mod.n => 0x20
	mod.i => 0x21
	mod.r => 0x22
	; mod.c => 0x23 ; not implemented

	pow.n => 0x24
	pow.i => 0x25
	pow.r => 0x26
	; pow.c => 0x27 ; not implemented

	inc.n => 0x28
	inc.i => 0x29   ; for completeness (same as inc.n)
	inc.r => 0x30
	; inc.c => 0x31 ; not implemented

	dec.n => 0x32
	dec.i => 0x33   ; for completeness (same as dec.n)
	dec.r => 0x34
	; dec.c => 0x35 ; not implemented

	; ==========================================

	magnitude => 0x36
	conjugate => 0x37
	combine   => 0x38
	project   => 0x39
	project.r => 0x3A
	project.i => 0x3B
	imaginary => 0x3C

	convert.n2r => 0x3D
	convert.i2r => 0x3E
	convert.r2i => 0x3F

	; ==========================================

	mask.b => 0x40
	mask.w => 0x41
	mask.d => 0x42
	mask.q => 0x43

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

	; empty instructions 0x58 ... 0x5F

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

	jump.l.n  {address: u32} => 0x60 @ address`32
	jump.l.i  {address: u32} => 0x61 @ address`32
	jump.l.r  {address: u32} => 0x62 @ address`32
	; jump.l.c => 0x63 not implemented
	jump.le.n {address: u32} => 0x64 @ address`32
	jump.le.i {address: u32} => 0x65 @ address`32
	jump.le.r {address: u32} => 0x66 @ address`32
	; jump.le.c => 0x67 not implemented
	jump.n.ge {address: u32} => 0x68 @ address`32
	jump.i.ge {address: u32} => 0x69 @ address`32
	jump.r.ge {address: u32} => 0x6A @ address`32
	; jump.c.ge => 0x6B not implemented
	jump.n.g  {address: u32} => 0x6C @ address`32
	jump.i.g  {address: u32} => 0x6D @ address`32
	jump.r.g  {address: u32} => 0x6E @ address`32
	; jump.c.g  => 0x6F not implemented

	compare.e  {address: u32} => 0x70 @ address`32
	compare.ne {address: u32} => 0x71 @ address`32

	compare.l.n  {address: u32} => 0x72 @ address`32
	compare.l.i  {address: u32} => 0x73 @ address`32
	compare.l.r  {address: u32} => 0x74 @ address`32
	; compare.l.c => 0x75 not implemented
	compare.le.n {address: u32} => 0x76 @ address`32
	compare.le.i {address: u32} => 0x77 @ address`32
	compare.le.r {address: u32} => 0x78 @ address`32
	; compare.le.c => 0x79 not implemented
	compare.n.ge {address: u32} => 0x80 @ address`32
	compare.i.ge {address: u32} => 0x81 @ address`32
	compare.r.ge {address: u32} => 0x82 @ address`32
	; compare.c.ge => 0x83 not implemented
	compare.n.g  {address: u32} => 0x84 @ address`32
	compare.i.g  {address: u32} => 0x85 @ address`32
	compare.r.g  {address: u32} => 0x86 @ address`32
	; compare.c.g  => 0x87 not implemented

	; empty instructions 0x88, 0x89

	; ==========================================

	global.r {index: u8} => 0x8A @ index`8
	global.w {index: u8} => 0x8B @ index`8
	global.e.r {index: u16} => 0x8C @ index`16
	global.e.w {index: u16} => 0x8D @ index`16

	local.r {index: u8} => 0x8E @ index`8
	local.w {index: u8} => 0x8F @ index`8

	local.r.0 => 0x90
	local.r.1 => 0x91
	local.r.2 => 0x92
	local.r.3 => 0x93
	local.r.4 => 0x94
	local.r.5 => 0x95
	local.r.6 => 0x96
	local.r.7 => 0x97

	local.w.0 => 0x98
	local.w.1 => 0x99
	local.w.2 => 0x9A
	local.w.3 => 0x9B
	local.w.4 => 0x9C
	local.w.5 => 0x9D
	local.w.6 => 0x9E
	local.w.7 => 0x9F

	; ==========================================

	arity {code: u8} => 0xA0 @ code`8
	arity.0 => 0xFF ; just in case, for completeness
	arity.1 => 0xA1
	arity.2 => 0xA2
	arity.3 => 0xA3
	arity.4 => 0xA4
	arity.5 => 0xA5
	arity.6 => 0xA6
	arity.7 => 0xA7
	arity.8 => 0xA8

	; empty instructions 0xA9 ... 0xAD

	; ==========================================

	param.r {index: u8} => 0xAE @ index`8
	param.w {index: u8} => 0xAF @ index`8

	param.r.0 => 0xB0
	param.r.1 => 0xB1
	param.r.2 => 0xB2
	param.r.3 => 0xB3
	param.r.4 => 0xB4
	param.r.5 => 0xB5
	param.r.6 => 0xB6
	param.r.7 => 0xB7

	param.w.0 => 0xB8
	param.w.1 => 0xB9
	param.w.2 => 0xBA
	param.w.3 => 0xBB
	param.w.4 => 0xBC
	param.w.5 => 0xBD
	param.w.6 => 0xBE
	param.w.7 => 0xBF

	; ==========================================

	address {address: u32} => 0x04 @ address`32 ; equivalent to `const.d`

	call.k {code: u8} => 0xC0 @ code`8
	; empty instructions 0xC1 ... 0xC6
	call.l => 0xC7
	; empty instructions 0xC8 ... 0xC9
	call {address: u32} => 0xCA @ address`32

	return   => 0xCB ; return (Call Back) a value
	return.v => 0xCC ; return void

	; === MATH FUNCTIONS =======================

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

	rest => 0xFF ; or 0x01 for completeness

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
