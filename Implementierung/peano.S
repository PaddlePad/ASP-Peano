.intel_syntax noprefix
.global peano

.text
//  grad = rdi
//  x*   = rsi
//  y*   = rdx

//BENUTZTE REGISTER:
//Parameter:    rdi, rsi, rdx
//Callersaved:  rcx, rax, 
//Calleesaved:  rbp, r13, r12

// Directions: 0 = up , 1 = right , 2 = down , 3 = left

peano: // rdi = grad, rsi x*, rdx* y

    // Calleesaved Registerwerte rdx abspeichern und danach wiederherstellen
    push rbp
    push r13
    push r12

    mov rax, 1                    //AL == 1
    xor rbp, rbp

    //Hardcode default Kurve in rsi //immediate oder register?
    mov qword ptr[rsi + 8], rbp
    mov qword ptr[rsi + 16], rbp
    mov qword ptr[rsi + 24], rax
    mov qword ptr[rsi + 32], 2
    mov qword ptr[rsi + 40], 2
    mov qword ptr[rsi + 48], rax
    mov qword ptr[rsi + 56], rbp
    mov qword ptr[rsi + 64], rbp

    mov rbp, rax                  //Für calcNext auf 1 setzen
    cmp rdi, rax                  //wenn grad = 1, koord berechnen und fertig
    jle .LKoordinatenCalc
    mov r13, rbp                    //imm 1 oder rbp?

.LoopCalcNext:  //currGrad = r13, Zielgrad
    cmp r13, rdi
    jge .LKoordinatenCalc       //ausprobieren ob jg oder jge
    call calcNext
    inc r13                     //currGrad++
    jmp .LoopCalcNext


.LKoordinatenCalc:   // rsi x*, rdx* y
    //index x = rax, index y = rcx, laufindex = r8
    mov rax, 1
    mov rcx, rax    //alles auf 1 setzen
    mov r8, rax      //r8 = 1
    //anzahl Felder berechnen für Abbruch 
    mov r9, rbp
    shl r9, 3
    add rbp, r9

    // Calleesave Registerwerte wiederherstellen
    mov rdi, rbp
    pop r12
    pop r13
    pop rbp

    // Erster Punkt hardcodiert
    mov qword ptr[rsi], 1     //x
    mov qword ptr[rdx], 1     //y

.LKoordinatenIteration:
    //Abbruchbedingung
    cmp r8, rdi
    jge .LEnd

    lea r9, [rsi + r8 * 8]
    lea r10, [rdx + r8 * 8]

    //letzen 2 bit entscheidend
    test qword ptr[r9], 0x0000_0000_0000_0001 //ob zahl gerade
    jnz .LDirUngerade                               //wenn %2 != 0, dir = 1 oder 3
    test qword ptr[r9], 0x0000_0000_0000_0002   //test 2. bit -> dir 0 oder 2
    jnz .LDir2

    //Dir0  //oben
    inc rcx
    mov qword ptr[r10], rcx    //y-koord in y* //change steht oben
    mov qword ptr[r9], rax    //x-koord in x*
    jmp .LKoordinatenIterationEnd

    .LDir2: //unten
    dec rcx
    mov qword ptr[r10], rcx    //y-koord in y*
    mov qword ptr[r9], rax    //x-koord in x*
    jmp .LKoordinatenIterationEnd

.LDirUngerade:
    test qword ptr[r9], 0x0000_0000_0000_0002   //test 2. bit 
    jnz .LDir3

    //Dir1  //rechts
    inc rax
    mov qword ptr[r9], rax    //x-koord in x*
    mov qword ptr[r10], rcx    //y-koord in y*
    jmp .LKoordinatenIterationEnd

    .LDir3: //links
    dec rax
    mov qword ptr[r9], rax    //x-koord in x*
    mov qword ptr[r10], rcx    //y-koord in y*
    jmp .LKoordinatenIterationEnd

.LKoordinatenIterationEnd:
    inc r8
    jmp .LKoordinatenIteration

.LEnd:
ret

//--------------HILFSFUNKTIONEN--------------

