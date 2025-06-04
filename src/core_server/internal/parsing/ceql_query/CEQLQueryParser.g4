parser grammar CEQLQueryParser;

options {
  tokenVocab = CEQLQueryLexer;
}

parse
 : (core_query | error )* EOF
 ;

error
 : UNEXPECTED_CHAR
   {
     throw new RuntimeException("UNEXPECTED_CHAR=" + $UNEXPECTED_CHAR.text);
   }
 ;

core_query
 : K_SELECT selection_strategy? list_of_variables
   from_clause
   K_WHERE cel_formula
   ( K_PARTITION K_BY partition_list )?
   ( K_WITHIN time_window )?
   ( K_CONSUME K_BY consumption_policy )?
   ( K_LIMIT limit )?
 ;

selection_strategy
 : K_ALL                        # ss_all
 | K_ANY                        # ss_any
 | K_LAST                       # ss_last
 | K_MAX                        # ss_max
 | K_NEXT                       # ss_next
 | K_STRICT                     # ss_strict
 ;

list_of_variables
 : STAR                         # s_star
 | K_NONE                         # s_none
 | s_event_name_with_projection ( COMMA s_event_name_with_projection )* # s_event_name_with_projection_list
 ;

from_clause
 : ( K_FROM stream_name ( COMMA stream_name )* )?
 ;


cel_formula
 : LEFT_PARENTHESIS cel_formula RIGHT_PARENTHESIS     # par_cel_formula
 | s_event_name                                       # event_type_cel_formula
 | (K_NOT | REGEX_EXCLAMAITON) LEFT_PARENTHESIS atomic_cel_formula RIGHT_PARENTHESIS   # not_event_type_atomic_cel_formula
 | cel_formula K_AS event_name                        # as_cel_formula
 | cel_formula PLUS                                   # non_contiguous_iteration_cel_formula
 | cel_formula COLON_PLUS                             # contiguous_iteration_cel_formula
 | cel_formula SEMICOLON cel_formula                  # non_contiguous_sequencing_cel_formula
 | cel_formula COLON cel_formula                      # contiguous_sequencing_cel_formula
 | cel_formula K_OR cel_formula                       # or_cel_formula
 | cel_formula K_FILTER filter                        # filter_cel_formula
 ;

partition_list
 : LEFT_SQUARE_BRACKET attribute_list RIGHT_SQUARE_BRACKET (COMMA LEFT_SQUARE_BRACKET attribute_list RIGHT_SQUARE_BRACKET)?
 ;

attribute_list
 :  attribute_name ( COMMA attribute_name )*
 ;

consumption_policy
 : K_ANY                                                # cp_any
 | K_PARTITION                                          # cp_partition
 | K_NONE                                               # cp_none
 ;

 limit
 : integer                                              # l_integer
 ;

filter
 : LEFT_PARENTHESIS filter RIGHT_PARENTHESIS                                 # par_filter
 | s_event_name LEFT_SQUARE_BRACKET predicate RIGHT_SQUARE_BRACKET             # atomic_filter
 | filter K_AND filter                                                       # and_filter
 | filter K_OR filter                                                        # or_filter
 ;

predicate
 : LEFT_PARENTHESIS predicate RIGHT_PARENTHESIS                                             # par_predicate
 | K_NOT predicate                                                                          # not_predicate
 | math_expr ( LE | LEQ | GE | GEQ | EQ | NEQ) math_expr                                    # inequality_predicate
 | string_literal ( EQ | NEQ ) string_literal_or_regexp                                     # equality_string_predicate
 | predicate K_AND predicate                                                                # and_predicate
 | predicate K_OR predicate                                                                 # or_predicate
 | attribute_name K_LIKE regexp                                                             # regex_predicate
 | attribute_name ( K_IN | K_NOT K_IN ) value_seq                                           # in_predicate
 | math_expr K_IN K_RANGE LEFT_PARENTHESIS math_expr COMMA math_expr RIGHT_PARENTHESIS      # in_range_predicate
 ;

string_literal
 : string
 | attribute_name
 ;

string_literal_or_regexp
 : string
 | attribute_name
 | regexp
 ;

math_expr
 : LEFT_PARENTHESIS math_expr RIGHT_PARENTHESIS             # par_math_expr
 | number                                                   # number_math_expr
 | attribute_name                                           # attribute_math_expr
 | ( PLUS | MINUS ) math_expr                               # unary_math_expr
 | math_expr ( STAR | SLASH | PERCENT ) math_expr           # mul_math_expr
 | math_expr ( PLUS | MINUS ) math_expr                     # sum_math_expr
 ;

value_seq
 : LEFT_CURLY_BRACKET number_seq RIGHT_CURLY_BRACKET
 | LEFT_CURLY_BRACKET string_seq RIGHT_CURLY_BRACKET
 ;

number_seq
 : number (COMMA number)*                # number_list
 | integer DOUBLE_DOT integer            # integer_range
 | double DOUBLE_DOT double              # double_range
 | number DOUBLE_DOT                     # number_range_lower
 | DOUBLE_DOT number                     # number_range_upper
 ;

