; Módulo principal - Recebe um número e computa o fatorial
MOD_A: BEGIN
MOD_B: EXTERN
	PUBLIC FAT
	PUBLIC N
SECTION DATA
	N: SPACE
	ONE: CONST 1
SECTION TEXT
	INPUT N
	LOAD N
	FAT: SUB ONE
	JMPZ FIM
	JMP MOD_B
	FIM: OUTPUT N
	STOP

END