calcNext: //rsi curr array pointer, currGrad = r8
    // wir versuchen die arrays jedes mal zu erstellen
    // wenn wir sie brauchen

    // momentane Länge -> rbp
    mov r9, rbp
    shl r9, 3
    add rbp, r9

    mov r11, rbp        // i = i + length
    shl r11, 3
    mov r12, r11        // offset vor der schleife bitshiften, ansonsten zu großer wert
    
    mov r10, rbp        // counter festlegen für später (Effizienz)
    dec r10
  
    // array -> rsi
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + r12], 0

    // Zweiter step reverse mirror (evtl nur mirror?)
    mov rcx, r10       // counter neu setzen, start bei 136 rückwerts                       
    call reverseMirror                               
    add r12, r11                 // i = i + length * 8;
    
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + r12], 0

    // Dritter step
    mov rcx, r10                //counter setzen
    call normalInsert
    add r12, r11                // i = i + length * 8;

    //Verbindungsschritt nach Rechts
    mov qword ptr[rsi + r12], 1

    //Vierter Step
    mov rcx, r10  
    call mirror
    add r12, r11                // i = i + length;

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + r12], 2

    //Fünfter Step
    mov rcx, r10 
    call reverse
    add r12, r11

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + r12], 2

    //Sechster Step
    mov rcx, r10
    call mirror
    add r12, r11

    //Verbindungsschritt nach rechts
    mov qword ptr[rsi + r12], 1

    //Siebter Schritt
    mov rcx, r10
    call normalInsert
    add r12, r11

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + r12], 0

    //Achter Step               //wie oben
    mov rcx, r10                // counter neu setzen  
    call reverseMirror              
    add r12, r11                // i = i + length;

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + r12], 0

    //Letzter Step
    mov rcx, r10
    call normalInsert

    //length = ((int)pow(9, currGrad) - 1);
    
ret

normalInsert: // rsi = array, size = rcx, offset = r12  Normales einfügen der Schleife
    cmp rcx, 0                              //ende der Schleife
    jle .LEndInsert                         // eventuell durch je austauschen

    mov r8, rcx                             //r8 = rcx
    shl r8, 3                               //mul rcx, 8
    mov rax, qword ptr[rsi + r8]            //qword ptr[rsi + rcx * 8]
    add r8, r12                             //dann: rcx * 8 + r12
    mov qword ptr[rsi + r8], rax            //wert von array in anderen Bereich schreiben, ptr[rsi + rcx * 8 + r12]
    
    dec rcx
    jmp normalInsert

.LEndInsert: ret

reverse: // rsi = array, size = rcx, offset = r12 für schnellere Vergleiche evtl 0 in al oder so
    cmp rcx, 0          //ende der Schleife
    jle .LEndReverse    // optimierung auf equal check

    //Ziel: out[i] = (in[i] + 2) % 4;
    mov r8, rcx
    shl r8, 3                           //rcx*8
    
    mov rax, qword ptr[rsi + r8]
    add rax, 2                          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp rax, 3
    jle .LoopReverse
    sub rax, 4                          //(out[i] + 2) % 4

.LoopReverse:    
    add r8, r12                         //rcx * 8 + r12
    mov qword ptr[rsi + r8], rax        //wert von in in out schreiben
    dec rcx
    jmp reverse

.LEndReverse:
ret   

mirror: //  rsi = array, size = rcx, offset = r12 //für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rcx, 0          //ende der Schleife
    jle .LEndMirror

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov r8, rcx
    shl r8, 3
    mov r9, r8
    add r8, r12                             // normaler offset: rcx * 8 + r12

    mov rax, qword ptr[rsi + r9]
    test rax, 0x0000_0000_0000_0001    //test last bit (mod 2), check wegen Leerzeichen
    jnz .LoopMirror 
    add rax, 2     //wie oben, (in[i]+2) % 4        
    cmp rax, 3
    jle .LoopMirror 
    sub rax, 4
           
.LoopMirror:    
    mov qword ptr[rsi + r8], rax  //wert von in in out schreiben
    dec rcx
    jmp mirror

.LEndMirror:
ret

reverseMirror: //  rsi = array, size = rcx, offset = r12//für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rcx, 0          //ende der Schleife
    jle .LEndReverseMirror

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov r8, rcx
    shl r8, 3
    mov r9, r8
    add r8, r12                             // normaler offset: rcx * 8 + r12

    mov rax, qword ptr[rsi + r9]
    test rax, 0x0000_0000_0000_0001    //test last bit (mod 2), check wegen Leerzeichen
    jz .LoopReverseMirror 
    add rax, 2     //wie oben, (in[i]+2) % 4        
    cmp rax, 3
    jle .LoopReverseMirror 
    sub rax, 4
           
.LoopReverseMirror:    
    mov qword ptr[rsi + r8], rax  //wert von in in out schreiben
    dec rcx
    jmp reverseMirror

.LEndReverseMirror:
ret