/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "jucompiler.y"

  #include <stdio.h>
  #include "ast.h"

  extern int yylex(void);;
  void yyerror(char *);
  extern char *yytext;
  extern char last_token_text[];
  extern int line_count, col_count, string_col;

  struct node *root = NULL; // AST root
  int syntax_errors = 0;    // error counter

#line 85 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 19 "jucompiler.y"

  char *val;              // tokens with string values
  struct node *node;      // AST nodes
  struct node_list *list; // list of nodes

#line 248 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_LPAR = 3,                       /* LPAR  */
  YYSYMBOL_RPAR = 4,                       /* RPAR  */
  YYSYMBOL_LBRACE = 5,                     /* LBRACE  */
  YYSYMBOL_RBRACE = 6,                     /* RBRACE  */
  YYSYMBOL_LSQ = 7,                        /* LSQ  */
  YYSYMBOL_RSQ = 8,                        /* RSQ  */
  YYSYMBOL_EQ = 9,                         /* EQ  */
  YYSYMBOL_ASSIGN = 10,                    /* ASSIGN  */
  YYSYMBOL_GE = 11,                        /* GE  */
  YYSYMBOL_GT = 12,                        /* GT  */
  YYSYMBOL_LE = 13,                        /* LE  */
  YYSYMBOL_LT = 14,                        /* LT  */
  YYSYMBOL_NE = 15,                        /* NE  */
  YYSYMBOL_NOT = 16,                       /* NOT  */
  YYSYMBOL_COMMA = 17,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 18,                 /* SEMICOLON  */
  YYSYMBOL_STAR = 19,                      /* STAR  */
  YYSYMBOL_DIV = 20,                       /* DIV  */
  YYSYMBOL_PLUS = 21,                      /* PLUS  */
  YYSYMBOL_MINUS = 22,                     /* MINUS  */
  YYSYMBOL_MOD = 23,                       /* MOD  */
  YYSYMBOL_AND = 24,                       /* AND  */
  YYSYMBOL_OR = 25,                        /* OR  */
  YYSYMBOL_ARROW = 26,                     /* ARROW  */
  YYSYMBOL_LSHIFT = 27,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 28,                    /* RSHIFT  */
  YYSYMBOL_XOR = 29,                       /* XOR  */
  YYSYMBOL_INT = 30,                       /* INT  */
  YYSYMBOL_DOUBLE = 31,                    /* DOUBLE  */
  YYSYMBOL_STRING = 32,                    /* STRING  */
  YYSYMBOL_BOOL = 33,                      /* BOOL  */
  YYSYMBOL_IF = 34,                        /* IF  */
  YYSYMBOL_ELSE = 35,                      /* ELSE  */
  YYSYMBOL_CLASS = 36,                     /* CLASS  */
  YYSYMBOL_PUBLIC = 37,                    /* PUBLIC  */
  YYSYMBOL_RETURN = 38,                    /* RETURN  */
  YYSYMBOL_STATIC = 39,                    /* STATIC  */
  YYSYMBOL_VOID = 40,                      /* VOID  */
  YYSYMBOL_WHILE = 41,                     /* WHILE  */
  YYSYMBOL_RESERVED = 42,                  /* RESERVED  */
  YYSYMBOL_DOTLENGTH = 43,                 /* DOTLENGTH  */
  YYSYMBOL_PRINT = 44,                     /* PRINT  */
  YYSYMBOL_PARSEINT = 45,                  /* PARSEINT  */
  YYSYMBOL_NATURAL = 46,                   /* NATURAL  */
  YYSYMBOL_DECIMAL = 47,                   /* DECIMAL  */
  YYSYMBOL_IDENTIFIER = 48,                /* IDENTIFIER  */
  YYSYMBOL_STRLIT = 49,                    /* STRLIT  */
  YYSYMBOL_BOOLLIT = 50,                   /* BOOLLIT  */
  YYSYMBOL_LOWER_THAN_ELSE = 51,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_UNARY = 52,                     /* UNARY  */
  YYSYMBOL_YYACCEPT = 53,                  /* $accept  */
  YYSYMBOL_Program = 54,                   /* Program  */
  YYSYMBOL_MemberList = 55,                /* MemberList  */
  YYSYMBOL_MethodDecl = 56,                /* MethodDecl  */
  YYSYMBOL_FieldDecl = 57,                 /* FieldDecl  */
  YYSYMBOL_IdentifierList = 58,            /* IdentifierList  */
  YYSYMBOL_Type = 59,                      /* Type  */
  YYSYMBOL_MethodHeader = 60,              /* MethodHeader  */
  YYSYMBOL_FormalParams = 61,              /* FormalParams  */
  YYSYMBOL_ParamList = 62,                 /* ParamList  */
  YYSYMBOL_MethodBody = 63,                /* MethodBody  */
  YYSYMBOL_StatementOrVarDecl = 64,        /* StatementOrVarDecl  */
  YYSYMBOL_VarDecl = 65,                   /* VarDecl  */
  YYSYMBOL_Statement = 66,                 /* Statement  */
  YYSYMBOL_StatementList = 67,             /* StatementList  */
  YYSYMBOL_MethodInvocation = 68,          /* MethodInvocation  */
  YYSYMBOL_ArgList = 69,                   /* ArgList  */
  YYSYMBOL_Assignment = 70,                /* Assignment  */
  YYSYMBOL_ParseArgs = 71,                 /* ParseArgs  */
  YYSYMBOL_Expr = 72                       /* Expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   476

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  164

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   307


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   112,   112,   121,   122,   126,   134,   140,   148,   168,
     172,   173,   180,   181,   182,   186,   192,   200,   206,   217,
     218,   228,   235,   245,   252,   253,   259,   270,   293,   309,
     318,   330,   338,   341,   345,   346,   347,   348,   349,   353,
     357,   361,   362,   370,   374,   379,   386,   390,   397,   405,
     410,   416,   421,   426,   431,   436,   441,   446,   451,   456,
     461,   466,   471,   476,   481,   486,   491,   496,   500,   504,
     508,   509,   512,   513,   514,   515,   518,   522,   525,   528
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "LPAR", "RPAR",
  "LBRACE", "RBRACE", "LSQ", "RSQ", "EQ", "ASSIGN", "GE", "GT", "LE", "LT",
  "NE", "NOT", "COMMA", "SEMICOLON", "STAR", "DIV", "PLUS", "MINUS", "MOD",
  "AND", "OR", "ARROW", "LSHIFT", "RSHIFT", "XOR", "INT", "DOUBLE",
  "STRING", "BOOL", "IF", "ELSE", "CLASS", "PUBLIC", "RETURN", "STATIC",
  "VOID", "WHILE", "RESERVED", "DOTLENGTH", "PRINT", "PARSEINT", "NATURAL",
  "DECIMAL", "IDENTIFIER", "STRLIT", "BOOLLIT", "LOWER_THAN_ELSE", "UNARY",
  "$accept", "Program", "MemberList", "MethodDecl", "FieldDecl",
  "IdentifierList", "Type", "MethodHeader", "FormalParams", "ParamList",
  "MethodBody", "StatementOrVarDecl", "VarDecl", "Statement",
  "StatementList", "MethodInvocation", "ArgList", "Assignment",
  "ParseArgs", "Expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-59)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -32,   -36,    26,    55,   -59,   -59,    64,    -4,   -59,   -59,
      27,   -59,   -59,   -59,   -15,   -59,   -59,   -59,    21,    23,
      72,    78,    85,   -59,   -59,   134,   182,    66,   106,   -59,
      82,    45,    86,    77,   -59,   105,    62,   -59,    95,   -59,
     -59,   -59,   111,   127,   112,   113,   123,    35,    79,   -59,
     -59,   110,   117,   124,   121,   -59,   -59,    42,   -59,   -59,
     -59,   151,   181,    75,   181,   -59,   181,   181,   -59,   -59,
      14,   -59,   -59,   -59,   -59,   346,   181,   160,    10,    58,
     181,   -59,   -59,   -59,   -59,    83,    93,   -59,   -59,   237,
     142,   259,   -59,   -59,   -59,   -59,   181,   181,   181,   181,
     181,   181,   -59,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   281,   149,   303,   164,   148,   166,   -59,
       4,   367,   367,    68,   -59,   -59,   199,   -59,   -59,   448,
       9,     9,     9,     9,   448,   -59,   -59,    44,    44,   -59,
     409,   388,   139,   139,   430,   199,   153,   161,   -59,   181,
     -59,   -59,   181,   -59,   143,   -59,   -59,   -59,   325,   367,
     199,   176,   -59,   -59
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     3,     0,     0,     2,     6,
       0,     4,     5,     9,     0,    13,    14,    12,     0,     0,
       0,     0,    10,    24,     7,     0,     0,     0,     0,    17,
       0,     0,     0,    19,    15,     0,     0,     8,     0,    41,
      23,    34,     0,     0,     0,     0,     0,     0,     0,    26,
      25,     0,     0,     0,     0,    21,    18,     0,    16,    11,
      40,     0,     0,     0,     0,    32,     0,     0,    77,    78,
      75,    79,    72,    73,    74,     0,     0,     0,     0,     0,
       0,    10,    35,    36,    37,     0,     0,    28,    42,     0,
       0,     0,    69,    68,    67,    76,     0,     0,     0,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,    46,    48,     0,    20,    22,     0,    71,    70,    61,
      62,    63,    64,    65,    66,    53,    54,    51,    52,    55,
      56,    57,    59,    60,    58,     0,     0,     0,    50,     0,
      45,    44,     0,    27,    29,    31,    39,    38,     0,    47,
       0,     0,    30,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -59,   -59,   -59,   -59,   -59,   102,    -1,   -59,   162,   -59,
     -59,   -59,   -59,   -58,   -59,   -28,   -59,   -27,   -26,   -57
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     6,    11,    12,    27,    31,    20,    32,    33,
      24,    28,    49,    50,    61,    72,   120,    73,    74,    75
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      51,    52,    53,    88,     1,    89,    91,    92,   151,    93,
      94,   116,     3,    19,    13,    15,    16,    79,    17,   113,
     115,   152,   121,   122,    80,    18,     4,    48,   103,   104,
     105,   106,   107,    51,    52,    53,   110,   111,    79,   129,
     130,   131,   132,   133,   134,    80,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,    86,    95,   117,   118,
       5,    63,   119,   103,   104,     7,    14,   107,   154,    21,
       8,    22,    15,    16,    64,    17,    90,    23,    63,    66,
      67,    25,     9,    36,    37,    36,   153,   155,    26,    54,
      56,    64,   158,    55,    57,   159,    66,    67,    51,    52,
      53,    10,   162,    46,    68,    69,    70,    38,    71,    58,
      59,    39,    40,    60,    62,    76,    77,    51,    52,    53,
      46,    68,    69,    70,    41,    71,    78,    81,    82,    85,
      63,   124,    51,    52,    53,    83,    15,    16,    29,    17,
      42,   125,    84,    64,    43,    65,   127,    44,    66,    67,
      45,    46,    38,   146,    47,   149,    39,    87,   103,   104,
     105,   106,   107,    63,    15,    16,    30,    17,   148,    41,
     150,   156,    46,    68,    69,    70,    64,    71,   160,   157,
     163,    66,    67,   123,    63,    42,    34,     0,    35,    43,
       0,     0,    44,     0,     0,    45,    46,    64,     0,    47,
      38,     0,    66,    67,    39,    46,    68,    69,    70,   114,
      71,     0,    15,    16,    30,    17,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,    46,    68,    69,    70,
       0,    71,     0,    42,     0,     0,     0,    43,     0,     0,
      44,   126,     0,    45,    46,     0,    96,    47,    97,    98,
      99,   100,   101,     0,     0,     0,   103,   104,   105,   106,
     107,   108,   109,   128,   110,   111,   112,     0,    96,     0,
      97,    98,    99,   100,   101,     0,     0,     0,   103,   104,
     105,   106,   107,   108,   109,   145,   110,   111,   112,     0,
      96,     0,    97,    98,    99,   100,   101,     0,     0,     0,
     103,   104,   105,   106,   107,   108,   109,   147,   110,   111,
     112,     0,    96,     0,    97,    98,    99,   100,   101,     0,
       0,     0,   103,   104,   105,   106,   107,   108,   109,     0,
     110,   111,   112,   161,    96,     0,    97,    98,    99,   100,
     101,     0,     0,     0,   103,   104,   105,   106,   107,   108,
     109,     0,   110,   111,   112,    96,     0,    97,    98,    99,
     100,   101,     0,     0,   102,   103,   104,   105,   106,   107,
     108,   109,     0,   110,   111,   112,    96,     0,    97,    98,
      99,   100,   101,     0,     0,     0,   103,   104,   105,   106,
     107,   108,   109,     0,   110,   111,   112,    96,     0,    97,
      98,    99,   100,   101,     0,     0,     0,   103,   104,   105,
     106,   107,   108,     0,     0,   110,   111,   112,    96,     0,
      97,    98,    99,   100,   101,     0,     0,     0,   103,   104,
     105,   106,   107,     0,     0,     0,   110,   111,   112,    96,
       0,    97,    98,    99,   100,   101,     0,     0,     0,   103,
     104,   105,   106,   107,     0,     0,     0,   110,   111,    97,
      98,    99,   100,     0,     0,     0,     0,   103,   104,   105,
     106,   107,     0,     0,     0,   110,   111
};

static const yytype_int16 yycheck[] =
{
      28,    28,    28,    61,    36,    62,    63,    64,     4,    66,
      67,     1,    48,    14,    18,    30,    31,     3,    33,    76,
      77,    17,    79,    80,    10,    40,     0,    28,    19,    20,
      21,    22,    23,    61,    61,    61,    27,    28,     3,    96,
      97,    98,    99,   100,   101,    10,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    57,    43,    48,     1,
       5,     3,     4,    19,    20,     1,    39,    23,   126,    48,
       6,    48,    30,    31,    16,    33,     1,     5,     3,    21,
      22,     3,    18,    17,    18,    17,    18,   145,     3,     7,
       4,    16,   149,    48,    17,   152,    21,    22,   126,   126,
     126,    37,   160,    45,    46,    47,    48,     1,    50,     4,
      48,     5,     6,    18,     3,     3,     3,   145,   145,   145,
      45,    46,    47,    48,    18,    50,     3,    48,    18,     8,
       3,    48,   160,   160,   160,    18,    30,    31,     4,    33,
      34,    48,    18,    16,    38,    18,     4,    41,    21,    22,
      44,    45,     1,     4,    48,     7,     5,     6,    19,    20,
      21,    22,    23,     3,    30,    31,    32,    33,     4,    18,
       4,    18,    45,    46,    47,    48,    16,    50,    35,    18,
       4,    21,    22,    81,     3,    34,     4,    -1,    26,    38,
      -1,    -1,    41,    -1,    -1,    44,    45,    16,    -1,    48,
       1,    -1,    21,    22,     5,    45,    46,    47,    48,    49,
      50,    -1,    30,    31,    32,    33,    -1,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,
      -1,    50,    -1,    34,    -1,    -1,    -1,    38,    -1,    -1,
      41,     4,    -1,    44,    45,    -1,     9,    48,    11,    12,
      13,    14,    15,    -1,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,     4,    27,    28,    29,    -1,     9,    -1,
      11,    12,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,     4,    27,    28,    29,    -1,
       9,    -1,    11,    12,    13,    14,    15,    -1,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,     4,    27,    28,
      29,    -1,     9,    -1,    11,    12,    13,    14,    15,    -1,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    29,     8,     9,    -1,    11,    12,    13,    14,
      15,    -1,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    29,     9,    -1,    11,    12,    13,
      14,    15,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    27,    28,    29,     9,    -1,    11,    12,
      13,    14,    15,    -1,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    29,     9,    -1,    11,
      12,    13,    14,    15,    -1,    -1,    -1,    19,    20,    21,
      22,    23,    24,    -1,    -1,    27,    28,    29,     9,    -1,
      11,    12,    13,    14,    15,    -1,    -1,    -1,    19,    20,
      21,    22,    23,    -1,    -1,    -1,    27,    28,    29,     9,
      -1,    11,    12,    13,    14,    15,    -1,    -1,    -1,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    27,    28,    11,
      12,    13,    14,    -1,    -1,    -1,    -1,    19,    20,    21,
      22,    23,    -1,    -1,    -1,    27,    28
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    36,    54,    48,     0,     5,    55,     1,     6,    18,
      37,    56,    57,    18,    39,    30,    31,    33,    40,    59,
      60,    48,    48,     5,    63,     3,     3,    58,    64,     4,
      32,    59,    61,    62,     4,    61,    17,    18,     1,     5,
       6,    18,    34,    38,    41,    44,    45,    48,    59,    65,
      66,    68,    70,    71,     7,    48,     4,    17,     4,    48,
      18,    67,     3,     3,    16,    18,    21,    22,    46,    47,
      48,    50,    68,    70,    71,    72,     3,     3,     3,     3,
      10,    48,    18,    18,    18,     8,    59,     6,    66,    72,
       1,    72,    72,    72,    72,    43,     9,    11,    12,    13,
      14,    15,    18,    19,    20,    21,    22,    23,    24,    25,
      27,    28,    29,    72,    49,    72,     1,    48,     1,     4,
      69,    72,    72,    58,    48,    48,     4,     4,     4,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,     4,     4,     4,     4,     7,
       4,     4,    17,    18,    66,    66,    18,    18,    72,    72,
      35,     8,    66,     4
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    55,    55,    55,    55,    56,    57,    57,
      58,    58,    59,    59,    59,    60,    60,    60,    60,    61,
      61,    62,    62,    63,    64,    64,    64,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    67,    68,    68,    68,    69,    69,    70,    71,
      71,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     0,     2,     2,     2,     4,     6,     2,
       0,     3,     1,     1,     1,     4,     5,     4,     5,     1,
       4,     2,     4,     3,     0,     2,     2,     4,     3,     5,
       7,     5,     2,     3,     1,     2,     2,     2,     5,     5,
       2,     0,     2,     3,     4,     4,     1,     3,     3,     7,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       3,     3,     1,     1,     1,     1,     2,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: CLASS IDENTIFIER LBRACE MemberList RBRACE  */
