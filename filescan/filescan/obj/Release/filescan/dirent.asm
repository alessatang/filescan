; Listing generated by Microsoft (R) Optimizing Compiler Version 18.00.31101.0 

	TITLE	I:\Study\filescan\filescan\dirent.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB OLDNAMES

PUBLIC	??_C@_15DJBMFJKF@?$AA?1?$AA?2?$AA?$AA@		; `string'
PUBLIC	??_C@_13BBDEGPLJ@?$AA?$CK?$AA?$AA@		; `string'
PUBLIC	??_C@_15HHEIKGOF@?$AA?1?$AA?$CK?$AA?$AA@	; `string'
EXTRN	__imp___wfindnext64i32:PROC
EXTRN	__imp___findclose:PROC
EXTRN	__imp___errno:PROC
EXTRN	__imp___wstat64i32:PROC
EXTRN	__imp___wfindfirst64i32:PROC
EXTRN	__imp__malloc:PROC
EXTRN	__imp__calloc:PROC
EXTRN	__imp__free:PROC
EXTRN	__imp__wcschr:PROC
;	COMDAT ??_C@_15HHEIKGOF@?$AA?1?$AA?$CK?$AA?$AA@
CONST	SEGMENT
??_C@_15HHEIKGOF@?$AA?1?$AA?$CK?$AA?$AA@ DB '/', 00H, '*', 00H, 00H, 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_13BBDEGPLJ@?$AA?$CK?$AA?$AA@
CONST	SEGMENT
??_C@_13BBDEGPLJ@?$AA?$CK?$AA?$AA@ DB '*', 00H, 00H, 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_15DJBMFJKF@?$AA?1?$AA?2?$AA?$AA@
CONST	SEGMENT
??_C@_15DJBMFJKF@?$AA?1?$AA?2?$AA?$AA@ DB '/', 00H, '\', 00H, 00H, 00H ; `string'
PUBLIC	_readdir
PUBLIC	_closedir
PUBLIC	_opendir
; Function compile flags: /Ogtp
;	COMDAT _opendir
_TEXT	SEGMENT
_name$GSCopy$1$ = -64					; size = 4
_dir$1$ = -60						; size = 4
_stat_buf$1 = -56					; size = 48
__$ArrayPad$ = -4					; size = 4
_opendir PROC						; COMDAT
; _name$ = ecx
; File i:\study\filescan\filescan\dirent.c
; Line 35
	push	ebp
	mov	ebp, esp
	sub	esp, 68					; 00000044H
	mov	eax, DWORD PTR ___security_cookie
	xor	eax, ebp
	mov	DWORD PTR __$ArrayPad$[ebp], eax
	push	ebx
	push	esi
	push	edi
	mov	edi, ecx
	mov	DWORD PTR _name$GSCopy$1$[ebp], edi
; Line 38
	test	edi, edi
	je	$LN8@opendir
	xor	eax, eax
	cmp	ax, WORD PTR [edi]
	je	$LN8@opendir
; Line 43
	mov	esi, edi
	lea	ecx, DWORD PTR [esi+2]
$LL16@opendir:
	mov	ax, WORD PTR [esi]
	add	esi, 2
	test	ax, ax
	jne	SHORT $LL16@opendir
	sub	esi, ecx
	sar	esi, 1
; Line 45
	movzx	eax, WORD PTR [edi+esi*2-2]
	push	eax
	push	OFFSET ??_C@_15DJBMFJKF@?$AA?1?$AA?2?$AA?$AA@
	call	DWORD PTR __imp__wcschr
	test	eax, eax
	mov	ecx, OFFSET ??_C@_15HHEIKGOF@?$AA?1?$AA?$CK?$AA?$AA@
; Line 47
	push	1
	mov	ebx, OFFSET ??_C@_13BBDEGPLJ@?$AA?$CK?$AA?$AA@
	push	576					; 00000240H
	cmove	ebx, ecx
	call	DWORD PTR __imp__calloc
; Line 48
	mov	edx, ebx
	mov	DWORD PTR _dir$1$[ebp], eax
	add	esp, 16					; 00000010H
	lea	eax, DWORD PTR [edx+2]
$LL17@opendir:
	mov	cx, WORD PTR [edx]
	add	edx, 2
	test	cx, cx
	jne	SHORT $LL17@opendir
	sub	edx, eax
	sar	edx, 1
	lea	eax, DWORD PTR [edx+esi]
	lea	eax, DWORD PTR [eax*2+2]
	push	eax
	call	DWORD PTR __imp__malloc
	mov	ecx, eax
	add	esp, 4
	mov	eax, DWORD PTR _dir$1$[ebp]
	mov	DWORD PTR [eax+572], ecx
; Line 49
	test	ecx, ecx
	jne	SHORT $LN7@opendir
; Line 50
	call	DWORD PTR __imp___errno
; Line 51
	mov	ebx, DWORD PTR _dir$1$[ebp]
	mov	DWORD PTR [eax], 12			; 0000000cH
	jmp	$Error$22
$LN7@opendir:
; Line 54
	mov	edx, edi
	mov	edi, ecx
$LL12@opendir:
	movzx	eax, WORD PTR [edx]
	lea	edx, DWORD PTR [edx+2]
	mov	WORD PTR [ecx], ax
	lea	ecx, DWORD PTR [ecx+2]
	test	ax, ax
	jne	SHORT $LL12@opendir
	mov	esi, ebx
$LL18@opendir:
	mov	ax, WORD PTR [ebx]
	add	ebx, 2
	test	ax, ax
	jne	SHORT $LL18@opendir
	sub	ebx, esi
	add	edi, -2					; fffffffeH
