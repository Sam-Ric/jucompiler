/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LPAR = 258,                    /* LPAR  */
    RPAR = 259,                    /* RPAR  */
    LBRACE = 260,                  /* LBRACE  */
    RBRACE = 261,                  /* RBRACE  */
    LSQ = 262,                     /* LSQ  */
    RSQ = 263,                     /* RSQ  */
    EQ = 264,                      /* EQ  */
    ASSIGN = 265,                  /* ASSIGN  */
    GE = 266,                      /* GE  */
    GT = 267,                      /* GT  */
    LE = 268,                      /* LE  */
    LT = 269,                      /* LT  */
    NE = 270,                      /* NE  */
    NOT = 271,                     /* NOT  */
    COMMA = 272,                   /* COMMA  */
    SEMICOLON = 273,               /* SEMICOLON  */
    STAR = 274,                    /* STAR  */
    DIV = 275,                     /* DIV  */
    PLUS = 276,                    /* PLUS  */
    MINUS = 277,                   /* MINUS  */
    MOD = 278,                     /* MOD  */
    AND = 279,                     /* AND  */
    OR = 280,                      /* OR  */
    ARROW = 281,                   /* ARROW  */
    LSHIFT = 282,                  /* LSHIFT  */
    RSHIFT = 283,                  /* RSHIFT  */
    XOR = 284,                     /* XOR  */
    INT = 285,                     /* INT  */
    DOUBLE = 286,                  /* DOUBLE  */
    STRING = 287,                  /* STRING  */
    BOOL = 288,                    /* BOOL  */
    IF = 289,                      /* IF  */
    ELSE = 290,                    /* ELSE  */
    CLASS = 291,                   /* CLASS  */
    PUBLIC = 292,                  /* PUBLIC  */
    RETURN = 293,                  /* RETURN  */
    STATIC = 294,                  /* STATIC  */
    VOID = 295,                    /* VOID  */
    WHILE = 296,                   /* WHILE  */
    RESERVED = 297,                /* RESERVED  */
    DOTLENGTH = 298,               /* DOTLENGTH  */
    PRINT = 299,                   /* PRINT  */
    PARSEINT = 300,                /* PARSEINT  */
    NATURAL = 301,                 /* NATURAL  */
    DECIMAL = 302,                 /* DECIMAL  */
    IDENTIFIER = 303,              /* IDENTIFIER  */
    STRLIT = 304,                  /* STRLIT  */
    BOOLLIT = 305,                 /* BOOLLIT  */
    LOWER_THAN_ELSE = 306,         /* LOWER_THAN_ELSE  */
    UNARY = 307                    /* UNARY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define LPAR 258
#define RPAR 259
#define LBRACE 260
#define RBRACE 261
#define LSQ 262
#define RSQ 263
#define EQ 264
#define ASSIGN 265
#define GE 266
#define GT 267
#define LE 268
#define LT 269
#define NE 270
#define NOT 271
#define COMMA 272
#define SEMICOLON 273
#define STAR 274
#define DIV 275
#define PLUS 276
#define MINUS 277
#define MOD 278
#define AND 279
#define OR 280
#define ARROW 281
#define LSHIFT 282
#define RSHIFT 283
#define XOR 284
#define INT 285
#define DOUBLE 286
#define STRING 287
#define BOOL 288
#define IF 289
#define ELSE 290
#define CLASS 291
#define PUBLIC 292
#define RETURN 293
#define STATIC 294
#define VOID 295
#define WHILE 296
#define RESERVED 297
#define DOTLENGTH 298
#define PRINT 299
#define PARSEINT 300
#define NATURAL 301
#define DECIMAL 302
#define IDENTIFIER 303
#define STRLIT 304
#define BOOLLIT 305
#define LOWER_THAN_ELSE 306
#define UNARY 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "jucompiler.y"

  char *val;              // tokens with string values
  struct node *node;      // AST nodes
  struct node_list *list; // list of nodes

#line 177 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
