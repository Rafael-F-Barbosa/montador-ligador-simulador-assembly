SECTION DATA 
DOIS: CONST 2.0   ; Argumento do tipo incorreto
DOIX: CONST       ; Const não recebendo argumetos
TRES: CONST 3 3   ; CONST recebendo 2 argumentos
OLD_DATA: SPACE 1 ; Space não recebe argumentos
NEW_DATA: SPACE    
TMP_DATA: SPACE    



SECTION TEXT
INPUT 1OLD_DATA,i  ; Input recebendo 2 argumetos   
LOAD              ; Load recebe pelo menos 1 argumento
L1 :                
DIV DOIS            
STORE  NEW_DATA1 ; Argumento que não foi declarado    
MUL DOIS           
STORE TMP_DATA     
LOAD OLD_DATA      
SUB TMP_DATa       
STORE TMP_DATA     
OUTPUT TMP_DATA    
COPY 1NEW_DATA,OLD_DATA; Quantidade de argumentos incorreta
COPY NEW_DATA ; Copy recebendo só um argumento 
LOAD OLD_DATA      
JMPP L1            
STOP               