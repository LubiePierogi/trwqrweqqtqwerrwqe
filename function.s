;
;
; Asemblerowa funkcja robiąca filtr medianowy
;
;



section .text
global function

function:

prologue:
		push    ebp
		mov	    ebp,esp
		sub     esp,40

		push    ebx
		push    esi
		push    edi

;   [ebp+4]  return
;   [ebp+8]  pierwszy wskaźnik
;   [ebp+12] drugi wskaźnik
;   [ebp+16] szerokość
;   [ebp+20] wysokość
;   [ebp+24] za argumentami już

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

		mov esi,DWORD[ebp-32]; esi=Y
		cmp esi,[ebp+20]; esi ? wysokosc

		jge verticalLoopEnd ; skakanko

horizontalLoop:

		inc DWORD[ebp-24] ; ++X
		mov eax,DWORD[ebp-24]; eax=X
		cmp eax,[ebp+16] ; eax ? szerokosc
		jge horizontalLoopEnd ; skakaneczko
		add [ebp-28],DWORD 4; adresCzytania+=4
		add [ebp-40],DWORD 4; adresPisania+=4

		mov eax,DWORD[ebp-24]
           	; w eax mamy teraz X
		mov esi,DWORD[ebp-32]
	    ; w esi mamy teraz Y

	    ;Tutaj już wezmę adresy zapisu i czytania
	 	mov ecx,[ebp-40] ; adres do zapisu
	 	mov ebx,[ebp-28]; adres czytania
	 	mov edi,[ebx] ; już przeczytane

	 ; w ebx mamy adres czytania
	 ; ale mamy X w eax i Y w esi xD

		test eax,eax
		jz copyOld
		test esi,esi
		jz copyOld

		cmp eax,[ebp-16]
		je copyOld
		cmp esi,[ebp-20]
		je copyOld

computeMedian:;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; NIE RUSZAĆ ESI!!!
; EAX TEŻ!!!
; I JESZCZE ECX!!!
;I jescze ebx, ale W EBX JEST ADRES CZYTANIA!

; w edi jest stary piksel niepotrzebny

; dobra dam push

		push esi
		push eax

	;mov [ecx],edi
	;xor esi,esi
		xor edx,edx ; od teraz w edx jest przesunięcie na kanał od koloru
		mov esi,[ebp-36]; tutaj do esi wczytujemy sobie szerokosc linii w bajtach

medianLoop:
;edx zajęte
		add ebx,edx; przesuwamy się na kolor, któ©y zmieniamy
		sub ebx,esi; dodatkowo cofamu się o linię

		mov al,BYTE[ebx-4]
		mov [ebp-1],al
		mov al,BYTE[ebx]
		mov [ebp-2],al
		mov al,BYTE[ebx+4]
		mov [ebp-3],al

		add ebx,esi ; jesteśmy teraz na naszej linii


		mov al,BYTE[ebx-4]
		mov [ebp-4],al
		mov al,BYTE[ebx]
		mov [ebp-5],al
		mov al,BYTE[ebx+4]
		mov [ebp-6],al



		add ebx,esi ; jesteśmy teraz w następnej linii

		mov al,BYTE[ebx-4]
		mov [ebp-7],al
		mov al,BYTE[ebx]
		mov [ebp-8],al
		mov al,BYTE[ebx+4]
		mov [ebp-9],al


		sub ebx,esi; wracamy do linii


bubbleSort:
		xor eax,eax
		push ecx
		push esi
		push ebx

		mov ecx,8
		mov esi,ebp
		sub esi,9
		mov ebx,esi
		add ecx,esi
bubbleSortLoop:
				; esi licznik pierwszego
				; ebx licznik pierwszego do cofania
				; ecx licznik ostatniego
				; eax miejsce do porównywania

internalBubbleSortLoop:

		mov al,BYTE[esi]
		mov ah,BYTE[ecx]
		cmp ah,al
		jge dontSwap

		mov [esi],ah
		mov [ecx],al
dontSwap:
		inc esi
		cmp esi,ecx
		jl internalBubbleSortLoop
internalBubbleSortEnd:

		mov esi,ebx
		dec ecx
		cmp ecx,esi
		jg bubbleSortLoop
bubbleSortEnd:
		pop ebx
		pop esi
		pop ecx
				; eax nie jest używane
		xor ax,ax
		mov al,BYTE[ebp-5]

		sub ebx,edx; wracamy spowrotem, bo trzeba

		mov [ecx+edx],al ; wpisanie wyniku na miejsce



		inc edx
		cmp edx,4
		jl medianLoop
medianLoopEnd:

		pop eax
endComputingMedian:;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		pop esi
  	jmp horizontalLoop
copyOld:
		mov [ecx],edi
endCopyingOld:
		jmp horizontalLoop
horizontalLoopEnd:
 		mov [ebp-24], DWORD -1 ; X=-1

		jmp verticalLoop
verticalLoopEnd:

epilogue:

		pop     edi
		pop     esi
		pop     ebx


		mov     esp,ebp ; znowu stary stack pointer
		pop     ebp ;przywroconko starego frame pointera
		ret
