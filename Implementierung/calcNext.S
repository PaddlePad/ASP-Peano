.intel_syntax noprefix
.global calcNext

.text

mov rax, 1
calcNext: // rdi = Grad, rsi curr array pointer

    // wir versuchen die arrays jedes mal zu erstellen
    // wenn wir sie brauchen

    // momentane Länge -> rax
    mul rax, 9

    // array -> rsi
    // step nach oben
    mov qword ptr[rsi + rax * 8], 0

    