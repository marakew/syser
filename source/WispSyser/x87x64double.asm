
.code

REAL10toDOUBLE	PROC, ipt:PTR QWORD, opt:PTR REAL8
;	mov rax, ipt
;	fld TBYTE PTR [rax]
	fld TBYTE PTR [rcx]
;	mov rax, opt
;	fstp REAL8 PTR [rax]
	fstp REAL8 PTR [rdx]
	ret
REAL10toDOUBLE	ENDP
END