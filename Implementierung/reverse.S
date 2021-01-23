.intel_syntax noprefix
.global reverse

.text

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
ret
    //SPÄTER