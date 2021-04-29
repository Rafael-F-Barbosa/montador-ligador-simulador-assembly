;Código fonte do
;módulo B:
MOD_B: BEGIN
VAL: EXTERN
L1: EXTERN
PUBLIC Y
PUBLIC MOD_B
PUBLIC X ; Caso eu não tivesse declarado x, ele não acusou erro - arrumar isso
OUTPUT Y
OUTPUT VAL
OUTPUT X
JMP L1
Y: SPACE
X: SPACE
END
