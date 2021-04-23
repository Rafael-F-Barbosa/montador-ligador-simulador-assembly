SECTION DATA
1DOIS: CONST 2  ;Label inicia com número
OLD_DATAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa: SPACE ; Label muito grande
NEW_ DATA : SPACE  ; Label com espaço no meio  
TMP_&DATA: SPACE  ; Label com símbolo doido no meio
TMP_,ANY_DATA: SPACE ; Label com vírgula no meio
L1 : CONST 3 ; Label repetida
JMP: SPACE ; Label com nome de instrução
SPACE: SPACE ; Label com nome de diretiva



SECTION TEXT
INPUT1 OLD_DATA ; Instrução ou diretiva inexistente      
LOAD OLD_DATA      
L1:          
DIV DOIS            
L2: STORi  NEW_DATA ; Instrução ou diretiva inexistente 
MUL DOIS           
STORE TMP_DATA     
LOAD OLD_DATA      
SUB TMP_DATa       
STORE TMP_DATA     
OUTPUT TMP_DATA    
COPY NEW_DATA,OLD_DATA
LOAD OLD_DATA      
JMPP L1            
STOP               