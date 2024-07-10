parser grammar StreamDeclarationParser;

options {
  tokenVocab = StreamDeclarationLexer;
}

parse
 : (core_stmt | error )* EOF
 ;

error
 : UNEXPECTED_CHAR
   {
     throw new RuntimeException("UNEXPECTED_CHAR=" + $UNEXPECTED_CHAR.text);
   }
 ;

core_stmt
 : core_declaration
 ;

core_declaration
 : stream_declaration
 ;

stream_declaration
 : K_DECLARE K_STREAM stream_name '{' event_list '}'
 ;

event_list
 : event_declaration ( ',' event_declaration )*
 ;

event_declaration
 : K_EVENT event_name '{' attribute_dec_list '}'
 ;

attribute_dec_list
 :
 | attribute_declaration ( ',' attribute_declaration )*
 ;

attribute_declaration
 : attribute_name ':' datatype
 ;

datatype
 : 'int'
 | 'string'
 | 'double'
 | 'boolean'
 | PRIMARY_TIME
 | IDENTIFIER
 ;


event_name
 : any_name
 ;

stream_name
 : any_name
 ;

attribute_name
 : any_name
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
 | K_DECLARE
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
 | K_SECONDS
 | K_SELECT
 | K_STREAM
 | K_STRICT
 | K_UNLESS
 | K_WHERE
 | K_WITHIN
 ;
