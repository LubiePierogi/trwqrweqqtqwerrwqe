section .text
global function

function:

prologue:
	push    ebp
	mov	    ebp,esp
	sub     esp,2000

	push    ebx
	push    esi
	push    edi

;   [ebp+4]  return
;   [ebp+8]  pierwszy wskaźnik
;   [ebp+12] drugi wskaźnik
;   [ebp+16] szerokość
;   [ebp+20] wysokość
;   [ebp+24] koniec argumentóœ

;   [ebp]    stare ebp

;   [ebp-1]:[ebp-9]  miejsca na liczenie miediany
;		[ebp-16]    szerokosc zmniejszona o 1
;   [ebp-20]    wysokosc zmniejszona o 1
;   [ebp-24]    miejsce teraz w poziomie
;   [ebp-28]  ADRES  miejsce teraz w poziomie * 4 bezwzględnie
;   [ebp-32]   miejsce tearaz w pionie
;   [ebp-36]      szerokosc linii w bajtach
;   [ebp-40]   ADRES zapisu
;

;mov [esp],DWORD 0

core:

	xor eax,eax
	xor ecx,ecx



	mov eax,DWORD[ebp+16]
	cmp eax,2
	jle epilogue
	or ecx,eax
	dec eax
	mov [ebp-16],eax    ; zrobienie szerokości-1
	shl ecx,2
	mov [ebp-36],ecx   ; zrobienie szerokości linii w bajtach


  mov eax,DWORD[ebp+20]
	cmp eax,2
	jle epilogue
	dec eax
	mov [ebp-20],eax  ; zrobienie wysokości-1


	mov [ebp-24],DWORD -1 ; X=-1
	mov [ebp-32],DWORD -1 ; Y=-1
 ;;miejsca /\
 ;        /||\
 ;         ||



	;;;
	;ADRES!
	;;
	;[ebp-28]


	mov eax,DWORD[ebp+8]
	mov ecx,DWORD[ebp+12]
	add eax,-4
	add ecx,-4
	mov [ebp-28],eax; adresCzytania=-4+pierwszy
	mov [ebp-40],ecx; adresPisania=-4+drugi
	;; to było zrobienie wskaźników na miejsce do czytania i pisania

verticalLoop:

	inc DWORD[ebp-32]; ++Y

	mov eax,DWORD[ebp+20]; eax=wysokosc
	cmp eax,[ebp-32]; eax ? Y

	jle verticalLoopEnd ; skakanko

	;jmp DWORD 0

horizontalLoop:

	inc DWORD[ebp-24] ; ++X
	mov eax,DWORD[ebp+16]; eax=szerokosc
	cmp eax,[ebp-24] ; eax ? X
	jle horizontalLoopEnd ; skakaneczko
	add [ebp-28],DWORD 4; adresCzytania+=4
	add [ebp-40],DWORD 4; adresPisania+=4

computeMedian:

	mov eax,[ebp-40]

	mov [eax],DWORD 0xffff00ff

endComputingMedian:



	jmp horizontalLoop
horizontalLoopEnd:
 mov [ebp-24], DWORD -1 ; X=-1
; add [ebp-28], DWORD 4 ; adresCzytania+=4
; add [ebp-40], DWORD 4 ; adresPisania+=4



	jmp verticalLoop
verticalLoopEnd:

epilogue:

	pop     edi
	pop     esi
	pop     ebx


	mov     esp,ebp ; znowu stary stack pointer
	pop     ebp ;przywroconko starego frame pointera
	ret
