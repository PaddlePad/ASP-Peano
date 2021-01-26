.intel_syntax noprefix
.global peano

.text
//  grad = rdi
//  x*   = rsi
//  y*   = rdx

//BENUTZTE REGISTER:
//Parameter:    rdi, rsi, rdx
//Callersaved:  rcx, rax, 
//Calleesaved:  rbp, rsp, r12, r13  //Pat: These

// Directions: 0 = up , 1 = right , 2 = down , 3 = left

peano: // rdi = grad, rsi x*, rdx* y
    mov al, 1                       //AL == 1
    mov rbp, al                  //Für calcNext auf 1 setzen

    //Hardcode default Kurve in rsi //immediate oder register?
    mov qword ptr[rsi], 0 
    mov qword ptr[rsi + 8], 0 
    mov qword ptr[rsi + 16], al
    mov qword ptr[rsi + 24], 2
    mov qword ptr[rsi + 32], 2 
    mov qword ptr[rsi + 40], al
    mov qword ptr[rsi + 48], 0 
    mov qword ptr[rsi + 56], 0

    cmp rdi, al                  //wenn grad = 1, koord berechnen und fertig
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
    xor r8, r8      //r8 = 0
    //anzahl Felder berechnen für abbruch
    mul rbp, 9

.LKoordinatenIteration:
    //Abbruchbedingung
    cmp r8, rbp
    jg .LEnd

    //letzen 2 bit entscheidend
    test qword ptr[rsi + r8 * 8], 0x0000_0000_0000_0001 //ob zahl gerade
    jnz .LDirUngerade                               //wenn %2 != 0, dir = 1 oder 3
    test ptr[rsi + r8 * 8], 0x0000_0000_0000_0002   //test 2. bit -> dir 0 oder 2
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
    test ptr[rsi + r8 * 8], 0x0000_0000_0000_0002   //test 2. bit 
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

    //Verbindungsschritt nach Rechts
    mov qword ptr[rsi + rsp * 8], 1

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

    //Verbindungsschritt nach rechts
    mov qword ptr[rsi + rsp * 8], 1

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
    
ret

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