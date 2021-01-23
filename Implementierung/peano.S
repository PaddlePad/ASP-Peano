.intel_syntax noprefix
.global peano

.text
//  grad = rdi
//  x*   = rsi
//  y*   = rdx

// Directions: 0 = up , 1 = right , 2 = down , 3 = left

peano:

//--------------HILFSFUNKTIONEN--------------

reverse: //  in = rbx, out = rbp, size = rcx (in bleiben gleich)
//für schnellere Vergleiche evtl 0 in al oder so
    cmp rcx, 0          //ende der Schleife
    jle L.EndReverse

    //Ziel: out[i] = (in[i] + 2) % 4;

    mov dword ptr[rbp + rcx * 8], dword ptr[rbx + rcx * 8]  //wert von in in out schreiben
    add dword ptr[rbp + rcx * 8], 2          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp dword ptr[rbp + rcx * 8], 3
    jle .LoopReverse
    sub dword ptr[rbp + rcx * 8], 4         //(out[i] + 2) % 4
.LoopReverse:    
    dec rcx
    jmp reverse

.LEndReverse:
    //SPÄTER
    

mirror: //  in = rbx, out = rbp, size = rsp
//für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rcx, 0          //ende der Schleife
    jle L.EndReverse

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov dword ptr[rbp + rcx * 8], dword ptr[rbx + rcx * 8]  //wert von in in out schreiben
    test dword ptr[rbp + rcx * 8], 0x0000 0000 0000 0001    //test last bit (mod 2), check wegen Leerzeichen
    jnz .LoopMirror 
    add dword ptr[rbp + rcx * 8], 2     //wie oben, (in[i]+2) % 4        
    cmp dword ptr[rbp + rcx * 8], 3
    jle .LoopMirror 
    sub dword ptr[rbp + rcx * 8], 4
           
.LoopMirror:    
    dec rcx
    jmp reverse

.LEndMirror:
    //SPÄTER


ret
