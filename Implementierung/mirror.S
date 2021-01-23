.intel_syntax noprefix
.global mirror

.text
mirror: //  in = rdi, out = rsi, size = rdx (in bleiben gleich)
//für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rdx, 0          //ende der Schleife
    jl .LEndMirror

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov rax, qword ptr[rdi + rdx * 8]
    mov qword ptr[rsi + rdx * 8], rax  //wert von in in out schreiben
    test qword ptr[rsi + rdx * 8], 0x0000_0000_0000_0001    //test last bit (mod 2)
    jz .LoopMirror 
    add qword ptr[rsi + rdx * 8], 2     //wie oben, (in[i]+2) % 4        
    cmp qword ptr[rsi + rdx * 8], 3
    jle .LoopMirror 
    sub qword ptr[rsi + rdx * 8], 4
           
.LoopMirror:    
    dec rdx
    jmp mirror

.LEndMirror:
ret
    //SPÄTER

// output -> 0, 0, 3, 2, 2, 3, 0, 0