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

    // Calleesaved Registerwerte rdx abspeichern und danach wiederherstellen
    push rbp
    push r12
    push r13
    push r14

    mov rax, 1                    //AL == 1
    mov rbp, rax                  //Für calcNext auf 1 setzen

    //Hardcode default Kurve in rsi //immediate oder register?
    mov qword ptr[rsi + 8], 0
    mov qword ptr[rsi + 16], 0
    mov qword ptr[rsi + 24], rax
    mov qword ptr[rsi + 32], 2
    mov qword ptr[rsi + 40], 2
    mov qword ptr[rsi + 48], rax
    mov qword ptr[rsi + 56], 0
    mov qword ptr[rsi + 64], 0

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
    mov r8, rax      //r8 = 1
    //anzahl Felder berechnen für Abbruch 
    mov r9, rbp
    shl r9, 3
    add rbp, r9

    // Calleesave Registerwerte wiederherstellen
    mov rdi, rbp
    pop r14
    pop r13
    pop r12
    pop rbp

    //ret

    // Erster Punkt hardcodiert
    mov qword ptr[rsi], 1     //x
    mov qword ptr[rdx], 1     //y

.LKoordinatenIteration:
    //Abbruchbedingung
    cmp r8, rdi
    jge .LEnd

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
    mov r11, rbp        // i = i + length
    shl r11, 3
    mov r10, rbp        // counter festlegen für später (Effizienz)
    dec r10
    // offset -> r14, reversed mirror offset -> r12 // evtl mit bool austauschen falls cmp effizient
    mov r14, rbp
    shl r14, 3          // offset vor der schleife bitshiften, ansonsten zu großer wert
    mov r12, r14

    // array -> rsi
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + r14], 0

    // Zweiter step reverse mirror (evtl nur mirror?)
    mov rcx, r10       // counter neu setzen, start bei 136 rückwerts                       
    call reverse                
    mov rcx, r10
    mov r12, r14                //reverse mirror offset setzen, vllt sinnlos = löschen
    call mirror                 
    add r14, r11                 // i = i + length * 8;
    xor r12, r12                // reverse-mirror offset nullen
    
    // Verbindungsschritt nach oben
    mov qword ptr[rsi + r14], 0

    // Dritter step
    mov rcx, r10                //counter setzen
    call normalInsert
    add r14, r11                // i = i + length * 8;

    //Verbindungsschritt nach Rechts
    mov qword ptr[rsi + r14], 1

    //Vierter Step
    mov rcx, r10  
    call mirror
    add r14, r11                // i = i + length;

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + r14], 2

    //Fünfter Step
    mov rcx, r10 
    call reverse
    add r14, r11

    //Verbindungsschritt nach Unten
    mov qword ptr[rsi + r14], 2

    //Sechster Step
    mov rcx, r10
    call mirror
    add r14, r11

    //Verbindungsschritt nach rechts
    mov qword ptr[rsi + r14], 1

    //Siebter Schritt
    mov rcx, r10
    call normalInsert
    add r14, r11

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + r14], 0

    //Achter Step               //wie oben
    mov rcx, r10                // counter neu setzen  
    call reverse 
    mov rcx, r10               
    mov r12, r14                //reverse mirror offset setzen
    call mirror                 
    add r14, r11                // i = i + length;
    xor r12, r12                // reverse-mirror offset nullen

    //Verbindungsschritt nach oben
    mov qword ptr[rsi + r14], 0

    //Letzter Step
    mov rcx, r10
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
    jle .LEndReverse    // optimierung auf equal check

    //Ziel: out[i] = (in[i] + 2) % 4;
    mov r8, rcx
    shl r8, 3                           //rcx*8
    mov rax, qword ptr[rsi + r8]
    add r8, r14                         //rcx * 8 + r14
    add rax, 2          //out[i] + 2    für schnellere Vergleiche evtl 2 in al oder so, Videos anschauen zu ptr
    cmp rax, 3

    jle .LoopReverse
    sub rax, 4         //(out[i] + 2) % 4
.LoopReverse:    
    mov qword ptr[rsi + r8], rax        //wert von in in out schreiben
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
    add r8, r14                             // normaler offset: rcx * 8 + r14
    add r9, r12                             // reverse mirror offset: rcx * 8 + r12

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