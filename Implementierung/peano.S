.intel_syntax noprefix
.global peano

.text
//  grad = rdi
//  x*   = rsi
//  y*   = rdx

// Directions: 0 = up , 1 = right , 2 = down , 3 = left

peano: // rdi = grad, rsi x*, rcx* y
mov rbp, 1


//--------------HILFSFUNKTIONEN--------------

calcNext: // rdi = Grad, rsi curr array pointer
    // wir versuchen die arrays jedes mal zu erstellen
    // wenn wir sie brauchen

    // momentane Länge -> rbp
    mul rbp, 9
    // offset -> rsp
    mov rsp, rbp
    mov r12, rsp

    // array -> rsi
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + rsp * 8], 0

    // Zweiter step reverse mirror (evtl nur mirror?)
    mov rcx, rbp                
    dec rcx                     // counter neu setzen  
    call reverse                
    mov r12, rsp                //reverse mirror offset setzen
    call mirror                 //TESTEN
    add rsp, rbp                // i = i + length;
    xor r12, r12                // reverse-mirror offset nullen
    
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + rsp * 8], 0

    // Dritter step
    mov rcx, rbp                //counter setzen
    dec rcx 
    call normalInsert
    add rsp, rbp                // i = i + length;

    //Verbindungsschritt nach Links
    mov qword ptr[rsi + rsp * 8], 3

    //Vierter Step
    mov rcx, rbp                
    dec rcx   
    call mirror
    add rsp, rbp                // i = i + length;

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + rsp * 8], 2

    //Fünfter Step
    mov rcx, rbp                
    dec rcx 
    call reverse
    add rsp, rbp

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + rsp * 8], 2

    //Sechster Step
    mov rcx, rbp                
    dec rcx 
    call mirror
    add rsp, rbp

    //Verbindungsschritt nach Links
    mov qword ptr[rsi + rsp * 8], 3

    //Siebter Schritt
    mov rcx, rbp                
    dec rcx 
    call normalInsert
    add rsp, rbp

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + rsp * 8], 0

    //Achter Step               //wie oben
    mov rcx, rbp                
    dec rcx                     // counter neu setzen  
    call reverse                
    mov r12, rsp                //reverse mirror offset setzen
    call mirror                 
    add rsp, rbp                // i = i + length;
    xor r12, r12                // reverse-mirror offset nullen

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + rsp * 8], 0

    //Letzter Step
    mov rcx, rbp                
    dec rcx 
    call normalInsert

    //length = ((int)pow(9, currGrad) - 1);


normalInsert: // rsi = array, size = rcx, offset = rsp        //Normales einfügen der Schleife
    cmp rcx, 0          //ende der Schleife
    jle .LEndInsert

    mov rax, qword ptr[rsi + rcx * 8]
    mov qword ptr[rsi + rcx * 8 + rsp], rax  //wert von array in anderen Bereich schreiben
    
    dec rcx
    jmp normalInsert

.LEndInsert: ret

reverse: // rsi = array, size = rcx, offset = rsp
//für schnellere Vergleiche evtl 0 in al oder so
    cmp rcx, 0          //ende der Schleife
    jle .LEndReverse

    //Ziel: out[i] = (in[i] + 2) % 4;
    mov rax, qword ptr[rsi + rcx * 8]
    mov qword ptr[rsi + rcx * 8 + rsp], rax  //wert von in in out schreiben
    add qword ptr[rsi + rcx * 8 + rsp], 2          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp qword ptr[rsi + rcx * 8 + rsp], 3
    jle .LoopReverse
    sub qword ptr[rsi + rcx * 8 + rsp], 4         //(out[i] + 2) % 4
.LoopReverse:    
    dec rcx
    jmp reverse

.LEndReverse:
ret   

mirror: //  rsi = array, size = rcx, offset = rsp, reverse-mirror offset = r12 
//für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rcx, 0          //ende der Schleife
    jle .LEndMirror

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov rax, qword ptr[rsi + rcx * 8 + r12]
    mov qword ptr[rsi + rcx * 8 + rsp], rax  //wert von in in out schreiben
    test qword ptr[rsi + rcx * 8 + rsp], 0x0000_0000_0000_0001    //test last bit (mod 2), check wegen Leerzeichen
    jnz .LoopMirror 
    add qword ptr[rsi + rcx * 8 + rsp], 2     //wie oben, (in[i]+2) % 4        
    cmp qword ptr[rsi + rcx * 8 + rsp], 3
    jle .LoopMirror 
    sub qword ptr[rsi + rcx * 8 + rsp], 4
           
.LoopMirror:    
    dec rcx
    jmp mirror

.LEndMirror:
ret