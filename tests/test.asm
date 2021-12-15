
#include "ice.asm"

main:

    push.a function_1
    global.b.c $inc
    push.z
    local.b.g $inc
    call.l
    call.k debug
    pop
    push.a function_2
    local.b.s $inc
    push.b 5
    local.b.g $inc
    call.l
    call.k debug
    pop

    halt

function_1:

    param.b $x
    local.b.g $x
    push.o
    add.i
    return

function_2:

    param.b $x
    local.b.g $x
    push.b 2
    add.i
    return


table_of_names:

    $inc = 0x01
    $x = 0x00
