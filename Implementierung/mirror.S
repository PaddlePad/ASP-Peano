.intel_syntax noprefix
.global mirror

.text
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
ret
    //SPÄTER