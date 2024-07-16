lexer grammar OptionDeclarationLexer;

K_DECLARE   : D E C L A R E;
K_QUARANTINE   : Q U A R A N T I N E;
K_FIXED_TIME : F I X E D '_' T I M E;
K_DIRECT : D I R E C T;
K_HOURS   : H O U R S?;
K_MINUTES   : M I N U T E S?;
K_SECONDS   : S E C O N D S?;

LEFT_CURLY_BRACKET : '{' ;
RIGHT_CURLY_BRACKET : '}' ;
COMMA : ',' ;

DOUBLE_LITERAL
 : INTEGER_LITERAL '.' NUMERICAL_EXPONENT
 | INTEGER_LITERAL? '.' DIGIT+
 | INTEGER_LITERAL? '.' DIGIT+ NUMERICAL_EXPONENT
 ;

INTEGER_LITERAL
 : DIGIT+
 ;

NUMERICAL_EXPONENT
 : E '-'? DIGIT+
 ;

IDENTIFIER
 :  '`' (~'`' | '``')* '`'
 | [a-zA-Z_] [a-zA-Z_0-9]*
 ;

UNEXPECTED_CHAR
 : .
 ;

fragment DIGIT : [0-9];

fragment A : [aA];
fragment B : [bB];
fragment C : [cC];
fragment D : [dD];
fragment E : [eE];
fragment F : [fF];
fragment G : [gG];
fragment H : [hH];
fragment I : [iI];
fragment J : [jJ];
fragment K : [kK];
fragment L : [lL];
fragment M : [mM];
fragment N : [nN];
fragment O : [oO];
fragment P : [pP];
fragment Q : [qQ];
fragment R : [rR];
fragment S : [sS];
fragment T : [tT];
fragment U : [uU];
fragment V : [vV];
fragment W : [wW];
fragment X : [xX];
fragment Y : [yY];
fragment Z : [zZ];
