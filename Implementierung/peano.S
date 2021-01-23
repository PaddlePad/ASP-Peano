.intel_syntax noprefix
.global peano

.text
//  grad = rdi
//  x*   = rsi
//  y*   = rdx

// Directions: 0 = up , 1 = right , 2 = down , 3 = left

//peano:

//--------------HILFSFUNKTIONEN--------------

reverse: //  in = rbx, out = rbp, size = rcx (in bleiben gleich)
//für schnellere Vergleiche evtl 0 in al oder so
    cmp rcx, 0          //ende der Schleife
    jle .LEndReverse

    //Ziel: out[i] = (in[i] + 2) % 4;

    mov rax, qword ptr[rbx + rcx * 8]
    mov qword ptr[rbp + rcx * 8], rax  //wert von in in out schreiben
    add qword ptr[rbp + rcx * 8], 2          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp qword ptr[rbp + rcx * 8], 3
    jle .LoopReverse
    sub qword ptr[rbp + rcx * 8], 4         //(out[i] + 2) % 4
.LoopReverse:    
    dec rcx
    jmp reverse

.LEndReverse:
    //SPÄTER
    

mirror: //  in = rbx, out = rbp, size = rsp
//für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rcx, 0          //ende der Schleife
    jle .LEndMirror

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov rax, qword ptr[rbx + rcx * 8]
    mov qword ptr[rbp + rcx * 8], rax  //wert von in in out schreiben
    test qword ptr[rbp + rcx * 8], 0x0000_0000_0000_0001    //test last bit (mod 2), check wegen Leerzeichen
    jnz .LoopMirror 
    add qword ptr[rbp + rcx * 8], 2     //wie oben, (in[i]+2) % 4        
    cmp qword ptr[rbp + rcx * 8], 3
    jle .LoopMirror 
    sub qword ptr[rbp + rcx * 8], 4
           
.LoopMirror:    
    dec rcx
    jmp mirror

.LEndMirror:
    //SPÄTER


ret
