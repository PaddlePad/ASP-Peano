.intel_syntax noprefix
.global mirror

.text
mirror: //  in = rbx, out = rbp, size = rsp
//für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rcx, 0          //ende der Schleife
    jle .LEndMirror

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov rdi, qword ptr[rbx + rcx * 8]
    mov qword ptr[rbp + rcx * 8], rdi  //wert von in in out schreiben
    test qword ptr[rbp + rcx * 8], 0x0000000000000001    //test last bit (mod 2), check wegen Leerzeichen
    jnz .LoopMirror 
    add qword ptr[rbp + rcx * 8], 2     //wie oben, (in[i]+2) % 4        
    cmp qword ptr[rbp + rcx * 8], 3
    jle .LoopMirror 
    sub qword ptr[rbp + rcx * 8], 4
           
.LoopMirror:    
    dec rcx
    jmp reverse

.LEndMirror:
ret
    //SPÄTER