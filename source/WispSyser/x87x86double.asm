
.386
.model flat, C

.code

REAL10toDOUBLE	PROC, ipt:PTR QWORD, opt:PTR REAL8
	mov eax, ipt
	fld TBYTE PTR [eax]
	mov eax, opt
	fstp REAL8 PTR [eax]
	ret
REAL10toDOUBLE	ENDP
END