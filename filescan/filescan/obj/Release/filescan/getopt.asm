; Listing generated by Microsoft (R) Optimizing Compiler Version 18.00.31101.0 

	TITLE	I:\Study\filescan\filescan\getopt.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB OLDNAMES

PUBLIC	??_C@_13IMODFHAA@?$AA?9?$AA?$AA@		; `string'
PUBLIC	??_C@_15KHFHDPFH@?$AA?9?$AA?9?$AA?$AA@		; `string'
PUBLIC	??_C@_13NMPKAAJP@?$AA?$DN?$AA?$AA@		; `string'
PUBLIC	_opterr
PUBLIC	_optopt
PUBLIC	_required_argument
PUBLIC	_optional_argument
PUBLIC	_optarg
PUBLIC	_optind
PUBLIC	_no_argument
EXTRN	__imp__wcsncmp:PROC
EXTRN	__imp__wcscspn:PROC
_opterr	DD	01H DUP (?)
_optopt	DD	01H DUP (?)
_optarg	DD	01H DUP (?)
_optcursor DD	01H DUP (?)
_BSS	ENDS
_optind	DD	01H
_required_argument DD 01H
_optional_argument DD 02H
_no_argument DD	00H
CONST	ENDS
;	COMDAT ??_C@_13NMPKAAJP@?$AA?$DN?$AA?$AA@
CONST	SEGMENT
??_C@_13NMPKAAJP@?$AA?$DN?$AA?$AA@ DB '=', 00H, 00H, 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_15KHFHDPFH@?$AA?9?$AA?9?$AA?$AA@
CONST	SEGMENT
??_C@_15KHFHDPFH@?$AA?9?$AA?9?$AA?$AA@ DB '-', 00H, '-', 00H, 00H, 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_13IMODFHAA@?$AA?9?$AA?$AA@
CONST	SEGMENT
??_C@_13IMODFHAA@?$AA?9?$AA?$AA@ DB '-', 00H, 00H, 00H	; `string'
PUBLIC	_getopt_long
PUBLIC	_getopt
PUBLIC	?wcschr@@YAPA_WPA_W_W@Z				; wcschr
; Function compile flags: /Ogtp
;	COMDAT ?wcschr@@YAPA_WPA_W_W@Z
_TEXT	SEGMENT
?wcschr@@YAPA_WPA_W_W@Z PROC				; wcschr, COMDAT
; __Str$ = ecx
; __Ch$dead$ = dx
; File c:\program files (x86)\microsoft visual studio 12.0\vc\include\string.h
; Line 383
	push	61					; 0000003dH
	push	ecx
	call	DWORD PTR __imp__wcschr
	add	esp, 8
	ret	0
?wcschr@@YAPA_WPA_W_W@Z ENDP				; wcschr
_TEXT	ENDS
; Function compile flags: /Ogtp
;	COMDAT _getopt
_TEXT	SEGMENT
_argc$1$ = -4						; size = 4
_optstring$dead$ = 8					; size = 4
_getopt	PROC						; COMDAT
; _argc$ = ecx
; _argv$ = edx
; File i:\study\filescan\filescan\getopt.c
; Line 58
	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
; Line 67
	mov	esi, DWORD PTR _optind
	mov	eax, ecx
	mov	DWORD PTR _argc$1$[ebp], eax
	mov	ebx, edx
	mov	DWORD PTR _optarg, 0
	mov	DWORD PTR _opterr, 0
	mov	DWORD PTR _optopt, 0
	push	edi
	cmp	esi, eax
	jge	$no_more_optchars$36
; Line 72
	mov	edi, DWORD PTR [ebx+esi*4]
	test	edi, edi
	je	SHORT $no_more_optchars$36
; Line 77
	cmp	WORD PTR [edi], 45			; 0000002dH
	jne	SHORT $no_more_optchars$36
; Line 82
	mov	ecx, OFFSET ??_C@_13IMODFHAA@?$AA?9?$AA?$AA@
	mov	eax, edi
	npad	2
$LL25@getopt:
	mov	dx, WORD PTR [eax]
	cmp	dx, WORD PTR [ecx]
	jne	SHORT $LN26@getopt
	test	dx, dx
	je	SHORT $LN27@getopt
	mov	dx, WORD PTR [eax+2]
	cmp	dx, WORD PTR [ecx+2]
	jne	SHORT $LN26@getopt
	add	eax, 4
	add	ecx, 4
	test	dx, dx
	jne	SHORT $LL25@getopt
$LN27@getopt:
	xor	eax, eax
	jmp	SHORT $LN28@getopt
$LN26@getopt:
	sbb	eax, eax
	or	eax, 1
$LN28@getopt:
	test	eax, eax
	je	SHORT $no_more_optchars$36
