; Listing generated by Microsoft (R) Optimizing Compiler Version 18.00.31101.0 

	TITLE	I:\Study\filescan\filescan\scandir.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB OLDNAMES

EXTRN	__imp__realloc:PROC
PUBLIC	?ag_scandir@@YAHPB_WPAPAPAUdirent@@P6AH0PBU1@PAX@Z3@Z ; ag_scandir
; Function compile flags: /Ogtp
;	COMDAT ?ag_scandir@@YAHPB_WPAPAPAUdirent@@P6AH0PBU1@PAX@Z3@Z
_TEXT	SEGMENT
_namelist$1$ = -28					; size = 4
_tmp_names$1$ = -24					; size = 4
_dirp$1$ = -20						; size = 4
_result$1$ = -16					; size = 4
_dirname$1$ = -12					; size = 4
_result$1$ = -8						; size = 4
_names_len$1$ = -8					; size = 4
_results_len$1$ = -4					; size = 4
_filter$dead$ = 8					; size = 4
_baton$ = 12						; size = 4
?ag_scandir@@YAHPB_WPAPAPAUdirent@@P6AH0PBU1@PAX@Z3@Z PROC ; ag_scandir, COMDAT
; _dirname$ = ecx
; _namelist$ = edx
; File i:\study\filescan\filescan\scandir.c
; Line 10
	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	eax, ecx
	mov	DWORD PTR _namelist$1$[ebp], edx
; Line 15
	xor	edi, edi
	mov	DWORD PTR _dirname$1$[ebp], eax
	mov	DWORD PTR _names_len$1$[ebp], 32	; 00000020H
	mov	DWORD PTR _results_len$1$[ebp], edi
; Line 17
	call	_opendir
	mov	esi, eax
	mov	DWORD PTR _dirp$1$[ebp], esi
; Line 18
	test	esi, esi
	je	$LN4@ag_scandir
; Line 22
	push	128					; 00000080H
	call	DWORD PTR __imp__malloc
	mov	ebx, eax
	add	esp, 4
; Line 23
	test	ebx, ebx
	je	$fail$69
	npad	9
$LL11@ag_scandir:
; File i:\study\filescan\filescan\dirent.c
; Line 103
	mov	ecx, DWORD PTR [esi]
	cmp	ecx, -1
	je	$LN21@ag_scandir
; Line 105
	cmp	DWORD PTR [esi+568], 0
	lea	edi, DWORD PTR [esi+568]
	mov	DWORD PTR _result$1$[ebp], edi
	je	SHORT $LN19@ag_scandir
	lea	eax, DWORD PTR [esi+8]
	push	eax
	push	ecx
	call	DWORD PTR __imp___wfindnext64i32
	add	esp, 8
	cmp	eax, -1
	je	$LN10@ag_scandir
$LN19@ag_scandir:
; File i:\study\filescan\filescan\scandir.c
; Line 28
	push	DWORD PTR _baton$[ebp]
; File i:\study\filescan\filescan\dirent.c
; Line 108
	lea	eax, DWORD PTR [esi+44]
; File i:\study\filescan\filescan\scandir.c
; Line 28
	push	edi
	push	DWORD PTR _dirname$1$[ebp]
; File i:\study\filescan\filescan\dirent.c
; Line 108
	mov	DWORD PTR [edi], eax
; File i:\study\filescan\filescan\scandir.c
; Line 28
	call	?filename_filter@@YAHPB_WPBUdirent@@PAX@Z ; filename_filter
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $LL11@ag_scandir
; Line 31
	mov	eax, DWORD PTR _names_len$1$[ebp]
	cmp	DWORD PTR _results_len$1$[ebp], eax
	jl	SHORT $LN7@ag_scandir
; Line 33
	add	eax, eax
	mov	DWORD PTR _tmp_names$1$[ebp], ebx
	mov	DWORD PTR _names_len$1$[ebp], eax
; Line 34
	shl	eax, 2
	push	eax
	push	ebx
	call	DWORD PTR __imp__realloc
	mov	ebx, eax
	add	esp, 8
; Line 35
	test	ebx, ebx
	je	SHORT $LN61@ag_scandir
