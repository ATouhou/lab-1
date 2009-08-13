/*
 *  lexer.h
 *
 *  Created by Cameron Palmer on 10/16/07.
 *  Copyright 2007 University of North Texas. All rights reserved.
 *
 */
 
#ifndef LEXER_H_
#define LEXER_H_

#define BSIZE 256

typedef enum {	EOT, ERROR, COMMENT,
				COMMA, QUESTION, EXCLAMATION, PERIOD,
				EQUAL, LTHAN, GTHAN, 
				PLUS, MINUS, ASTERIX, PERCENT, CARAT, FSLASH,
				PIPE, AMP, COLON, SEMI, TILDE,
				LBRACKET, RBRACKET, LCURLY, RCURLY, LPAREN, RPAREN,
				 
				ID, CONSTANT, STRING_LITERAL, SIZEOF,
				PTR_OP, INC_OP, DEC_OP, LEFT_OP, RIGHT_OP, LE_OP, GE_OP, EQ_OP, NE_OP,
				AND_OP, OR_OP, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, ADD_ASSIGN,
				SUB_ASSIGN, LEFT_ASSIGN, RIGHT_ASSIGN, AND_ASSIGN,
				XOR_ASSIGN, OR_ASSIGN, TYPE_NAME,
				
				TYPEDEF, EXTERN, STATIC, AUTO, REGISTER,
				CHAR, SHORT, INT, LONG, SIGNED, UNSIGNED, FLOAT, DOUBLE, CONST, VOLATILE, VOID,
				STRUCT, UNION, ENUM, ELLIPSIS,
				
				CASE, DEFAULT, IF, ELSE, SWITCH, WHILE, DO, FOR, GOTO, CONTINUE, BREAK, RETURN } T;
extern long int col;
extern long int line;
extern char *yystring;

T yylex(void);

#endif /*LEXER_H_*/