; Line 87
	mov	ecx, OFFSET ??_C@_15KHFHDPFH@?$AA?9?$AA?9?$AA?$AA@
	mov	eax, edi
$LL29@getopt:
	mov	dx, WORD PTR [eax]
	cmp	dx, WORD PTR [ecx]
	jne	SHORT $LN30@getopt
	test	dx, dx
	je	SHORT $LN31@getopt
	mov	dx, WORD PTR [eax+2]
	cmp	dx, WORD PTR [ecx+2]
	jne	SHORT $LN30@getopt
	add	eax, 4
	add	ecx, 4
	test	dx, dx
	jne	SHORT $LL29@getopt
$LN31@getopt:
	xor	eax, eax
	jmp	SHORT $LN32@getopt
$LN30@getopt:
	sbb	eax, eax
	or	eax, 1
$LN32@getopt:
	test	eax, eax
	jne	SHORT $LN14@getopt
; Line 88
	inc	esi
	mov	DWORD PTR _optind, esi
$no_more_optchars$36:
; Line 156
	pop	edi
	pop	esi
	mov	DWORD PTR _optcursor, 0
; Line 157
	or	eax, -1
	pop	ebx
; Line 158
	mov	esp, ebp
	pop	ebp
	ret	0
$LN14@getopt:
; Line 92
	mov	eax, DWORD PTR _optcursor
	test	eax, eax
	je	SHORT $LN11@getopt
	cmp	WORD PTR [eax], 0
	jne	SHORT $LN12@getopt
$LN11@getopt:
; Line 93
	lea	eax, DWORD PTR [edi+2]
	mov	DWORD PTR _optcursor, eax
$LN12@getopt:
; Line 95
	movzx	esi, WORD PTR [eax]
; Line 104
	push	esi
	push	OFFSET ??_C@_1FA@PFJCFACM@?$AAA?$AA?3?$AAa?$AAB?$AA?3?$AAC?$AA?3?$AAc?$AAD?$AAG?$AA?3?$AAg?$AA?3?$AAF?$AAf?$AAH?$AAh?$AAi?$AAL?$AAl?$AAm?$AA?3?$AAn?$AAo?$AAp?$AA?3?$AAQ?$AAR?$AAr?$AAS?$AAs?$AAv@
	mov	DWORD PTR _optopt, esi
	call	DWORD PTR __imp__wcschr
	mov	ecx, eax
	add	esp, 8
; Line 105
	test	ecx, ecx
	je	$LN10@getopt
; Line 108
	cmp	WORD PTR [ecx+2], 58			; 0000003aH
	jne	$LN3@getopt
; Line 109
	mov	eax, DWORD PTR _optcursor
	add	eax, 2
	mov	DWORD PTR _optarg, eax
; Line 110
	cmp	WORD PTR [eax], 0
	jne	$LN33@getopt
; Line 116
	cmp	WORD PTR [ecx+4], 58			; 0000003aH
; Line 127
	mov	ecx, DWORD PTR _optind
	je	SHORT $LN7@getopt
	inc	ecx
	cmp	ecx, DWORD PTR _argc$1$[ebp]
	jge	SHORT $LN6@getopt
; Line 128
	mov	eax, DWORD PTR [ebx+ecx*4]
; Line 151
	inc	ecx
	pop	edi
	mov	DWORD PTR _optarg, eax
; Line 153
	mov	eax, esi
	pop	esi
	mov	DWORD PTR _optcursor, 0
	mov	DWORD PTR _optind, ecx
	pop	ebx
; Line 158
	mov	esp, ebp
	pop	ebp
	ret	0
$LN6@getopt:
; Line 135
	mov	esi, 63					; 0000003fH
	mov	DWORD PTR _optarg, 0
	pop	edi
; Line 153
	mov	eax, esi
	mov	DWORD PTR _optcursor, 0
	inc	ecx
	pop	esi
	mov	DWORD PTR _optind, ecx
	pop	ebx
; Line 158
	mov	esp, ebp
	pop	ebp
	ret	0
$LN7@getopt:
	pop	edi
; Line 153
	mov	eax, esi
	mov	DWORD PTR _optarg, 0
	inc	ecx
	mov	DWORD PTR _optcursor, 0
	pop	esi
	mov	DWORD PTR _optind, ecx
	pop	ebx
; Line 158
	mov	esp, ebp
	pop	ebp
	ret	0
$LN33@getopt:
; Line 138
	mov	ecx, DWORD PTR _optind
; Line 153
	mov	eax, esi
	pop	edi
	inc	ecx
	mov	DWORD PTR _optcursor, 0
	pop	esi
	mov	DWORD PTR _optind, ecx
	pop	ebx
; Line 158
	mov	esp, ebp
	pop	ebp
	ret	0