$LN7@ag_scandir:
; Line 43
	mov	esi, DWORD PTR [edi]
	lea	ecx, DWORD PTR [esi+2]
$LL67@ag_scandir:
	mov	ax, WORD PTR [esi]
	add	esi, 2
	test	ax, ax
	jne	SHORT $LL67@ag_scandir
	sub	esi, ecx
; Line 44
	push	4
	sar	esi, 1
	call	DWORD PTR __imp__malloc
	add	esp, 4
; Line 45
	lea	esi, DWORD PTR [esi*2+2]
	mov	edi, eax
	push	esi
	call	DWORD PTR __imp__malloc
; Line 47
	mov	ecx, DWORD PTR _result$1$[ebp]
	add	esp, 4
	mov	DWORD PTR [edi], eax
	push	esi
	push	DWORD PTR [ecx]
	push	eax
	call	_memcpy
; Line 68
	mov	eax, DWORD PTR _results_len$1$[ebp]
	add	esp, 12					; 0000000cH
; Line 69
	inc	DWORD PTR _results_len$1$[ebp]
; Line 70
	mov	esi, DWORD PTR _dirp$1$[ebp]
	mov	DWORD PTR [ebx+eax*4], edi
	jmp	$LL11@ag_scandir
$LN61@ag_scandir:
; Line 36
	push	DWORD PTR _tmp_names$1$[ebp]
	call	DWORD PTR __imp__free
	mov	edi, DWORD PTR _results_len$1$[ebp]
	add	esp, 4
$fail$69:
; File i:\study\filescan\filescan\dirent.c
; Line 82
	mov	eax, DWORD PTR [esi]
	mov	DWORD PTR _result$1$[ebp], -1
	cmp	eax, -1
	je	SHORT $LN54@ag_scandir
; Line 84
	push	eax
	call	DWORD PTR __imp___findclose
	add	esp, 4
	mov	DWORD PTR _result$1$[ebp], eax
$LN54@ag_scandir:
; Line 87
	push	DWORD PTR [esi+572]
	call	DWORD PTR __imp__free
	add	esp, 4
; Line 88
	push	esi
	call	DWORD PTR __imp__free
	add	esp, 4
; Line 91
	cmp	DWORD PTR _result$1$[ebp], -1
	jne	SHORT $LN53@ag_scandir
; Line 93
	call	DWORD PTR __imp___errno
	mov	DWORD PTR [eax], 9
$LN53@ag_scandir:
; File i:\study\filescan\filescan\scandir.c
; Line 81
	test	ebx, ebx
	je	SHORT $LN4@ag_scandir
; Line 83
	xor	esi, esi
	test	edi, edi
	jle	SHORT $LN1@ag_scandir
$LL3@ag_scandir:
; Line 84
	push	DWORD PTR [ebx+esi*4]
	call	DWORD PTR __imp__free
	inc	esi
	add	esp, 4
	cmp	esi, edi
	jl	SHORT $LL3@ag_scandir
$LN1@ag_scandir:
; Line 86
	push	ebx
	call	DWORD PTR __imp__free
	add	esp, 4
$LN4@ag_scandir:
; Line 88
	or	eax, -1
; Line 89
	pop	edi
	pop	esi
	pop	ebx
	mov	esp, ebp
	pop	ebp
	ret	0
$LN21@ag_scandir:
; File i:\study\filescan\filescan\dirent.c
; Line 113
	call	DWORD PTR __imp___errno
	mov	DWORD PTR [eax], 9
$LN10@ag_scandir:
; File i:\study\filescan\filescan\scandir.c
; Line 72
	mov	ecx, esi
	call	_closedir
; Line 73
	mov	eax, DWORD PTR _namelist$1$[ebp]
; Line 89
	pop	edi
	pop	esi
	mov	DWORD PTR [eax], ebx
	mov	eax, DWORD PTR _results_len$1$[ebp]
	pop	ebx
	mov	esp, ebp
	pop	ebp
	ret	0
?ag_scandir@@YAHPB_WPAPAPAUdirent@@P6AH0PBU1@PAX@Z3@Z ENDP ; ag_scandir
_TEXT	ENDS
END
