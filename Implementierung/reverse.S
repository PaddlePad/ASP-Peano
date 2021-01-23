.intel_syntax noprefix
.global reverse

.text

reverse: //  in = rbx, out = rbp, size = rcx (in bleiben gleich)
//für schnellere Vergleiche evtl 0 in al oder so
    cmp rcx, 0          //ende der Schleife
    jle .LEndReverse

    //Ziel: out[i] = (in[i] + 2) % 4;

    mov rdi, qword ptr[rbx + rcx * 4]
    mov qword ptr[rbp + rcx * 8], rdi   //wert von in in out schreiben
    add qword ptr[rbp + rcx * 8], 2          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp qword ptr[rbp + rcx * 8], 3
    jle .LoopReverse
    sub qword ptr[rbp + rcx * 8], 4         //(out[i] + 2) % 4
.LoopReverse:    
    dec rcx
    jmp reverse

.LEndReverse:
ret
    //SPÄTER