;Matthew Parrish
;CS301, Project 1
;Frogger in Assembly
;the actual assembly file

section .text
global _mapAdvance
global _getMapLine
global _movToon
global _getToonY
global _getToonX
global _getAlive
global _setDead
global _getStartChar

extern _getchar
;extern _getch ;USE IF YOUR COMPUTER SUPPORTS

	
;Pre conditions: None
;Post conditions: returns a char
_getStartChar:
	call _getchar
	ret

;Pre condition: None
;Post condition: returns 1 if player alive, 0 if not, effectively a bool
_getAlive:
	mov eax, DWORD[alive]
	ret

;Pre condition: None
;Post condition: Sets player to dead, essential sets to false
_setDead:
	mov DWORD[alive], 0
	ret
	
;Pre condition: None
;Post condition: Get input, then moves character accordingly. Won't move off map.
_movToon: 
	call _getchar
;	call _getch ;USE IF COMPUTER SUPPORTS
	
	cmp eax, 's'
	je addOneY ;MOVE DOWN

	cmp eax, 'w'
	je subOneY ;MOVE UP

	cmp eax, 'a'
	je subOneX ;MOVES LEFT

	cmp eax, 'd'
	je addOneX ;MOVES RIGHT

	ret 
 
;Pre condition: None
;Post condition: returns without moving player if try to go off screen
badMoveValue:
	ret
	
;Pre condition: None
;Post condition: Moves player right, unless would go off screen, then do nothing
addOneX:
	mov ecx, [xRightBound]
	cmp DWORD[toonLocation], ecx 
	ja badMoveValue

	add DWORD[toonLocation], 1
	ret

;Pre condition: None
;Post condition: Moves player left, unless would go off screen, then do nothing
subOneX:
	mov ecx, [xLeftBound]
	cmp DWORD[toonLocation], ecx
	jb badMoveValue

	sub DWORD[toonLocation], 1
	ret

;Pre condition: None
;Post condition: Moves player down, unless would go off screen, then do nothing
addOneY:
	mov ecx, [yBottomBound]
	cmp DWORD[toonLocation+4], ecx
	ja badMoveValue

	add DWORD[toonLocation+4], 1
	ret

;Pre condition: None
;Post condition: Moves player up, can't go off screen, because win stops program
subOneY:
	sub DWORD[toonLocation+4], 1
	ret

;Pre condition: None
;Post condition: Returns player Y location
_getToonY:
	mov eax, DWORD[toonLocation+4]
	ret

;Pre condition: None
;Post condition: Returns player X location
_getToonX:
	mov eax, DWORD[toonLocation]
	ret

;Pre condition: None
;Post condition: Returns a specified line of the map
_getMapLine: ;Array, element 0 is first
	mov eax, DWORD[mapInfo+4*eax] 
	ret
	
;Pre condition: None
;Post condition: Advances the base map (not player) one turn
_mapAdvance: 
	rol DWORD[mapInfo +  4], 1
	ror DWORD[mapInfo +  8], 1
	rol DWORD[mapInfo + 12], 1 ;water section
	ror DWORD[mapInfo + 16], 1
	rol DWORD[mapInfo + 20], 1

	rol DWORD[mapInfo + 28], 2
	ror DWORD[mapInfo + 32], 1
	rol DWORD[mapInfo + 36], 1 ;traffic section
	ror DWORD[mapInfo + 40], 1
	rol DWORD[mapInfo + 44], 1
	ret

section .data
mapInfo: ;1s good, 0 kill
	 ;two bits of 0s at the beginning for cushion and wraparound
	dd 0x1d6cdd6c ;end zone,               [+0
	dd 0x3bae3bae ;water, shift 1, right   [+4
	dd 0x33323332 ;water, shift 1, left    [+8
	dd 0x1f3e1f3e ;water, shift 1 right    [+12
	dd 0x38703870 ;water, shift 1, right   [+16
	dd 0x2eee2eee ;water, shift 1, left    [+20
	dd 0xffffffff ;middle                  [+24
	dd 0x3f3c3f3c ;traffic, shift 2 left   [+28
	dd 0xf3eff3ef ;traffic, shift 1, right [+32
	dd 0xbd35bd35 ;traffic, shift 1, left  [+36
	dd 0xb2f7b2f7 ;traffic, shift 1, right [+40
	dd 0x1ef71ef7 ;traffic, shift 1, left  [+44
	dd 0xffffffff ;start                   [+48

toonLocation: ;toonLocation + 0 = X
	      ;toonLocation + 4 = Y
	dd 13 ;x-location, int
	dd 12 ;y-location, int

alive: ;1 for alive, 0 for dead, effectively a bool
	dd 1 

xRightBound:  ;maximum value allowed for player's X location
	dd 25
xLeftBound:   ;minimum value allowed for player's X location
	dd 1
yBottomBound: ;maximum value allowed for player's Y location
	dd 11
