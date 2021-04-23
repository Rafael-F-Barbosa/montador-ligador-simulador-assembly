;Exemplo 1 com erros de segunda passagem


SECTION DATA


B: SPACE            ;comentario
H: SPACE 2
R: SPACE
DOIS  : CONST 2
tres  : CONST
quatro     : CONST 4 4
;r: Const 3


SECTION TEXT        ; comentario w djfksfk

    INPUT B
    INPUT HP

LOAD B
LOAD H B
COPY B, H

DIV DOIS

STORE R
OUTPUT R
STOP r
