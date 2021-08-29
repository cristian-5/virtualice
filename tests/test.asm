
#include "ice.asm"

push.b 8
convert.to.f
push.b 5
convert.to.f
div.f

push.b 6
convert.to.f
push.b 7
convert.to.f
div.f

pow.f

call.k f2s
top
call.k ostream
call.k deallocate
