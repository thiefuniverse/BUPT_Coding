;;; a program for student score management system
data segment
    jumpTable dw caseInfo,caseSort,caseAverage,caseStages,caseQuit,caseError
    displayinfo db 0dh,0ah,'welcome!',0Dh,0ah,'you can press ',0Dh,0ah,'0 for add student infomation,',0Dh,0ah,'1 for sort display,',0Dh,0ah,'2 for print average score,',0Dh,0ah,'3 for get all stages numbers of people,',0Dh,0ah,'4 for quit',0dh,0ah,'$'
    error db 'error! wrong command!','$' 
    nameInfo db 0Dh,0ah,'please input student name (end with ;) :',0Dh,0ah,'$'
    classInfo db 0Dh,0ah,'please input student class (0-19)(end with ;)',0Dh,0ah,'$'
    stIDInfo db 0Dh,0ah,'please input student id(end with ;)',0Dh,0ah,'$'
    scoreInfo db 0Dh,0ah,'please input student score(end with ;)',0Dh,0ah,'$'
    addItInfo db 0dh,0ah,'add a student successfully',0dh,0ah,'$'   
    bye db 0dh,0ah,'bye!',0dh,0ah,'$'
    scorep50 db 0dh,0ah,'numbers below 60 is:','$'
    scorep60 db 0dh,0ah,'numbers from 60 to 69 is:','$'
    scorep70 db 0dh,0ah,'numbers from 70 to 79 is:','$'
    scorep80 db 0dh,0ah,'numbers from 80 to 89 is:','$'
    scorep90 db 0dh,0ah,'numbers from 90 to 99 is:','$'
    nextLine db 0dh,0ah,'$'
     

    pName db 'Name:','$'
    pClass db 'Class:','$'
    pID db 'ID: ','$'
    pScore db 'Score:','$'
    
    sortInfo db 0dh,0ah,'sort by scores',0dh,0ah,'$'
    averInter dw 0
    average dw 0
    averDec db 0
    averCf dw 0 
    averInfo db 0dh,0ah,'average score is ','$' 
    
    printNum dw 1
    ;;;0 for add student infomation
    ;;;1 for sort display
    ;;;2 for print average score
    ;;;3 for get all stages numbers of people
    ;;;4 for quit
    ;;;5 for default 
    studentName db 100(21 dup(''))
    studentClass db 100(3 dup(''))
    studentID db 100(11 dup(''))
    studentScore db 100(5 dup('')) 
    studentSort dw 100(dup(0))
    countName dw 0
    countClass dw 0
    countID dw 0
    countScore dw 0
    countAll dw 0
    score50 dw 0
    score60 dw 0
    score70 dw 0
    score80 dw 0
    score90 dw 0
data ends

stack segment
    dw   128  dup(0)
stack ends

code segment
start:
    assume cs:code,ds:data
; set segment registers:
    mov ax, data
    mov ds, ax
    mov es, ax 
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;for main 
still:
    call getCommand
    mov bx,0
    mov bl,al
    cmp bx,5
    jbe caseJudge
    mov bx,5
caseJudge:
    shl bx,1
    jmp jumpTable[bx]
    
caseInfo:
    call addInfo 
    jmp still
caseSort:
    call sortBy
    jmp still
caseAverage:
    call getAverage
    jmp still
caseStages:
    call getStage
    jmp still
caseQuit:
     lea dx,bye
     mov ah, 9
     int 21h
     jmp caseEnd
caseError:
     lea dx,error
     mov ah, 9
     int 21h
     jmp still    
    