$LN10@getopt:
; Line 147
	mov	esi, 63					; 0000003fH
$LN3@getopt:
; Line 150
	mov	eax, DWORD PTR _optcursor
	test	eax, eax
	je	SHORT $LN34@getopt
	add	eax, 2
	mov	DWORD PTR _optcursor, eax
	cmp	WORD PTR [eax], 0
	jne	SHORT $LN2@getopt
$LN34@getopt:
	mov	ecx, DWORD PTR _optind
; Line 151
	inc	ecx
	mov	DWORD PTR _optind, ecx
$LN2@getopt:
; Line 153
	pop	edi
	mov	eax, esi
	pop	esi
	pop	ebx
; Line 158
	mov	esp, ebp
	pop	ebp
	ret	0
_getopt	ENDP
_TEXT	ENDS
; Function compile flags: /Ogtp
;	COMDAT _getopt_long
_TEXT	SEGMENT
_argument_name_length$1$ = -16				; size = 4
_argc$1$ = -12						; size = 4
_num_matches$1$ = -8					; size = 4
_argv$1$ = -4						; size = 4
_optstring$dead$ = 8					; size = 4
_longopts$ = 12						; size = 4
_longindex$ = 16					; size = 4
_getopt_long PROC					; COMDAT
; _argc$ = ecx
; _argv$ = edx
; File i:\study\filescan\filescan\getopt.c
; Line 165
	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
; Line 168
	xor	eax, eax
	mov	DWORD PTR _argv$1$[ebp], edx
	push	ebx
	mov	DWORD PTR _num_matches$1$[ebp], eax
	xor	ebx, ebx
; Line 173
	mov	DWORD PTR _optarg, eax
; Line 174
	mov	DWORD PTR _optopt, eax
; Line 176
	mov	eax, DWORD PTR _optind
	mov	DWORD PTR _argc$1$[ebp], ecx
	push	esi
	mov	esi, DWORD PTR _longopts$[ebp]
	cmp	eax, ecx
	jl	SHORT $LN19@getopt_lon
; Line 177
	pop	esi
	or	eax, -1
	pop	ebx
; Line 239
	mov	esp, ebp
	pop	ebp
	ret	0
$LN19@getopt_lon:
; Line 179
	mov	edx, DWORD PTR [edx+eax*4]
	mov	ecx, edx
	push	edi
	lea	edi, DWORD PTR [ecx+2]
	npad	3
$LL34@getopt_lon:
	mov	ax, WORD PTR [ecx]
	add	ecx, 2
	test	ax, ax
	jne	SHORT $LL34@getopt_lon
	sub	ecx, edi
	sar	ecx, 1
	cmp	ecx, 3
	jb	$LN17@getopt_lon
	push	2
	push	OFFSET ??_C@_15KHFHDPFH@?$AA?9?$AA?9?$AA?$AA@
	push	edx
	call	DWORD PTR __imp__wcsncmp
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	$LN17@getopt_lon
; Line 183
	mov	eax, DWORD PTR _optind
	mov	edi, DWORD PTR _argv$1$[ebp]
; Line 184
	push	OFFSET ??_C@_13NMPKAAJP@?$AA?$DN?$AA?$AA@
	mov	edi, DWORD PTR [edi+eax*4]
	add	edi, 4
	push	edi
	call	DWORD PTR __imp__wcscspn
; Line 185
	mov	ecx, esi
	mov	DWORD PTR _argument_name_length$1$[ebp], eax
	add	esp, 8
	cmp	DWORD PTR [ecx], ebx
	je	$LN11@getopt_lon
	npad	5
$LL16@getopt_lon:
; Line 186
	push	eax
	push	edi
	push	DWORD PTR [esi]
	call	DWORD PTR __imp__wcsncmp
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $LN15@getopt_lon
; Line 188
	inc	DWORD PTR _num_matches$1$[ebp]
	mov	ebx, esi
; Line 190
	mov	eax, DWORD PTR [esi]
	mov	ecx, edi
	npad	6
$LL35@getopt_lon:
	mov	dx, WORD PTR [eax]
	cmp	dx, WORD PTR [ecx]
	jne	SHORT $LN36@getopt_lon
	test	dx, dx
	je	SHORT $LN37@getopt_lon
	mov	dx, WORD PTR [eax+2]
	cmp	dx, WORD PTR [ecx+2]
	jne	SHORT $LN36@getopt_lon
	add	eax, 4
	add	ecx, 4
	test	dx, dx
	jne	SHORT $LL35@getopt_lon
$LN37@getopt_lon:
	xor	eax, eax
	jmp	SHORT $LN38@getopt_lon
$LN36@getopt_lon:
	sbb	eax, eax
	or	eax, 1
