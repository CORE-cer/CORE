parser grammar OptionDeclarationParser;

options {
  tokenVocab = OptionDeclarationLexer;
}

parse
 : (option_declaration | error )* EOF
 ;

error
 : UNEXPECTED_CHAR
   {
     throw new RuntimeException("UNEXPECTED_CHAR=" + $UNEXPECTED_CHAR.text);
   }
 ;


option_declaration
 : K_DECLARE K_QUARANTINE LEFT_CURLY_BRACKET quarantine_policy* RIGHT_CURLY_BRACKET
 ;


quarantine_policy
 : K_FIXED_TIME time_span LEFT_CURLY_BRACKET stream_names RIGHT_CURLY_BRACKET # fixed_time_policy
 | K_DIRECT LEFT_CURLY_BRACKET stream_names RIGHT_CURLY_BRACKET # direct_policy
 ;

 stream_names
 : stream_name ( COMMA stream_name )*
 ;

time_span
 : hour_span? minute_span? second_span?
 ;

hour_span
 : integer K_HOURS
 ;

minute_span
 : integer K_MINUTES
 ;

second_span
 : integer K_SECONDS
 ;

stream_name
 : any_name
 ;

any_name
 : IDENTIFIER
 ;

number
 : integer
 | double
 ;

integer
 : INTEGER_LITERAL
 ;

double
 : DOUBLE_LITERAL
 ;