caseEnd:    
    ;;;program ends
    mov ax, 4c00h
    int 21h    
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;for function
    ;;; add student info to db
    addInfo proc near
        push dx
        push bx
        push cx
        
        inc [countAll]
        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        lea bx,studentName
        add bx,[countName]   ;;;counter is db init position   
        
        lea dx,nameInfo      ;;print info
        mov ah, 9
        int 21h
     input1:
        mov ah, 1
        int 21h
        cmp al,';'
        je inputClass
        mov [bx],al
        inc bx
        jmp input1
     ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;   
     inputClass:
        inc bx
        mov [bx],'$'
        add [countName],21
        
        lea bx,studentClass
        add bx,[countClass]   ;;;counter is db init position     
        
        lea dx,ClassInfo     ;;print info
        mov ah, 9
        int 21h
     input2:
        mov ah, 1
        int 21h
        cmp al,';'
        je inputID
        mov [bx],al
        inc bx
        jmp input2  
     ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
     inputID:
        inc bx
        mov [bx],'$'
        add [countClass],3 
        lea bx,studentID
        add bx,[countID]   ;;;counter is db init position   
        
        lea dx,stIDInfo    ;;print info
        mov ah, 9
        int 21h
     input3:
        mov ah, 1
        int 21h
        cmp al,';'
        je inputScore
        mov [bx],al
        inc bx
        jmp input3   
     ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
     inputScore:
        inc bx
        mov [bx],'$'
        add [countID],11       
        lea bx,studentScore
        add bx,[countScore]   ;;;counter is db init position  
        mov cx,0
        lea dx,scoreInfo    ;;print info
        mov ah, 9
        int 21h
     input4:
        inc cl
        mov ah, 1
        int 21h 
        cmp al,';'
        je inputEnd
        cmp cl,1
        jne cl2
        
        ;;;; ch = al x 10
        mov ch,al
        sub ch,30h
        mov ah,ch
        shl ch,3
        add ch,ah
        add ch,ah
        push cx
        mov cl,ch
        mov ch,0
        add word ptr[averInter],cx
        pop cx 
        jmp clEnd
        
      cl2:
        cmp cl,2
        jne cl3
        mov ch,al
        sub ch,30h
        push cx
        mov cl,ch
        mov ch,0
        add word ptr[averInter],cx
        pop cx  
        jmp clEnd
        
      cl3:
        cmp cl,3
        je clEnd
        
        mov ch,al
        sub ch,30h
        add byte ptr[averDec],ch

        
        cmp byte ptr[averDec],10
        jb clEnd
        inc word ptr[averInter]
        sub byte ptr[averDec],10 
        
      clEnd:
        mov [bx],al
        inc bx
        jmp input4
     ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;   
     inputEnd:
        mov [bx],'$'
        add [countScore],5 
        lea dx,addItInfo
        mov ah,9
        int 21h
        
        pop cx
        pop bx     
        pop dx
        ret
        addInfo endp
    
      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    sortBy proc near
        push cx
        push bx
        push dx
        
        lea dx,sortInfo
        mov ah,9
        int 21h
        lea bx,studentSort
        mov cx,[countAll] 
       
        mov dx,0
     sortInit:
        inc dx
        mov [bx],dx
        add bx,2
        loop sortInit
        
        mov cx,[countAll]
        lea bx,studentSort 
         cmp cx,1
        je printLoop
        dec cx
     loop1:
        mov di,cx
        lea bx,studentScore
        lea dx,studentSort
     loop2:
        mov ax,[bx]
        cmp ax,[bx+5]
        je interEqual 
        ja count1
     exchange:
        push bx
        mov bx,dx
        mov ax,[bx]
        xchg ax,[bx+2]
        mov [bx],ax
        pop bx
        jmp count1
        
     interEqual:
       push ax
       mov al,[bx+3]
       cmp al,[bx+8] 
       jae count1
       pop ax
       jmp exchange
       
     count1:
        add bx,4
        add dx,2
        
        loop loop2:
        mov cx,di
        loop loop1
        
        mov cx,[countAll] 
        lea bx,studentSort 
        
        
     printLoop:
        mov ax,[bx]
        call printStudent
        add bx,2 
        
        push dx
        push ax
        lea dx,nextLine
        mov ah, 9
        int 21h
        pop ax
        pop dx 
        loop printLoop
         
        pop dx
        pop bx
        pop cx
        ret
        sortBy endp  
     ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;print average score
    getAverage proc near
        push cx
        push bx
        push dx
        
        mov cx,[countAll]
        mov dx,0
        mov ax,[averInter]
        div cx
        
        mov [average],ax
        
        lea dx,averInfo
        mov ah,9
        int 21h
        
        push average
        call printMemory 
        pop ax
        pop dx
        pop bx
        pop cx
        ret
        getAverage endp   
    
    ;;; push ax -- start address, printNum -- num
    printMemory proc near
        push bp 
        mov bp,sp
        push cx
        push si
        push bx 
        push dx 
        
        mov si,[bp+4]       
        mov ax,si
        mov cx,10
        mov dx,0
        div cx
        mov dl,al
        mov cl,dl
        add dl,'0'
        mov ah,2
        int 21h 

        mov al,10
        mul cl
        sub si,ax
        mov dx,si
        add dl,'0'
        mov ah,2
        int 21h
        
        pop dx
        pop bx
        pop si
        pop cx
        pop bp
        ret
        printMemory endp
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;get stages scores
    getStage proc near
        push cx
        push bx
        push dx
         
        ;;;clear data
        mov word ptr[score50],0
        mov word ptr[score60],0
        mov word ptr[score70],0
        mov word ptr[score80],0
        mov word ptr[score90],0
        
        mov cx,[countAll]
        lea bx,studentScore
       
     judge: 
        mov dl,[bx]
        sub dl,30h
        cmp dl,5
        jbe at50
        cmp dl,6
        je at60
        cmp dl,7
        je at70
        cmp dl,8
        je at80
        cmp dl,9
        je at90
        
     at50:
        inc [score50]
        jmp judgeLoop
     at60:
        inc [score60]
        jmp judgeLoop
     at70:
        inc [score70]
        jmp judgeLoop
     at80:
        inc [score80]
        jmp judgeLoop 
     at90:
        inc [score90]
        jmp judgeLoop  
        
     judgeLoop:
        add bx,5
        loop judge       
        
        lea dx,scorep50
        mov ah,9
        int 21h
        
        push score50
        call printMemory
        pop ax
         
        lea dx,scorep60
        mov ah,9
        int 21h
        
        push score60
        call printMemory
        pop ax        
        
        lea dx,scorep70
        mov ah,9
        int 21h
        
        push score70
        call printMemory
        pop ax 
        lea dx,scorep80
        mov ah,9
        int 21h
        
        push score80
        call printMemory
        pop ax
        lea dx,scorep90
        mov ah,9
        int 21h
        
        push score90
        call printMemory
        pop ax
        pop dx
        pop bx
        pop cx
        ret
        getStage endp 
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;print help info
    getCommand proc near
        push dx
        lea dx,displayinfo
        mov ah, 9
        int 21h 
        mov ah, 1
        int 21h
        sub al,30h
        pop dx 
        ret
        getCommand endp 
    
    printStudent proc near
        push bx
        push cx
        push dx
        
        dec al
        
        
        mov ch,al
        lea bx,studentName
        mov cl,21
        mul cl
        add bx,ax
        
        lea dx,pName
        mov ah, 9
        int 21h
        
        mov dx,bx
        mov ah,9
        int 21h
        
        mov ax,0
        mov al,ch
        lea bx,studentClass
        mov cl,3
        mul cl
        add bx,ax 
        
        lea dx,pClass
        mov ah, 9
        int 21h
        mov dx,bx
        mov ah,9
        int 21h
         
         mov ax,0
         mov al,ch
        lea bx,studentID
        mov cl,11
        mul cl
        add bx,ax
        lea dx,pID
        mov ah, 9
        int 21h
        mov dx,bx
        mov ah,9
        int 21h
        
        mov ax,0
         mov al,ch
        lea bx,studentScore
        mov cl,5
        mul cl
        add bx,ax
        lea dx,pScore
        mov ah, 9
        int 21h
        mov dx,bx
        mov ah,9
        int 21h
        
        pop dx 
        pop cx
        pop bx
        ret
        printStudent endp
code ends

