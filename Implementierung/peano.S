.intel_syntax noprefix
.global peano

.text
//  grad = rdi
//  x*   = rsi
//  y*   = rdx

//BENUTZTE REGISTER:
//Parameter:    rdi, rsi, rdx
//Callersaved:  rcx, rax, 
//Calleesaved:  rbp, r12, r13, r14

// Directions: 0 = up , 1 = right , 2 = down , 3 = left

peano: // rdi = grad, rsi x*, rdx* y
    mov rax, 1                    //AL == 1
    mov rbp, rax                  //Für calcNext auf 1 setzen

    //Hardcode default Kurve in rsi //immediate oder register?
    mov qword ptr[rsi], 0
    mov qword ptr[rsi + 8], 0
    mov qword ptr[rsi + 16], rax
    mov qword ptr[rsi + 24], 2
    mov qword ptr[rsi + 32], 2
    mov qword ptr[rsi + 40], rax
    mov qword ptr[rsi + 48], 0
    mov qword ptr[rsi + 56], 0

    cmp rdi, rax                  //wenn grad = 1, koord berechnen und fertig
    jle .LKoordinatenCalc
    mov r13, rbp                    //imm 1 oder rbp?
    mov qword ptr[rdx], 8           //8 speichern, wird so oft verwendet

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
    xor r8, r8      //r8 = 0
    //anzahl Felder berechnen für Abbruch 
    mov r9, rbp
    shl r9, 3
    add rbp, r9
    //imul rbp, 9

.LKoordinatenIteration:
    //Abbruchbedingung
    cmp r8, rbp
    jg .LEnd

    //letzen 2 bit entscheidend
    test qword ptr[rsi + r8 * 8], 0x0000_0000_0000_0001 //ob zahl gerade
    jnz .LDirUngerade                               //wenn %2 != 0, dir = 1 oder 3
    test qword ptr[rsi + r8 * 8], 0x0000_0000_0000_0002   //test 2. bit -> dir 0 oder 2
    jnz .LDir2

    //Dir0  //oben
    inc rcx
    mov qword ptr[rdx + r8 * 8], rcx    //y-koord in y* //change steht oben
    mov qword ptr[rsi + r8 * 8], rax    //x-koord in x*
    jmp .LKoordinatenIterationEnd

    .LDir2: //unten
    dec rcx
    mov qword ptr[rdx + r8 * 8], rcx    //y-koord in y*
    mov qword ptr[rsi + r8 * 8], rax    //x-koord in x*
    jmp .LKoordinatenIterationEnd

.LDirUngerade:
    test qword ptr[rsi + r8 * 8], 0x0000_0000_0000_0002   //test 2. bit 
    jnz .LDir3

    //Dir1  //rechts
    inc rax
    mov qword ptr[rsi + r8 * 8], rax    //x-koord in x*
    mov qword ptr[rdx + r8 * 8], rcx    //y-koord in y*
    jmp .LKoordinatenIterationEnd

    .LDir3: //links
    dec rax
    mov qword ptr[rsi + r8 * 8], rax    //x-koord in x*
    mov qword ptr[rdx + r8 * 8], rcx    //y-koord in y*
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
    //imul rbp, 9
    // offset -> r14
    mov r14, rbp
    mov r12, r14

    // array -> rsi
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + r14 * 8], 0

    // Zweiter step reverse mirror (evtl nur mirror?)
    mov rcx, rbp                
    dec rcx                     // counter neu setzen  
    call reverse                
    mov r12, r14                //reverse mirror offset setzen
    call mirror                 //TESTEN
    add r14, rbp                // i = i + length;
    xor r12, r12                // reverse-mirror offset nullen
    
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + r14 * 8], 0

    // Dritter step
    mov rcx, rbp                //counter setzen
    dec rcx 
    call normalInsert
    add r14, rbp                // i = i + length;

    //Verbindungsschritt nach Rechts
    mov qword ptr[rsi + r14 * 8], 1

    //Vierter Step
    mov rcx, rbp                
    dec rcx   
    call mirror
    add r14, rbp                // i = i + length;

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + r14 * 8], 2

    //Fünfter Step
    mov rcx, rbp                
    dec rcx 
    call reverse
    add r14, rbp

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + r14 * 8], 2

    //Sechster Step
    mov rcx, rbp                
    dec rcx 
    call mirror
    add r14, rbp

    //Verbindungsschritt nach rechts
    mov qword ptr[rsi + r14 * 8], 1

    //Siebter Schritt
    mov rcx, rbp                
    dec rcx 
    call normalInsert
    add r14, rbp

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + r14 * 8], 0

    //Achter Step               //wie oben
    mov rcx, rbp                
    dec rcx                     // counter neu setzen  
    call reverse                
    mov r12, r14                //reverse mirror offset setzen
    call mirror                 
    add r14, rbp                // i = i + length;
    xor r12, r12                // reverse-mirror offset nullen

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + r14 * 8], 0

    //Letzter Step
    mov rcx, rbp                
    dec rcx 
    call normalInsert

    //length = ((int)pow(9, currGrad) - 1);
    
ret

normalInsert: // rsi = array, size = rcx, offset = r14        //Normales einfügen der Schleife
    cmp rcx, 0          //ende der Schleife
    jle .LEndInsert

    mov r8, rcx                             //r8 = rcx
    shl r8, 3                               //mul rcx, 8
    mov rax, qword ptr[rsi + r8]            //qword ptr[rsi + rcx * 8]
    add r8, r14                             //dann: rcx * 8 + r14
    mov qword ptr[rsi + r8], rax            //wert von array in anderen Bereich schreiben, ptr[rsi + rcx * 8 + r14]
    
    dec rcx
    jmp normalInsert

.LEndInsert: ret

reverse: // rsi = array, size = rcx, offset = r14
//für schnellere Vergleiche evtl 0 in al oder so
    cmp rcx, 0          //ende der Schleife
    jle .LEndReverse

    //Ziel: out[i] = (in[i] + 2) % 4;
    mov r8, rcx
    shl r8, 3                           //rcx*8
    mov rax, qword ptr[rsi + r8]
    add r8, r14                         //rcx * 8 + r14
    mov qword ptr[rsi + r8], rax        //wert von in in out schreiben
    add qword ptr[rsi + r8], 2          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp qword ptr[rsi + r8], 3

    jle .LoopReverse
    sub qword ptr[rsi + r8], 4         //(out[i] + 2) % 4
.LoopReverse:    
    dec rcx
    jmp reverse

.LEndReverse:
ret   

mirror: //  rsi = array, size = rcx, offset = r14, reverse-mirror offset = r12 
//für schnellere Vergleiche evtl 0 in al oder so, wie oben
    cmp rcx, 0          //ende der Schleife
    jle .LEndMirror

    //Ziel: if ((in[i] % 2) == 0), do {out[i] = (in[i]+2) % 4;}
    mov r8, rcx
    shl r8, 3
    mov r9, r8
    add r8, r14                             //rcx * 8 + r14
    add r9, r12                             //rcx * 8 + r12

    mov rax, qword ptr[rsi + r9]
    mov qword ptr[rsi + r8], rax  //wert von in in out schreiben
    test qword ptr[rsi + r8], 0x0000_0000_0000_0001    //test last bit (mod 2), check wegen Leerzeichen
    jnz .LoopMirror 
    add qword ptr[rsi + r8], 2     //wie oben, (in[i]+2) % 4        
    cmp qword ptr[rsi + r8], 3
    jle .LoopMirror 
    sub qword ptr[rsi + r8], 4
           
.LoopMirror:    
    dec rcx
    jmp mirror

.LEndMirror:
ret