$LN38@getopt_lon:
	test	eax, eax
	je	SHORT $LN29@getopt_lon
$LN15@getopt_lon:
; Line 185
	mov	eax, DWORD PTR _argument_name_length$1$[ebp]
	add	esi, 16					; 00000010H
	cmp	DWORD PTR [esi], 0
	jne	SHORT $LL16@getopt_lon
$LN29@getopt_lon:
; Line 195
	cmp	DWORD PTR _num_matches$1$[ebp], 1
	jne	$LN11@getopt_lon
; Line 198
	mov	ecx, DWORD PTR _longindex$[ebp]
	test	ecx, ecx
	je	SHORT $LN10@getopt_lon
; Line 199
	mov	eax, ebx
	sub	eax, DWORD PTR _longopts$[ebp]
	sar	eax, 4
	mov	DWORD PTR [ecx], eax
$LN10@getopt_lon:
; Line 205
	mov	ecx, DWORD PTR [ebx+8]
	test	ecx, ecx
	je	SHORT $LN9@getopt_lon
; Line 206
	mov	eax, DWORD PTR [ebx+12]
	mov	DWORD PTR [ecx], eax
$LN9@getopt_lon:
; Line 208
	cmp	DWORD PTR [ebx+8], 0
	je	SHORT $LN22@getopt_lon
	xor	esi, esi
	jmp	SHORT $LN23@getopt_lon
$LN22@getopt_lon:
	mov	esi, DWORD PTR [ebx+12]
$LN23@getopt_lon:
; Line 210
	cmp	DWORD PTR [ebx+4], 0
; Line 211
	mov	eax, DWORD PTR _optind
; File c:\program files (x86)\microsoft visual studio 12.0\vc\include\string.h
; Line 383
	push	61					; 0000003dH
; File i:\study\filescan\filescan\getopt.c
; Line 210
	je	SHORT $LN8@getopt_lon
; File c:\program files (x86)\microsoft visual studio 12.0\vc\include\string.h
; Line 383
	mov	edi, DWORD PTR _argv$1$[ebp]
	push	DWORD PTR [edi+eax*4]
	call	DWORD PTR __imp__wcschr
	add	esp, 8
; File i:\study\filescan\filescan\getopt.c
; Line 211
	mov	DWORD PTR _optarg, eax
; Line 212
	test	eax, eax
	je	SHORT $LN7@getopt_lon
; Line 213
	add	eax, 2
	mov	DWORD PTR _optarg, eax
$LN7@getopt_lon:
; Line 215
	cmp	DWORD PTR [ebx+4], 1
	jne	SHORT $LN39@getopt_lon
; Line 218
	test	eax, eax
	jne	SHORT $LN39@getopt_lon
	mov	ecx, DWORD PTR _optind
	inc	ecx
	cmp	ecx, DWORD PTR _argc$1$[ebp]
	jge	SHORT $LN33@getopt_lon
; Line 219
	mov	eax, DWORD PTR [edi+ecx*4]
	mov	DWORD PTR _optarg, eax
; Line 222
	test	eax, eax
	jne	SHORT $LN1@getopt_lon
$LN33@getopt_lon:
; Line 223
	mov	esi, 58					; 0000003aH
; Line 237
	inc	ecx
	pop	edi
; Line 238
	mov	eax, esi
	mov	DWORD PTR _optind, ecx
	pop	esi
	pop	ebx
; Line 239
	mov	esp, ebp
	pop	ebp
	ret	0
$LN8@getopt_lon:
; File c:\program files (x86)\microsoft visual studio 12.0\vc\include\string.h
; Line 383
	mov	ecx, DWORD PTR _argv$1$[ebp]
	push	DWORD PTR [ecx+eax*4]
	call	DWORD PTR __imp__wcschr
	add	esp, 8
; File i:\study\filescan\filescan\getopt.c
; Line 225
	test	eax, eax
	je	SHORT $LN39@getopt_lon
$LN11@getopt_lon:
; Line 234
	mov	esi, 63					; 0000003fH
$LN39@getopt_lon:
	mov	ecx, DWORD PTR _optind
$LN1@getopt_lon:
	pop	edi
; Line 238
	mov	eax, esi
	inc	ecx
	pop	esi
	mov	DWORD PTR _optind, ecx
	pop	ebx
; Line 239
	mov	esp, ebp
	pop	ebp
	ret	0
$LN17@getopt_lon:
; Line 180
	mov	edx, DWORD PTR _argv$1$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _argc$1$[ebp]
	call	_getopt
	add	esp, 4
	pop	edi
	pop	esi
	pop	ebx
; Line 239
	mov	esp, ebp
	pop	ebp
	ret	0
_getopt_long ENDP
_TEXT	ENDS
END
