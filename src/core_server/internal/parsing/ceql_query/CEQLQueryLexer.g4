lexer grammar CEQLQueryLexer;

K_ALL       : A L L;
K_AND       : A N D;
K_ANY       : A N Y;
K_AS        : A S;
K_BY        : B Y;
K_CONSUME   : C O N S U M E;
K_LIMIT   : L I M I T;
K_DISTINCT  : D I S T I N C T;
K_EVENT     : E V E N T;
K_EVENTS    : E V E N T S;
K_FILTER    : F I L T E R;
K_FROM      : F R O M;
K_HOURS     : H O U R S?;
K_IN        : I N;
K_LAST      : L A S T;
K_LIKE      : L I K E;
K_MAX       : M A X;
K_MINUTES   : M I N U T E S?;
K_NEXT      : N E X T;
K_NONE      : N O N E;
K_NOT       : N O T;
K_OR        : O R;
K_PARTITION : P A R T I T I O N;
K_RANGE     : R A N G E;
K_SECONDS   : S E C O N D S?;
K_SELECT    : S E L E C T;
K_STREAM    : S T R E A M;
K_STRICT    : S T R I C T;
K_UNLESS    : U N L E S S;
K_WHERE     : W H E R E;
K_WITHIN    : W I T H I N;

PERCENT : '%';
PLUS    : '+';
MINUS   : '-';
STAR    : '*';
SLASH   : '/';

LE  : '<'  ;
LEQ : '<=' ;
GE  : '>'  ;
GEQ : '>=' ;
EQ  : '==' | '='  ;
NEQ : '!=' | '<>' ;

SEMICOLON : ';' ;
COLON : ':' ;
COMMA : ',' ;
DOUBLE_DOT : '..';
LEFT_PARENTHESIS : '(' ;
RIGHT_PARENTHESIS : ')' ;
LEFT_SQUARE_BRACKET : '[' ;
RIGHT_SQUARE_BRACKET : ']' ;
LEFT_CURLY_BRACKET : '{' ;
RIGHT_CURLY_BRACKET : '}' ;
COLON_PLUS : ':+' ;

IDENTIFIER
 :  '`' (~'`' | '``')* '`'
 | [a-zA-Z_] [a-zA-Z_0-9]*
 ;

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

STRING_LITERAL
 : '\'' ( ~'\'' | '\'\'' )* '\''
 ;

SINGLE_LINE_COMMENT
 : '--' ~[\r\n]* -> channel(HIDDEN)
 ;

MULTILINE_COMMENT
 : '/*' .*? ( '*/' | EOF ) -> channel(HIDDEN)
 ;

SPACES
 : [ \u000B\t\r\n] -> channel(HIDDEN)
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
