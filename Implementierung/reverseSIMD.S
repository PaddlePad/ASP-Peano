
.intel_syntax noprefix
.global SIMDtest

.text

normalInsertSIMD: // rsi = array, size = rcx, offset = r12  Normales einfügen der Schleife
    cmp rcx, 0                              //ende der Schleife
    jle .LEndInsertSIMD                         // eventuell durch je austauschen

    mov r8, rcx                             //r8 = rcx
    dec r8
    shl r8, 3                               //mul rcx, 8

    movupd xmm1, [rsi + r8]            //qword ptr[rsi + rcx * 8]
    add r8, r12                             //dann: rcx * 8 + r12
    movupd [rsi + r8], xmm1            //wert von array in anderen Bereich schreiben, ptr[rsi + rcx * 8 + r12]
    
    dec rcx
    dec rcx
    jmp normalInsertSIMD

.LEndInsertSIMD: ret

reverseSIMD: // rsi = array, size = rcx, offset = r12, instructions from SSE 2.x
    xor rax, rax
    mov rax, 2
    movq xmm0, rax
    pshufd xmm0, xmm0, 01_00_01_00  //2 in upper half and lower half of xmm0, also 000000000000010 000000000000010
    inc rax
    movq xmm2, rax
    pshufd xmm2, xmm2, 01_00_01_00  //für später in xmm2: 000000000000011 000000000000011
    //einmal vor programmstart, da sonst xmm0/2 nicht genutzt

.LoopSIMD:
    cmp rcx, 0          //ende der Schleife
    jle .LEndReverse    // optimierung auf equal check

    //Ziel: out[i] = (in[i] + 2) % 4;
    mov r8, rcx
    shl r8, 3                           //rcx*8
    //aligned?
    movupd xmm1, qword ptr[rsi + r8]    //move 2x64Bit into xmm1
    paddq xmm1, xmm0                    //out[i] + 2
    pand xmm1, xmm2                     //(out[i] + 2) % 4, da die oberen bits nicht gebraucht werden

.LoopReverseSIMD:    
    add r8, r12                         //rcx * 8 + r12
    movupd qword ptr[rsi + r8], xmm1    //wert von in in out schreiben
    dec rcx
    dec rcx                             //weil ja 2 auf einmal
    jmp reverse

.LEndReverseSIMD: ret   