#line 112 "jucompiler.y"
                                              {
      (yyval.node) = newnode(Program, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
      addchild((yyval.node), newnode(Identifier, (yyvsp[-3].val), (yylsp[-3]).first_line, (yylsp[-3]).first_column)); // IDENTIFIER
      addchildren((yyval.node), (yyvsp[-1].list)); // all members from the list MemberList
      root = (yyval.node);
    }
#line 1600 "y.tab.c"
    break;

  case 3: /* MemberList: %empty  */
#line 121 "jucompiler.y"
                { (yyval.list) = newlist(); }
#line 1606 "y.tab.c"
    break;

  case 4: /* MemberList: MemberList MethodDecl  */
#line 122 "jucompiler.y"
                          {
      (yyval.list) = (yyvsp[-1].list);
      append((yyval.list), (yyvsp[0].node));
    }
#line 1615 "y.tab.c"
    break;

  case 5: /* MemberList: MemberList FieldDecl  */
#line 126 "jucompiler.y"
                         {
      (yyval.list) = (yyvsp[-1].list);
      struct node_list *field_list = (yyvsp[0].list);
      while ((field_list = field_list->next) != NULL) {
        append((yyval.list), field_list->node);
      }
      free((yyvsp[0].list));
    }
#line 1628 "y.tab.c"
    break;

  case 6: /* MemberList: MemberList SEMICOLON  */
#line 134 "jucompiler.y"
                         {
      (yyval.list) = (yyvsp[-1].list);  // ignore empty statement
    }
#line 1636 "y.tab.c"
    break;

  case 7: /* MethodDecl: PUBLIC STATIC MethodHeader MethodBody  */
#line 140 "jucompiler.y"
                                          {
      (yyval.node) = newnode(MethodDecl, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
      addchild((yyval.node), (yyvsp[-1].node)); // MethodHeader
      addchild((yyval.node), (yyvsp[0].node)); // MethodBody
    }
#line 1646 "y.tab.c"
    break;

  case 8: /* FieldDecl: PUBLIC STATIC Type IDENTIFIER IdentifierList SEMICOLON  */
#line 148 "jucompiler.y"
                                                           {
      // first IDENTIFIER
      (yyval.list) = newlist();
      struct node *field = newnode(FieldDecl, NULL,  (yylsp[-5]).first_line, (yylsp[-5]).first_column);
      addchild(field, (yyvsp[-3].node));  // Type
      addchild(field, newnode(Identifier, (yyvsp[-2].val), (yylsp[-2]).first_line, (yylsp[-2]).first_column));  // IDENTIFIER
      append((yyval.list), field);

      // create a list for additional IDENTIFIERs
      struct node_list *identifiers = (yyvsp[-1].list);
      while ((identifiers = identifiers->next) != NULL) {
        struct node *extra_field = newnode(FieldDecl, NULL, (yylsp[-5]).first_line, (yylsp[-5]).first_column);
        // new Type node for each field
        enum category type_category = (yyvsp[-3].node)->category;
        addchild(extra_field, newnode(type_category, NULL, (yylsp[-5]).first_line, (yylsp[-5]).first_column));
        addchild(extra_field, identifiers->node); // IDENTIFIER
        append((yyval.list), extra_field);
      }
      free((yyvsp[-1].list));
    }
#line 1671 "y.tab.c"
    break;

  case 9: /* FieldDecl: error SEMICOLON  */
#line 168 "jucompiler.y"
                    { (yyval.list) = newlist(); }
#line 1677 "y.tab.c"
    break;

  case 10: /* IdentifierList: %empty  */
#line 172 "jucompiler.y"
                { (yyval.list) = newlist(); }
#line 1683 "y.tab.c"
    break;

  case 11: /* IdentifierList: IdentifierList COMMA IDENTIFIER  */
#line 173 "jucompiler.y"
                                    {
      (yyval.list) = (yyvsp[-2].list);
      append((yyval.list), newnode(Identifier, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column));
    }
#line 1692 "y.tab.c"
    break;

  case 12: /* Type: BOOL  */
#line 180 "jucompiler.y"
            { (yyval.node) = newnode(Bool, NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 1698 "y.tab.c"
    break;

  case 13: /* Type: INT  */
#line 181 "jucompiler.y"
            { (yyval.node) = newnode(Int, NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 1704 "y.tab.c"
    break;

  case 14: /* Type: DOUBLE  */
#line 182 "jucompiler.y"
            { (yyval.node) = newnode(Double, NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 1710 "y.tab.c"
    break;

  case 15: /* MethodHeader: Type IDENTIFIER LPAR RPAR  */
#line 186 "jucompiler.y"
                              {
      (yyval.node) = newnode(MethodHeader, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
      addchild((yyval.node), (yyvsp[-3].node));
      addchild((yyval.node), newnode(Identifier, (yyvsp[-2].val), (yylsp[-2]).first_line, (yylsp[-2]).first_column));
      addchild((yyval.node), newnode(MethodParams, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column));
    }
#line 1721 "y.tab.c"
    break;

  case 16: /* MethodHeader: Type IDENTIFIER LPAR FormalParams RPAR  */
#line 192 "jucompiler.y"
                                           {
      (yyval.node) = newnode(MethodHeader, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
      addchild((yyval.node), (yyvsp[-4].node));
      addchild((yyval.node), newnode(Identifier, (yyvsp[-3].val), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
      struct node *params = newnode(MethodParams, NULL, (yylsp[-2]).first_line, (yylsp[-2]).first_column);
      addchildren(params, (yyvsp[-1].list));
      addchild((yyval.node), params);
    }
#line 1734 "y.tab.c"
    break;

  case 17: /* MethodHeader: VOID IDENTIFIER LPAR RPAR  */
#line 200 "jucompiler.y"
                              {
      (yyval.node) = newnode(MethodHeader, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
      addchild((yyval.node), newnode(Void, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column));
      addchild((yyval.node), newnode(Identifier, (yyvsp[-2].val), (yylsp[-2]).first_line, (yylsp[-2]).first_column));
      addchild((yyval.node), newnode(MethodParams, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column));
    }
#line 1745 "y.tab.c"
    break;

  case 18: /* MethodHeader: VOID IDENTIFIER LPAR FormalParams RPAR  */
#line 206 "jucompiler.y"
                                           {
      (yyval.node) = newnode(MethodHeader, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
      addchild((yyval.node), newnode(Void, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column));
      addchild((yyval.node), newnode(Identifier, (yyvsp[-3].val), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
      struct node *params = newnode(MethodParams, NULL, (yylsp[-2]).first_line, (yylsp[-2]).first_column);
      addchildren(params, (yyvsp[-1].list));
      addchild((yyval.node), params);
    }
#line 1758 "y.tab.c"
    break;

  case 19: /* FormalParams: ParamList  */
#line 217 "jucompiler.y"
              { (yyval.list) = (yyvsp[0].list); }
#line 1764 "y.tab.c"
    break;

  case 20: /* FormalParams: STRING LSQ RSQ IDENTIFIER  */
#line 218 "jucompiler.y"
                              {
      (yyval.list) = newlist();
      struct node *param = newnode(ParamDecl, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
      addchild(param, newnode(StringArray, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column));
      addchild(param, newnode(Identifier, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column));
      append((yyval.list), param);
    }
#line 1776 "y.tab.c"
    break;

  case 21: /* ParamList: Type IDENTIFIER  */
#line 228 "jucompiler.y"
                    {
      (yyval.list) = newlist();
      struct node *param = newnode(ParamDecl, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild(param, (yyvsp[-1].node)); // Type
      addchild(param, newnode(Identifier, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column)); // IDENTIFIER
      append((yyval.list), param);
    }
#line 1788 "y.tab.c"
    break;

  case 22: /* ParamList: ParamList COMMA Type IDENTIFIER  */
#line 235 "jucompiler.y"
                                    {
      (yyval.list) = (yyvsp[-3].list);
      struct node *param = newnode(ParamDecl, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild(param, (yyvsp[-1].node)); // Type
      addchild(param, newnode(Identifier, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column)); // IDENTIFIER
      append((yyval.list), param);
    }
#line 1800 "y.tab.c"
    break;

  case 23: /* MethodBody: LBRACE StatementOrVarDecl RBRACE  */
#line 245 "jucompiler.y"
                                     {
      (yyval.node) = newnode(MethodBody, NULL, (yylsp[-2]).first_line, (yylsp[-2]).first_column);
      addchildren((yyval.node), (yyvsp[-1].list));
    }
#line 1809 "y.tab.c"
    break;

  case 24: /* StatementOrVarDecl: %empty  */
#line 252 "jucompiler.y"
                { (yyval.list) = newlist(); }
#line 1815 "y.tab.c"
    break;

  case 25: /* StatementOrVarDecl: StatementOrVarDecl Statement  */
#line 253 "jucompiler.y"
                                 {
      (yyval.list) = (yyvsp[-1].list);
      // add non-NULL statements
      if ((yyvsp[0].node) != NULL)
        append((yyval.list), (yyvsp[0].node));
    }
#line 1826 "y.tab.c"
    break;

  case 26: /* StatementOrVarDecl: StatementOrVarDecl VarDecl  */
#line 259 "jucompiler.y"
                               {
      (yyval.list) = (yyvsp[-1].list);
      struct node_list *var_list = (yyvsp[0].list);
      while ((var_list = var_list->next) != NULL) {
        append((yyval.list), var_list->node);
      }
      free((yyvsp[0].list));
    }
#line 1839 "y.tab.c"
    break;

  case 27: /* VarDecl: Type IDENTIFIER IdentifierList SEMICOLON  */
#line 270 "jucompiler.y"
                                             {
      (yyval.list) = newlist();

      // first Var
      struct node *var = newnode(VarDecl, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
      addchild(var, (yyvsp[-3].node));
      addchild(var, newnode(Identifier, (yyvsp[-2].val), (yylsp[-2]).first_line, (yylsp[-2]).first_column));
      append((yyval.list), var);

      // additional Vars
      struct node_list *identifiers = (yyvsp[-1].list);
      while ((identifiers = identifiers->next) != NULL) {
        struct node *extra_var = newnode(VarDecl, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
        enum category type_category = (yyvsp[-3].node)->category;
        addchild(extra_var, newnode(type_category, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column));
        addchild(extra_var, identifiers->node);
        append((yyval.list), extra_var);
      }
      free((yyvsp[-1].list));
    }
#line 1864 "y.tab.c"
    break;

  case 28: /* Statement: LBRACE StatementList RBRACE  */
#line 293 "jucompiler.y"
                                {
      // check number of statements
      int count = 0;
      struct node_list *temp = (yyvsp[-1].list);
      while ((temp = temp->next) != NULL) count++;

      if (count == 0) {
        (yyval.node) = NULL;
      } else if (count == 1) {
        (yyval.node) = (yyvsp[-1].list)->next->node;
        free((yyvsp[-1].list));
      } else {
        (yyval.node) = newnode(Block, NULL, (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        addchildren((yyval.node), (yyvsp[-1].list));
      }
    }
#line 1885 "y.tab.c"
    break;

  case 29: /* Statement: IF LPAR Expr RPAR Statement  */
#line 309 "jucompiler.y"
                                                      {
      (yyval.node) = newnode(If, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
      addchild((yyval.node), (yyvsp[-2].node));
      if ((yyvsp[0].node) != NULL)
        addchild((yyval.node), (yyvsp[0].node));
      else
        addchild((yyval.node), newnode(Block, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column));
      addchild((yyval.node), newnode(Block, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column));
    }
#line 1899 "y.tab.c"
    break;

  case 30: /* Statement: IF LPAR Expr RPAR Statement ELSE Statement  */
#line 318 "jucompiler.y"
                                               {
      (yyval.node) = newnode(If, NULL, (yylsp[-6]).first_line, (yylsp[-6]).first_column);
      addchild((yyval.node), (yyvsp[-4].node));
      if ((yyvsp[-2].node) != NULL)
        addchild((yyval.node), (yyvsp[-2].node));
      else
        addchild((yyval.node), newnode(Block, NULL, (yylsp[-6]).first_line, (yylsp[-6]).first_column));
      if ((yyvsp[0].node) != NULL)
        addchild((yyval.node), (yyvsp[0].node));
      else
        addchild((yyval.node), newnode(Block, NULL, (yylsp[-6]).first_line, (yylsp[-6]).first_column));
    }
#line 1916 "y.tab.c"
    break;

  case 31: /* Statement: WHILE LPAR Expr RPAR Statement  */
#line 330 "jucompiler.y"
                                   {
      (yyval.node) = newnode(While, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
      addchild((yyval.node), (yyvsp[-2].node));
      if ((yyvsp[0].node) != NULL)
        addchild((yyval.node), (yyvsp[0].node));
      else
        addchild((yyval.node), newnode(Block, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column));
    }
#line 1929 "y.tab.c"
    break;

  case 32: /* Statement: RETURN SEMICOLON  */
#line 338 "jucompiler.y"
                     {
      (yyval.node) = newnode(Return, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 1937 "y.tab.c"
    break;

  case 33: /* Statement: RETURN Expr SEMICOLON  */
#line 341 "jucompiler.y"
                          {
      (yyval.node) = newnode(Return, NULL, (yylsp[-2]).first_line, (yylsp[-2]).first_column);
      addchild((yyval.node), (yyvsp[-1].node));
    }
#line 1946 "y.tab.c"
    break;

  case 34: /* Statement: SEMICOLON  */
#line 345 "jucompiler.y"
              { (yyval.node) = NULL; }
#line 1952 "y.tab.c"
    break;

  case 35: /* Statement: MethodInvocation SEMICOLON  */
#line 346 "jucompiler.y"
                               { (yyval.node) = (yyvsp[-1].node); }
#line 1958 "y.tab.c"
    break;

  case 36: /* Statement: Assignment SEMICOLON  */
#line 347 "jucompiler.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1964 "y.tab.c"
    break;

  case 37: /* Statement: ParseArgs SEMICOLON  */
#line 348 "jucompiler.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 1970 "y.tab.c"
    break;

  case 38: /* Statement: PRINT LPAR Expr RPAR SEMICOLON  */
#line 349 "jucompiler.y"
                                   {
      (yyval.node) = newnode(Print, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
      addchild((yyval.node), (yyvsp[-2].node));
    }
#line 1979 "y.tab.c"
    break;

  case 39: /* Statement: PRINT LPAR STRLIT RPAR SEMICOLON  */
#line 353 "jucompiler.y"
                                     {
      (yyval.node) = newnode(Print, NULL, (yylsp[-4]).first_line, (yylsp[-4]).first_column);
      addchild((yyval.node), newnode(StrLit, (yyvsp[-2].val), (yylsp[-2]).first_line, (yylsp[-2]).first_column));
    }
#line 1988 "y.tab.c"
    break;

  case 40: /* Statement: error SEMICOLON  */
#line 357 "jucompiler.y"
                    { (yyval.node) = NULL; }
#line 1994 "y.tab.c"
    break;

  case 41: /* StatementList: %empty  */
#line 361 "jucompiler.y"
                { (yyval.list) = newlist(); }
#line 2000 "y.tab.c"
    break;

  case 42: /* StatementList: StatementList Statement  */
#line 362 "jucompiler.y"
                            {
      (yyval.list) = (yyvsp[-1].list);
      if ((yyvsp[0].node) != NULL)
        append((yyval.list), (yyvsp[0].node));
    }
#line 2010 "y.tab.c"
    break;

  case 43: /* MethodInvocation: IDENTIFIER LPAR RPAR  */
#line 370 "jucompiler.y"
                         {
      (yyval.node) = newnode(Call, NULL, (yylsp[-2]).first_line, (yylsp[-2]).first_column);
      addchild((yyval.node), newnode(Identifier, (yyvsp[-2].val), (yylsp[-2]).first_line, (yylsp[-2]).first_column));
    }
#line 2019 "y.tab.c"
    break;

  case 44: /* MethodInvocation: IDENTIFIER LPAR ArgList RPAR  */
#line 374 "jucompiler.y"
                                 {
      (yyval.node) = newnode(Call, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
      addchild((yyval.node), newnode(Identifier, (yyvsp[-3].val), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
      addchildren((yyval.node), (yyvsp[-1].list));
    }
#line 2029 "y.tab.c"
    break;

  case 45: /* MethodInvocation: IDENTIFIER LPAR error RPAR  */
#line 379 "jucompiler.y"
                               {
      (yyval.node) = newnode(Call, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
      addchild((yyval.node), newnode(Identifier, (yyvsp[-3].val), (yylsp[-3]).first_line, (yylsp[-3]).first_column));
    }
#line 2038 "y.tab.c"
    break;

  case 46: /* ArgList: Expr  */
#line 386 "jucompiler.y"
         {
      (yyval.list) = newlist();
      append((yyval.list), (yyvsp[0].node));
    }
#line 2047 "y.tab.c"
    break;

  case 47: /* ArgList: ArgList COMMA Expr  */
#line 390 "jucompiler.y"
                       {
      (yyval.list) = (yyvsp[-2].list);
      append((yyval.list), (yyvsp[0].node));
    }
#line 2056 "y.tab.c"
    break;

  case 48: /* Assignment: IDENTIFIER ASSIGN Expr  */
#line 397 "jucompiler.y"
                           {
      (yyval.node) = newnode(Assign, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), newnode(Identifier, (yyvsp[-2].val), (yylsp[-2]).first_line, (yylsp[-2]).first_column));
      addchild((yyval.node), (yyvsp[0].node));
    }
#line 2066 "y.tab.c"
    break;

  case 49: /* ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR  */
#line 405 "jucompiler.y"
                                               {
      (yyval.node) = newnode(ParseArgs, NULL, (yylsp[-6]).first_line, (yylsp[-6]).first_column);
      addchild((yyval.node), newnode(Identifier, (yyvsp[-4].val), (yylsp[-4]).first_line, (yylsp[-4]).first_column));
      addchild((yyval.node), (yyvsp[-2].node));
    }
#line 2076 "y.tab.c"
    break;

  case 50: /* ParseArgs: PARSEINT LPAR error RPAR  */
#line 410 "jucompiler.y"
                             {
      (yyval.node) = newnode(ParseArgs, NULL, (yylsp[-3]).first_line, (yylsp[-3]).first_column);
    }
#line 2084 "y.tab.c"
    break;

  case 51: /* Expr: Expr PLUS Expr  */
#line 416 "jucompiler.y"
                     {
      (yyval.node) = newnode(Add, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2094 "y.tab.c"
    break;

  case 52: /* Expr: Expr MINUS Expr  */
#line 421 "jucompiler.y"
                     {
      (yyval.node) = newnode(Sub, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2104 "y.tab.c"
    break;

  case 53: /* Expr: Expr STAR Expr  */
#line 426 "jucompiler.y"
                     {
      (yyval.node) = newnode(Mul, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2114 "y.tab.c"
    break;

  case 54: /* Expr: Expr DIV Expr  */
#line 431 "jucompiler.y"
                     {
      (yyval.node) = newnode(Div, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2124 "y.tab.c"
    break;

  case 55: /* Expr: Expr MOD Expr  */
#line 436 "jucompiler.y"
                     {
      (yyval.node) = newnode(Mod, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2134 "y.tab.c"
    break;

  case 56: /* Expr: Expr AND Expr  */
#line 441 "jucompiler.y"
                     {
      (yyval.node) = newnode(And, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2144 "y.tab.c"
    break;

  case 57: /* Expr: Expr OR Expr  */
#line 446 "jucompiler.y"
                     {
      (yyval.node) = newnode(Or, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2154 "y.tab.c"
    break;

  case 58: /* Expr: Expr XOR Expr  */
#line 451 "jucompiler.y"
                     {
      (yyval.node) = newnode(Xor, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2164 "y.tab.c"
    break;

  case 59: /* Expr: Expr LSHIFT Expr  */
#line 456 "jucompiler.y"
                     {
      (yyval.node) = newnode(Lshift, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2174 "y.tab.c"
    break;

  case 60: /* Expr: Expr RSHIFT Expr  */
#line 461 "jucompiler.y"
                     {
      (yyval.node) = newnode(Rshift, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2184 "y.tab.c"
    break;

  case 61: /* Expr: Expr EQ Expr  */
#line 466 "jucompiler.y"
                     {
      (yyval.node) = newnode(Eq, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2194 "y.tab.c"
    break;

  case 62: /* Expr: Expr GE Expr  */
#line 471 "jucompiler.y"
                     {
      (yyval.node) = newnode(Ge, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2204 "y.tab.c"
    break;

  case 63: /* Expr: Expr GT Expr  */
#line 476 "jucompiler.y"
                     {
      (yyval.node) = newnode(Gt, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2214 "y.tab.c"
    break;

  case 64: /* Expr: Expr LE Expr  */
#line 481 "jucompiler.y"
                     {
      (yyval.node) = newnode(Le, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2224 "y.tab.c"
    break;

  case 65: /* Expr: Expr LT Expr  */
#line 486 "jucompiler.y"
                     {
      (yyval.node) = newnode(Lt, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2234 "y.tab.c"
    break;

  case 66: /* Expr: Expr NE Expr  */
#line 491 "jucompiler.y"
                     {
      (yyval.node) = newnode(Ne, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[-2].node)); // Expr1
      addchild((yyval.node), (yyvsp[0].node)); // Expr2
    }
#line 2244 "y.tab.c"
    break;

  case 67: /* Expr: MINUS Expr  */
#line 496 "jucompiler.y"
                                      {
      (yyval.node) = newnode(Minus, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[0].node));
    }
#line 2253 "y.tab.c"
    break;

  case 68: /* Expr: PLUS Expr  */
#line 500 "jucompiler.y"
                                      {
      (yyval.node) = newnode(Plus, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[0].node));
    }
#line 2262 "y.tab.c"
    break;

  case 69: /* Expr: NOT Expr  */
#line 504 "jucompiler.y"
                                      {
      (yyval.node) = newnode(Not, NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
      addchild((yyval.node), (yyvsp[0].node));
    }
#line 2271 "y.tab.c"
    break;

  case 70: /* Expr: LPAR Expr RPAR  */
#line 508 "jucompiler.y"
                   { (yyval.node) = (yyvsp[-1].node); }
#line 2277 "y.tab.c"
    break;

  case 71: /* Expr: LPAR error RPAR  */
#line 509 "jucompiler.y"
                    {
      (yyval.node) = NULL;
    }
#line 2285 "y.tab.c"
    break;

  case 72: /* Expr: MethodInvocation  */
#line 512 "jucompiler.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2291 "y.tab.c"
    break;

  case 73: /* Expr: Assignment  */
#line 513 "jucompiler.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2297 "y.tab.c"
    break;

  case 74: /* Expr: ParseArgs  */
#line 514 "jucompiler.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2303 "y.tab.c"
    break;

  case 75: /* Expr: IDENTIFIER  */
#line 515 "jucompiler.y"
               {
      (yyval.node) = newnode(Identifier, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2311 "y.tab.c"
    break;

  case 76: /* Expr: IDENTIFIER DOTLENGTH  */
#line 518 "jucompiler.y"
                         {
      (yyval.node) = newnode(Length, NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);
      addchild((yyval.node), newnode(Identifier, (yyvsp[-1].val), (yylsp[-1]).first_line, (yylsp[-1]).first_column));
    }
#line 2320 "y.tab.c"
    break;

  case 77: /* Expr: NATURAL  */
#line 522 "jucompiler.y"
            {
      (yyval.node) = newnode(Natural, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2328 "y.tab.c"
    break;

  case 78: /* Expr: DECIMAL  */
#line 525 "jucompiler.y"
            {
      (yyval.node) = newnode(Decimal, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2336 "y.tab.c"
    break;

  case 79: /* Expr: BOOLLIT  */
#line 528 "jucompiler.y"
            {
      (yyval.node) = newnode(BoolLit, (yyvsp[0].val), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2344 "y.tab.c"
    break;


#line 2348 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 533 "jucompiler.y"


void yyerror(char *s) {
    syntax_errors++;
    int col = (yytext[0] == '\0') ? col_count : string_col;
    printf("Line %d, col %d: %s: %s\n", line_count, col, s, last_token_text[0] ? last_token_text : yytext);
    last_token_text[0] = '\0'; //reset
}