$LL19@opendir:
	mov	ax, WORD PTR [edi+2]
	add	edi, 2
	test	ax, ax
	jne	SHORT $LL19@opendir
	mov	ecx, ebx
	shr	ecx, 2
	rep movsd
	mov	ecx, ebx
; Line 56
	mov	ebx, DWORD PTR _dir$1$[ebp]
	and	ecx, 3
	rep movsb
	lea	eax, DWORD PTR [ebx+8]
	push	eax
	push	DWORD PTR [ebx+572]
	call	DWORD PTR __imp___wfindfirst64i32
	add	esp, 8
	mov	DWORD PTR [ebx], eax
; Line 57
	cmp	eax, -1
	jne	SHORT $LN5@opendir
; Line 58
	push	44					; 0000002cH
	lea	eax, DWORD PTR _stat_buf$1[ebp+4]
	mov	DWORD PTR _stat_buf$1[ebp], 0
	push	0
	push	eax
	call	_memset
; Line 59
	lea	eax, DWORD PTR _stat_buf$1[ebp]
	push	eax
	push	DWORD PTR _name$GSCopy$1$[ebp]
	call	DWORD PTR __imp___wstat64i32
	add	esp, 20					; 00000014H
; Line 60
	cmp	eax, -1
	jne	SHORT $LN4@opendir
; Line 61
	call	DWORD PTR __imp___errno
	mov	DWORD PTR [eax], 2
; Line 69
	jmp	SHORT $Error$22
$LN4@opendir:
; Line 64
	call	DWORD PTR __imp___errno
	mov	DWORD PTR [eax], 20			; 00000014H
$Error$22:
; Line 71
	push	DWORD PTR [ebx+572]
	mov	esi, DWORD PTR __imp__free
	call	esi
; Line 72
	push	ebx
	call	esi
	add	esp, 8
	xor	eax, eax
; Line 74
	pop	edi
	pop	esi
	pop	ebx
	mov	ecx, DWORD PTR __$ArrayPad$[ebp]
	xor	ecx, ebp
	call	@__security_check_cookie@4
	mov	esp, ebp
	pop	ebp
	ret	0
$LN5@opendir:
; Line 67
	mov	DWORD PTR [ebx+568], 0
; Line 68
	mov	eax, ebx
; Line 74
	pop	edi
	pop	esi
	pop	ebx
	mov	ecx, DWORD PTR __$ArrayPad$[ebp]
	xor	ecx, ebp
	call	@__security_check_cookie@4
	mov	esp, ebp
	pop	ebp
	ret	0
$LN8@opendir:
; Line 39
	call	DWORD PTR __imp___errno
; Line 74
	mov	ecx, DWORD PTR __$ArrayPad$[ebp]
	pop	edi
	pop	esi
	mov	DWORD PTR [eax], 2
	xor	ecx, ebp
	xor	eax, eax
	pop	ebx
	call	@__security_check_cookie@4
	mov	esp, ebp
	pop	ebp
	ret	0
_opendir ENDP
_TEXT	ENDS
; Function compile flags: /Ogtp
;	COMDAT _closedir
_TEXT	SEGMENT
_closedir PROC						; COMDAT
; _dir$ = ecx
; File i:\study\filescan\filescan\dirent.c
; Line 77
	push	ebx
	push	edi
	mov	ebx, ecx
; Line 78
	or	edi, -1
; Line 80
	test	ebx, ebx
	je	SHORT $LN6@closedir
; Line 82
	mov	eax, DWORD PTR [ebx]
	cmp	eax, edi
	je	SHORT $LN2@closedir
; Line 84
	push	eax
	call	DWORD PTR __imp___findclose
	add	esp, 4
	mov	edi, eax
$LN2@closedir:
; Line 87
	push	DWORD PTR [ebx+572]
	call	DWORD PTR __imp__free
; Line 88
	push	ebx
	call	DWORD PTR __imp__free
	add	esp, 8
; Line 91
	cmp	edi, -1
	jne	SHORT $LN8@closedir
$LN6@closedir:
; Line 93
	call	DWORD PTR __imp___errno
	mov	DWORD PTR [eax], 9
$LN8@closedir:
; Line 96
	mov	eax, edi
	pop	edi
	pop	ebx
; Line 97
	ret	0
_closedir ENDP
_TEXT	ENDS
; Function compile flags: /Ogtp
;	COMDAT _readdir
_TEXT	SEGMENT
_readdir PROC						; COMDAT
; _dir$ = ecx
; File i:\study\filescan\filescan\dirent.c
; Line 100
	push	esi
	mov	esi, ecx
; Line 103
	test	esi, esi
	je	SHORT $LN4@readdir
	mov	edx, DWORD PTR [esi]
	cmp	edx, -1
	je	SHORT $LN4@readdir
; Line 105
	cmp	DWORD PTR [esi+568], 0
	push	edi
	je	SHORT $LN2@readdir
	lea	ecx, DWORD PTR [esi+8]
	push	ecx
	push	edx
	call	DWORD PTR __imp___wfindnext64i32
	add	esp, 8
	cmp	eax, -1
	jne	SHORT $LN2@readdir
; Line 116
	pop	edi
	xor	eax, eax
	pop	esi
; Line 117
	ret	0
$LN2@readdir:
; Line 108
	lea	ecx, DWORD PTR [esi+44]
	pop	edi
	mov	DWORD PTR [esi+568], ecx
; Line 116
	lea	eax, DWORD PTR [esi+568]
	pop	esi
; Line 117
	ret	0
$LN4@readdir:
; Line 113
	call	DWORD PTR __imp___errno
	pop	esi
	mov	DWORD PTR [eax], 9
; Line 116
	xor	eax, eax
; Line 117
	ret	0
_readdir ENDP
_TEXT	ENDS
END
