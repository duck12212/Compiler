此程式為單一輸入,一次只能輸入一次
為scanner+parser

文法為:
Regular  Expressions   
digit  →  [0-9] 
digits  →  digit+ 
number  →  digits(.digits)?(E[+-]?digits)? 
letter  →  [A-Z] 
id  →  letter(letter|digit)* 
declare  →  declare 
if→  If 
=  →  = 
+  →  + 
-→-   
*  →  * 
/  →  / 
;  →  ; 
)  →) 
(→( 
&rt;→&rt; 

Grammar   
Start  →Stmts  $ 
Stmts  →AssignStmts  |  ifStmt 
AssignStmts  →  Stmt  AssignStmts  |  Stmt 
ifStmt  →  if  (id&rt;digit)  {  AssignStmts  } 
Stmt  →  declare  id  =  expression; 
expression  →  expression  +  term  |  expression  -  term  |  term 
term  →  term*factor  |  term/factor  |  factor 
factor  →  (expression)  |  id  |  number