string_seq
 : string (COMMA string)*
 ;

time_window
 : event_span
 | time_span
 | custom_span
 ;

event_span
 : integer K_EVENTS
 ;

time_span
 : hour_span? minute_span? second_span?
 ;

hour_span
 : number K_HOURS
 ;

minute_span
 : number K_MINUTES
 ;

second_span
 : number K_SECONDS
 ;

custom_span
 : integer LEFT_SQUARE_BRACKET any_name RIGHT_SQUARE_BRACKET
 ;

named_event
 : s_event_name ( K_AS event_name )?
 ;

s_event_name_with_projection
 : s_event_name (LEFT_SQUARE_BRACKET list_of_attribute_names RIGHT_SQUARE_BRACKET)?
 ;

s_event_name
 : ( stream_name '>' ) ? event_name
 ;

event_name
 : any_name
 ;

atomic_cel_formula
 : s_event_name                                   # atomic_cel_formula_r
 | atomic_cel_formula K_FILTER filter             # atomic_cel_formula_filter
 ;

stream_name
 : any_name
 ;

list_of_attribute_names
 : attribute_name ( COMMA attribute_name )*
 ;

attribute_name
 : any_name
 ;

integer
 : INTEGER_LITERAL
 ;

double
 : DOUBLE_LITERAL
 ;

number
 : integer
 | double
 ;

string
 : STRING_LITERAL
 ;

any_name
 : IDENTIFIER
 ;

keyword
 : K_ALL
 | K_AND
 | K_ANY
 | K_AS
 | K_BY
 | K_CONSUME
 | K_DISTINCT
 | K_EVENT
 | K_EVENTS
 | K_FILTER
 | K_FROM
 | K_HOURS
 | K_IN
 | K_LAST
 | K_LIKE
 | K_MAX
 | K_MINUTES
 | K_NEXT
 | K_NONE
 | K_NOT
 | K_OR
 | K_PARTITION
 | K_RANGE
 | K_SECONDS
 | K_SELECT
 | K_STREAM
 | K_STRICT
 | K_UNLESS
 | K_WHERE
 | K_WITHIN
 ;

// regexp parser
regexp
 : '<<' regexp_alternation '>>'
 ;
 
regexp_alternation
 : regexp_exp ( '|' regexp_exp )*
 ;
 
regexp_exp
 : regexp_element+
 ;
 
regexp_element
 : regexp_group quantifier?
 ;
 
regexp_group
 : parenthesis
 | atom
 ;
 
parenthesis
 : REGEX_L_PAR regexp_alternation REGEX_R_PAR
 ;
 
quantifier: REGEX_QUESTION | REGEX_PLUS | REGEX_STAR | REGEX_L_CURLY quantity REGEX_R_CURLY;
quantity: quantExact | quantRange | quantMin | quantMax;
quantExact: regexp_number;
quantRange: regexp_number REGEX_COMMA regexp_number;
quantMin: regexp_number REGEX_COMMA;
quantMax: REGEX_COMMA regexp_number;
 
atom: characterClass | sharedAtom | literal;
 
characterClass: REGEX_L_BRACK '^'? ccAtom+ REGEX_R_BRACK;
 
ccAtom
 : ccRange
 | sharedAtom
 | ccSingle;
 
ccRange: ccLiteral REGEX_HYPHEN ccLiteral;
 
ccSingle: ccLiteral;
 
ccLiteral
 : ccEscapes
 | ccOther;
 
ccEscapes: '\\' (REGEX_HAT | REGEX_HYPHEN | REGEX_R_BRACK | '\\');
 
ccOther: ~(REGEX_HAT | REGEX_HYPHEN | REGEX_R_BRACK | '\\');
 
literal: escapes | REGEX_DOT | other;
 
escapes:
  '\\' (
    REGEX_L_BRACK
    | REGEX_R_BRACK
    | REGEX_L_PAR
    | REGEX_R_PAR
    | REGEX_L_CURLY
    | REGEX_R_CURLY
    | REGEX_STAR
    | REGEX_PLUS
    | REGEX_QUESTION
    | '|'
    | '.'
    | '\\'
  );
 
 other:
  ~(
    REGEX_L_BRACK
    | REGEX_R_BRACK
    | REGEX_L_PAR
    | REGEX_R_PAR
    | REGEX_L_CURLY
    | REGEX_R_CURLY
    | REGEX_STAR
    | REGEX_PLUS
    | REGEX_QUESTION
    | '|'
    | '\\'
  );
 
sharedAtom
 : REGEX_DECIMAL_DIGIT
 | REGEX_NOT_DECIMAL_DIGIT
 | REGEX_WHITESPACE
 | REGEX_NOT_WHITESPACE
 | REGEX_ALPHANUMERIC
 | REGEX_NOT_ALPHANUMERIC
 ;
 
regexp_number
 : REGEX_DIGIT+
 ; 