
#include "ice.asm"

eulers_identity:

    ; e^(i*pi) + 1 = 0
    ; e^(i*theta) = cos(theta) + i*sin(theta)

    math.pi        ; push pi
    math.cos       ; cos(pi)

    math.pi        ; push pi
    math.sin       ; sin(pi)

    complex.f      ; make the complex (cos(pi), i * sin(pi))

    project           ; project the complex onto the 2 components
    swap              ; swap the components
    call.k    f2s     ; convert the real part to a string
    top               ; duplicate the string pointer
    call.k ostream    ; print the real part
    call.k deallocate ; deallocate the string

    push.a string_p
    call.k odata      ; print the string " + "

    call.k    f2s     ; convert the imaginary part to a string
    top               ; duplicate the string pointer
    call.k ostream    ; print the imaginary part
    call.k deallocate ; deallocate the string

    push.a string_i
    call.k odata      ; print the string "i"

    ; SHOULD PRINT: -1.000000 + 0.000000i

    halt

string_p:
	#d " + ", 0x00
	string_p_length = $ - string_p

string_i:
    #d "i", 0x00
    string_i_length = $ - string_i
