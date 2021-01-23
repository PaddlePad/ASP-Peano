.intel_syntax noprefix
.global reverse

.text

reverse: //  in = rdi, out = rsi, size = rdx (in bleiben gleich)
//für schnellere Vergleiche evtl 0 in al oder so   
    cmp rdx, 0          //ende der Schleife
    jl .LEndReverse

    //Ziel: out[i] = (in[i] + 2) % 4;

    mov rax, qword ptr[rdi + rdx * 8]
    mov qword ptr[rsi + rdx * 8], rax   //wert von in in out schreiben
    add qword ptr[rsi + rdx * 8], 2          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp qword ptr[rsi + rdx * 8], 3
    jle .LoopReverse
    sub qword ptr[rsi + rdx * 8], 4         //(out[i] + 2) % 4
.LoopReverse:    
    dec rdx
    jmp reverse

.LEndReverse:
ret
    //SPÄTER