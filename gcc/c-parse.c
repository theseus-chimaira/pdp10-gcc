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
#line 34 "c-parse.y"

#include "config.h"
#include "system.h"
#include "tree.h"
#include "input.h"
#include "cpplib.h"
#include "intl.h"
#include "timevar.h"
#include "c-lex.h"
#include "c-tree.h"
#include "c-pragma.h"
#include "flags.h"
#include "output.h"
#include "toplev.h"
#include "ggc.h"
  
#ifdef MULTIBYTE_CHARS
#include <locale.h>
#endif


/* Like YYERROR but do call yyerror.  */
#define YYERROR1 { yyerror ("syntax error"); YYERROR; }
#ifndef YYLEX
#define YYLEX yylex ()
#endif
/* Cause the "yydebug" variable to be defined.  */
#define YYDEBUG 1

#line 101 "c-p12907.c"

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


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    IDENTIFIER = 258,              /* IDENTIFIER  */
    TYPENAME = 259,                /* TYPENAME  */
    SCSPEC = 260,                  /* SCSPEC  */
    STATIC = 261,                  /* STATIC  */
    TYPESPEC = 262,                /* TYPESPEC  */
    TYPE_QUAL = 263,               /* TYPE_QUAL  */
    CONSTANT = 264,                /* CONSTANT  */
    STRING = 265,                  /* STRING  */
    ELLIPSIS = 266,                /* ELLIPSIS  */
    SIZEOF = 267,                  /* SIZEOF  */
    ENUM = 268,                    /* ENUM  */
    STRUCT = 269,                  /* STRUCT  */
    UNION = 270,                   /* UNION  */
    IF = 271,                      /* IF  */
    ELSE = 272,                    /* ELSE  */
    WHILE = 273,                   /* WHILE  */
    DO = 274,                      /* DO  */
    FOR = 275,                     /* FOR  */
    SWITCH = 276,                  /* SWITCH  */
    CASE = 277,                    /* CASE  */
    DEFAULT = 278,                 /* DEFAULT  */
    BREAK = 279,                   /* BREAK  */
    CONTINUE = 280,                /* CONTINUE  */
    RETURN = 281,                  /* RETURN  */
    GOTO = 282,                    /* GOTO  */
    ASM_KEYWORD = 283,             /* ASM_KEYWORD  */
    TYPEOF = 284,                  /* TYPEOF  */
    ALIGNOF = 285,                 /* ALIGNOF  */
    ATTRIBUTE = 286,               /* ATTRIBUTE  */
    EXTENSION = 287,               /* EXTENSION  */
    LABEL = 288,                   /* LABEL  */
    REALPART = 289,                /* REALPART  */
    IMAGPART = 290,                /* IMAGPART  */
    VA_ARG = 291,                  /* VA_ARG  */
    CHOOSE_EXPR = 292,             /* CHOOSE_EXPR  */
    TYPES_COMPATIBLE_P = 293,      /* TYPES_COMPATIBLE_P  */
    PTR_VALUE = 294,               /* PTR_VALUE  */
    PTR_BASE = 295,                /* PTR_BASE  */
    PTR_EXTENT = 296,              /* PTR_EXTENT  */
    STRING_FUNC_NAME = 297,        /* STRING_FUNC_NAME  */
    VAR_FUNC_NAME = 298,           /* VAR_FUNC_NAME  */
    ASSIGN = 299,                  /* ASSIGN  */
    OROR = 300,                    /* OROR  */
    ANDAND = 301,                  /* ANDAND  */
    EQCOMPARE = 302,               /* EQCOMPARE  */
    ARITHCOMPARE = 303,            /* ARITHCOMPARE  */
    LSHIFT = 304,                  /* LSHIFT  */
    RSHIFT = 305,                  /* RSHIFT  */
    UNARY = 306,                   /* UNARY  */
    PLUSPLUS = 307,                /* PLUSPLUS  */
    MINUSMINUS = 308,              /* MINUSMINUS  */
    HYPERUNARY = 309,              /* HYPERUNARY  */
    POINTSAT = 310,                /* POINTSAT  */
    INTERFACE = 311,               /* INTERFACE  */
    IMPLEMENTATION = 312,          /* IMPLEMENTATION  */
    END = 313,                     /* END  */
    SELECTOR = 314,                /* SELECTOR  */
    DEFS = 315,                    /* DEFS  */
    ENCODE = 316,                  /* ENCODE  */
    CLASSNAME = 317,               /* CLASSNAME  */
    PUBLIC = 318,                  /* PUBLIC  */
    PRIVATE = 319,                 /* PRIVATE  */
    PROTECTED = 320,               /* PROTECTED  */
    PROTOCOL = 321,                /* PROTOCOL  */
    OBJECTNAME = 322,              /* OBJECTNAME  */
    CLASS = 323,                   /* CLASS  */
    ALIAS = 324                    /* ALIAS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 66 "c-parse.y"
long itype; tree ttype; enum tree_code code;
	const char *filename; int lineno; 

#line 221 "c-p12907.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_TYPENAME = 4,                   /* TYPENAME  */
  YYSYMBOL_SCSPEC = 5,                     /* SCSPEC  */
  YYSYMBOL_STATIC = 6,                     /* STATIC  */
  YYSYMBOL_TYPESPEC = 7,                   /* TYPESPEC  */
  YYSYMBOL_TYPE_QUAL = 8,                  /* TYPE_QUAL  */
  YYSYMBOL_CONSTANT = 9,                   /* CONSTANT  */
  YYSYMBOL_STRING = 10,                    /* STRING  */
  YYSYMBOL_ELLIPSIS = 11,                  /* ELLIPSIS  */
  YYSYMBOL_SIZEOF = 12,                    /* SIZEOF  */
  YYSYMBOL_ENUM = 13,                      /* ENUM  */
  YYSYMBOL_STRUCT = 14,                    /* STRUCT  */
  YYSYMBOL_UNION = 15,                     /* UNION  */
  YYSYMBOL_IF = 16,                        /* IF  */
  YYSYMBOL_ELSE = 17,                      /* ELSE  */
  YYSYMBOL_WHILE = 18,                     /* WHILE  */
  YYSYMBOL_DO = 19,                        /* DO  */
  YYSYMBOL_FOR = 20,                       /* FOR  */
  YYSYMBOL_SWITCH = 21,                    /* SWITCH  */
  YYSYMBOL_CASE = 22,                      /* CASE  */
  YYSYMBOL_DEFAULT = 23,                   /* DEFAULT  */
  YYSYMBOL_BREAK = 24,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 25,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 26,                    /* RETURN  */
  YYSYMBOL_GOTO = 27,                      /* GOTO  */
  YYSYMBOL_ASM_KEYWORD = 28,               /* ASM_KEYWORD  */
  YYSYMBOL_TYPEOF = 29,                    /* TYPEOF  */
  YYSYMBOL_ALIGNOF = 30,                   /* ALIGNOF  */
  YYSYMBOL_ATTRIBUTE = 31,                 /* ATTRIBUTE  */
  YYSYMBOL_EXTENSION = 32,                 /* EXTENSION  */
  YYSYMBOL_LABEL = 33,                     /* LABEL  */
  YYSYMBOL_REALPART = 34,                  /* REALPART  */
  YYSYMBOL_IMAGPART = 35,                  /* IMAGPART  */
  YYSYMBOL_VA_ARG = 36,                    /* VA_ARG  */
  YYSYMBOL_CHOOSE_EXPR = 37,               /* CHOOSE_EXPR  */
  YYSYMBOL_TYPES_COMPATIBLE_P = 38,        /* TYPES_COMPATIBLE_P  */
  YYSYMBOL_PTR_VALUE = 39,                 /* PTR_VALUE  */
  YYSYMBOL_PTR_BASE = 40,                  /* PTR_BASE  */
  YYSYMBOL_PTR_EXTENT = 41,                /* PTR_EXTENT  */
  YYSYMBOL_STRING_FUNC_NAME = 42,          /* STRING_FUNC_NAME  */
  YYSYMBOL_VAR_FUNC_NAME = 43,             /* VAR_FUNC_NAME  */
  YYSYMBOL_ASSIGN = 44,                    /* ASSIGN  */
  YYSYMBOL_45_ = 45,                       /* '='  */
  YYSYMBOL_46_ = 46,                       /* '?'  */
  YYSYMBOL_47_ = 47,                       /* ':'  */
  YYSYMBOL_OROR = 48,                      /* OROR  */
  YYSYMBOL_ANDAND = 49,                    /* ANDAND  */
  YYSYMBOL_50_ = 50,                       /* '|'  */
  YYSYMBOL_51_ = 51,                       /* '^'  */
  YYSYMBOL_52_ = 52,                       /* '&'  */
  YYSYMBOL_EQCOMPARE = 53,                 /* EQCOMPARE  */
  YYSYMBOL_ARITHCOMPARE = 54,              /* ARITHCOMPARE  */
  YYSYMBOL_LSHIFT = 55,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 56,                    /* RSHIFT  */
  YYSYMBOL_57_ = 57,                       /* '+'  */
  YYSYMBOL_58_ = 58,                       /* '-'  */
  YYSYMBOL_59_ = 59,                       /* '*'  */
  YYSYMBOL_60_ = 60,                       /* '/'  */
  YYSYMBOL_61_ = 61,                       /* '%'  */
  YYSYMBOL_UNARY = 62,                     /* UNARY  */
  YYSYMBOL_PLUSPLUS = 63,                  /* PLUSPLUS  */
  YYSYMBOL_MINUSMINUS = 64,                /* MINUSMINUS  */
  YYSYMBOL_HYPERUNARY = 65,                /* HYPERUNARY  */
  YYSYMBOL_POINTSAT = 66,                  /* POINTSAT  */
  YYSYMBOL_67_ = 67,                       /* '.'  */
  YYSYMBOL_68_ = 68,                       /* '('  */
  YYSYMBOL_69_ = 69,                       /* '['  */
  YYSYMBOL_INTERFACE = 70,                 /* INTERFACE  */
  YYSYMBOL_IMPLEMENTATION = 71,            /* IMPLEMENTATION  */
  YYSYMBOL_END = 72,                       /* END  */
  YYSYMBOL_SELECTOR = 73,                  /* SELECTOR  */
  YYSYMBOL_DEFS = 74,                      /* DEFS  */
  YYSYMBOL_ENCODE = 75,                    /* ENCODE  */
  YYSYMBOL_CLASSNAME = 76,                 /* CLASSNAME  */
  YYSYMBOL_PUBLIC = 77,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 78,                   /* PRIVATE  */
  YYSYMBOL_PROTECTED = 79,                 /* PROTECTED  */
  YYSYMBOL_PROTOCOL = 80,                  /* PROTOCOL  */
  YYSYMBOL_OBJECTNAME = 81,                /* OBJECTNAME  */
  YYSYMBOL_CLASS = 82,                     /* CLASS  */
  YYSYMBOL_ALIAS = 83,                     /* ALIAS  */
  YYSYMBOL_84_ = 84,                       /* ')'  */
  YYSYMBOL_85_ = 85,                       /* ';'  */
  YYSYMBOL_86_ = 86,                       /* '}'  */
  YYSYMBOL_87_ = 87,                       /* '~'  */
  YYSYMBOL_88_ = 88,                       /* '!'  */
  YYSYMBOL_89_ = 89,                       /* ','  */
  YYSYMBOL_90_ = 90,                       /* '{'  */
  YYSYMBOL_91_ = 91,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 92,                  /* $accept  */
  YYSYMBOL_program = 93,                   /* program  */
  YYSYMBOL_extdefs = 94,                   /* extdefs  */
  YYSYMBOL_95_1 = 95,                      /* @1  */
  YYSYMBOL_96_2 = 96,                      /* @2  */
  YYSYMBOL_extdef = 97,                    /* extdef  */
  YYSYMBOL_datadef = 98,                   /* datadef  */
  YYSYMBOL_fndef = 99,                     /* fndef  */
  YYSYMBOL_100_3 = 100,                    /* $@3  */
  YYSYMBOL_101_4 = 101,                    /* $@4  */
  YYSYMBOL_102_5 = 102,                    /* $@5  */
  YYSYMBOL_103_6 = 103,                    /* $@6  */
  YYSYMBOL_104_7 = 104,                    /* $@7  */
  YYSYMBOL_105_8 = 105,                    /* $@8  */
  YYSYMBOL_identifier = 106,               /* identifier  */
  YYSYMBOL_unop = 107,                     /* unop  */
  YYSYMBOL_expr = 108,                     /* expr  */
  YYSYMBOL_exprlist = 109,                 /* exprlist  */
  YYSYMBOL_nonnull_exprlist = 110,         /* nonnull_exprlist  */
  YYSYMBOL_unary_expr = 111,               /* unary_expr  */
  YYSYMBOL_sizeof = 112,                   /* sizeof  */
  YYSYMBOL_alignof = 113,                  /* alignof  */
  YYSYMBOL_cast_expr = 114,                /* cast_expr  */
  YYSYMBOL_expr_no_commas = 115,           /* expr_no_commas  */
  YYSYMBOL_116_9 = 116,                    /* $@9  */
  YYSYMBOL_117_10 = 117,                   /* $@10  */
  YYSYMBOL_118_11 = 118,                   /* $@11  */
  YYSYMBOL_119_12 = 119,                   /* $@12  */
  YYSYMBOL_120_13 = 120,                   /* $@13  */
  YYSYMBOL_primary = 121,                  /* primary  */
  YYSYMBOL_122_14 = 122,                   /* $@14  */
  YYSYMBOL_string = 123,                   /* string  */
  YYSYMBOL_old_style_parm_decls = 124,     /* old_style_parm_decls  */
  YYSYMBOL_lineno_datadecl = 125,          /* lineno_datadecl  */
  YYSYMBOL_datadecls = 126,                /* datadecls  */
  YYSYMBOL_datadecl = 127,                 /* datadecl  */
  YYSYMBOL_lineno_decl = 128,              /* lineno_decl  */
  YYSYMBOL_setspecs = 129,                 /* setspecs  */
  YYSYMBOL_maybe_resetattrs = 130,         /* maybe_resetattrs  */
  YYSYMBOL_decl = 131,                     /* decl  */
  YYSYMBOL_declspecs_nosc_nots_nosa_noea = 132, /* declspecs_nosc_nots_nosa_noea  */
  YYSYMBOL_declspecs_nosc_nots_nosa_ea = 133, /* declspecs_nosc_nots_nosa_ea  */
  YYSYMBOL_declspecs_nosc_nots_sa_noea = 134, /* declspecs_nosc_nots_sa_noea  */
  YYSYMBOL_declspecs_nosc_nots_sa_ea = 135, /* declspecs_nosc_nots_sa_ea  */
  YYSYMBOL_declspecs_nosc_ts_nosa_noea = 136, /* declspecs_nosc_ts_nosa_noea  */
  YYSYMBOL_declspecs_nosc_ts_nosa_ea = 137, /* declspecs_nosc_ts_nosa_ea  */
  YYSYMBOL_declspecs_nosc_ts_sa_noea = 138, /* declspecs_nosc_ts_sa_noea  */
  YYSYMBOL_declspecs_nosc_ts_sa_ea = 139,  /* declspecs_nosc_ts_sa_ea  */
  YYSYMBOL_declspecs_sc_nots_nosa_noea = 140, /* declspecs_sc_nots_nosa_noea  */
  YYSYMBOL_declspecs_sc_nots_nosa_ea = 141, /* declspecs_sc_nots_nosa_ea  */
  YYSYMBOL_declspecs_sc_nots_sa_noea = 142, /* declspecs_sc_nots_sa_noea  */
  YYSYMBOL_declspecs_sc_nots_sa_ea = 143,  /* declspecs_sc_nots_sa_ea  */
  YYSYMBOL_declspecs_sc_ts_nosa_noea = 144, /* declspecs_sc_ts_nosa_noea  */
  YYSYMBOL_declspecs_sc_ts_nosa_ea = 145,  /* declspecs_sc_ts_nosa_ea  */
  YYSYMBOL_declspecs_sc_ts_sa_noea = 146,  /* declspecs_sc_ts_sa_noea  */
  YYSYMBOL_declspecs_sc_ts_sa_ea = 147,    /* declspecs_sc_ts_sa_ea  */
  YYSYMBOL_declspecs_ts = 148,             /* declspecs_ts  */
  YYSYMBOL_declspecs_nots = 149,           /* declspecs_nots  */
  YYSYMBOL_declspecs_ts_nosa = 150,        /* declspecs_ts_nosa  */
  YYSYMBOL_declspecs_nots_nosa = 151,      /* declspecs_nots_nosa  */
  YYSYMBOL_declspecs_nosc_ts = 152,        /* declspecs_nosc_ts  */
  YYSYMBOL_declspecs_nosc_nots = 153,      /* declspecs_nosc_nots  */
  YYSYMBOL_declspecs_nosc = 154,           /* declspecs_nosc  */
  YYSYMBOL_declspecs = 155,                /* declspecs  */
  YYSYMBOL_maybe_type_quals_attrs = 156,   /* maybe_type_quals_attrs  */
  YYSYMBOL_typespec_nonattr = 157,         /* typespec_nonattr  */
  YYSYMBOL_typespec_attr = 158,            /* typespec_attr  */
  YYSYMBOL_typespec_reserved_nonattr = 159, /* typespec_reserved_nonattr  */
  YYSYMBOL_typespec_reserved_attr = 160,   /* typespec_reserved_attr  */
  YYSYMBOL_typespec_nonreserved_nonattr = 161, /* typespec_nonreserved_nonattr  */
  YYSYMBOL_initdecls = 162,                /* initdecls  */
  YYSYMBOL_notype_initdecls = 163,         /* notype_initdecls  */
  YYSYMBOL_maybeasm = 164,                 /* maybeasm  */
  YYSYMBOL_initdcl = 165,                  /* initdcl  */
  YYSYMBOL_166_15 = 166,                   /* @15  */
  YYSYMBOL_notype_initdcl = 167,           /* notype_initdcl  */
  YYSYMBOL_168_16 = 168,                   /* @16  */
  YYSYMBOL_maybe_attribute = 169,          /* maybe_attribute  */
  YYSYMBOL_attributes = 170,               /* attributes  */
  YYSYMBOL_attribute = 171,                /* attribute  */
  YYSYMBOL_attribute_list = 172,           /* attribute_list  */
  YYSYMBOL_attrib = 173,                   /* attrib  */
  YYSYMBOL_any_word = 174,                 /* any_word  */
  YYSYMBOL_scspec = 175,                   /* scspec  */
  YYSYMBOL_init = 176,                     /* init  */
  YYSYMBOL_177_17 = 177,                   /* $@17  */
  YYSYMBOL_initlist_maybe_comma = 178,     /* initlist_maybe_comma  */
  YYSYMBOL_initlist1 = 179,                /* initlist1  */
  YYSYMBOL_initelt = 180,                  /* initelt  */
  YYSYMBOL_181_18 = 181,                   /* $@18  */
  YYSYMBOL_initval = 182,                  /* initval  */
  YYSYMBOL_183_19 = 183,                   /* $@19  */
  YYSYMBOL_designator_list = 184,          /* designator_list  */
  YYSYMBOL_designator = 185,               /* designator  */
  YYSYMBOL_nested_function = 186,          /* nested_function  */
  YYSYMBOL_187_20 = 187,                   /* $@20  */
  YYSYMBOL_188_21 = 188,                   /* $@21  */
  YYSYMBOL_notype_nested_function = 189,   /* notype_nested_function  */
  YYSYMBOL_190_22 = 190,                   /* $@22  */
  YYSYMBOL_191_23 = 191,                   /* $@23  */
  YYSYMBOL_declarator = 192,               /* declarator  */
  YYSYMBOL_after_type_declarator = 193,    /* after_type_declarator  */
  YYSYMBOL_parm_declarator = 194,          /* parm_declarator  */
  YYSYMBOL_parm_declarator_starttypename = 195, /* parm_declarator_starttypename  */
  YYSYMBOL_parm_declarator_nostarttypename = 196, /* parm_declarator_nostarttypename  */
  YYSYMBOL_notype_declarator = 197,        /* notype_declarator  */
  YYSYMBOL_struct_head = 198,              /* struct_head  */
  YYSYMBOL_union_head = 199,               /* union_head  */
  YYSYMBOL_enum_head = 200,                /* enum_head  */
  YYSYMBOL_structsp_attr = 201,            /* structsp_attr  */
  YYSYMBOL_202_24 = 202,                   /* @24  */
  YYSYMBOL_203_25 = 203,                   /* @25  */
  YYSYMBOL_204_26 = 204,                   /* @26  */
  YYSYMBOL_205_27 = 205,                   /* @27  */
  YYSYMBOL_structsp_nonattr = 206,         /* structsp_nonattr  */
  YYSYMBOL_maybecomma = 207,               /* maybecomma  */
  YYSYMBOL_maybecomma_warn = 208,          /* maybecomma_warn  */
  YYSYMBOL_component_decl_list = 209,      /* component_decl_list  */
  YYSYMBOL_component_decl_list2 = 210,     /* component_decl_list2  */
  YYSYMBOL_component_decl = 211,           /* component_decl  */
  YYSYMBOL_components = 212,               /* components  */
  YYSYMBOL_components_notype = 213,        /* components_notype  */
  YYSYMBOL_component_declarator = 214,     /* component_declarator  */
  YYSYMBOL_component_notype_declarator = 215, /* component_notype_declarator  */
  YYSYMBOL_enumlist = 216,                 /* enumlist  */
  YYSYMBOL_enumerator = 217,               /* enumerator  */
  YYSYMBOL_typename = 218,                 /* typename  */
  YYSYMBOL_219_28 = 219,                   /* @28  */
  YYSYMBOL_absdcl = 220,                   /* absdcl  */
  YYSYMBOL_absdcl_maybe_attribute = 221,   /* absdcl_maybe_attribute  */
  YYSYMBOL_absdcl1 = 222,                  /* absdcl1  */
  YYSYMBOL_absdcl1_noea = 223,             /* absdcl1_noea  */
  YYSYMBOL_absdcl1_ea = 224,               /* absdcl1_ea  */
  YYSYMBOL_direct_absdcl1 = 225,           /* direct_absdcl1  */
  YYSYMBOL_array_declarator = 226,         /* array_declarator  */
  YYSYMBOL_stmts_and_decls = 227,          /* stmts_and_decls  */
  YYSYMBOL_lineno_stmt_decl_or_labels_ending_stmt = 228, /* lineno_stmt_decl_or_labels_ending_stmt  */
  YYSYMBOL_lineno_stmt_decl_or_labels_ending_decl = 229, /* lineno_stmt_decl_or_labels_ending_decl  */
  YYSYMBOL_lineno_stmt_decl_or_labels_ending_label = 230, /* lineno_stmt_decl_or_labels_ending_label  */
  YYSYMBOL_lineno_stmt_decl_or_labels_ending_error = 231, /* lineno_stmt_decl_or_labels_ending_error  */
  YYSYMBOL_lineno_stmt_decl_or_labels = 232, /* lineno_stmt_decl_or_labels  */
  YYSYMBOL_errstmt = 233,                  /* errstmt  */
  YYSYMBOL_pushlevel = 234,                /* pushlevel  */
  YYSYMBOL_poplevel = 235,                 /* poplevel  */
  YYSYMBOL_c99_block_start = 236,          /* c99_block_start  */
  YYSYMBOL_c99_block_end = 237,            /* c99_block_end  */
  YYSYMBOL_maybe_label_decls = 238,        /* maybe_label_decls  */
  YYSYMBOL_label_decls = 239,              /* label_decls  */
  YYSYMBOL_label_decl = 240,               /* label_decl  */
  YYSYMBOL_compstmt_or_error = 241,        /* compstmt_or_error  */
  YYSYMBOL_compstmt_start = 242,           /* compstmt_start  */
  YYSYMBOL_compstmt_nostart = 243,         /* compstmt_nostart  */
  YYSYMBOL_compstmt_contents_nonempty = 244, /* compstmt_contents_nonempty  */
  YYSYMBOL_compstmt_primary_start = 245,   /* compstmt_primary_start  */
  YYSYMBOL_compstmt = 246,                 /* compstmt  */
  YYSYMBOL_simple_if = 247,                /* simple_if  */
  YYSYMBOL_if_prefix = 248,                /* if_prefix  */
  YYSYMBOL_249_29 = 249,                   /* @29  */
  YYSYMBOL_do_stmt_start = 250,            /* do_stmt_start  */
  YYSYMBOL_251_30 = 251,                   /* @30  */
  YYSYMBOL_save_filename = 252,            /* save_filename  */
  YYSYMBOL_save_lineno = 253,              /* save_lineno  */
  YYSYMBOL_lineno_labeled_stmt = 254,      /* lineno_labeled_stmt  */
  YYSYMBOL_c99_block_lineno_labeled_stmt = 255, /* c99_block_lineno_labeled_stmt  */
  YYSYMBOL_lineno_stmt = 256,              /* lineno_stmt  */
  YYSYMBOL_lineno_label = 257,             /* lineno_label  */
  YYSYMBOL_select_or_iter_stmt = 258,      /* select_or_iter_stmt  */
  YYSYMBOL_259_31 = 259,                   /* $@31  */
  YYSYMBOL_260_32 = 260,                   /* @32  */
  YYSYMBOL_261_33 = 261,                   /* @33  */
  YYSYMBOL_262_34 = 262,                   /* @34  */
  YYSYMBOL_263_35 = 263,                   /* $@35  */
  YYSYMBOL_264_36 = 264,                   /* $@36  */
  YYSYMBOL_265_37 = 265,                   /* $@37  */
  YYSYMBOL_266_38 = 266,                   /* @38  */
  YYSYMBOL_for_init_stmt = 267,            /* for_init_stmt  */
  YYSYMBOL_stmt = 268,                     /* stmt  */
  YYSYMBOL_label = 269,                    /* label  */
  YYSYMBOL_maybe_type_qual = 270,          /* maybe_type_qual  */
  YYSYMBOL_xexpr = 271,                    /* xexpr  */
  YYSYMBOL_asm_operands = 272,             /* asm_operands  */
  YYSYMBOL_nonnull_asm_operands = 273,     /* nonnull_asm_operands  */
  YYSYMBOL_asm_operand = 274,              /* asm_operand  */
  YYSYMBOL_asm_clobbers = 275,             /* asm_clobbers  */
  YYSYMBOL_parmlist = 276,                 /* parmlist  */
  YYSYMBOL_277_39 = 277,                   /* $@39  */
  YYSYMBOL_parmlist_1 = 278,               /* parmlist_1  */
  YYSYMBOL_279_40 = 279,                   /* $@40  */
  YYSYMBOL_280_41 = 280,                   /* $@41  */
  YYSYMBOL_parmlist_2 = 281,               /* parmlist_2  */
  YYSYMBOL_parms = 282,                    /* parms  */
  YYSYMBOL_parm = 283,                     /* parm  */
  YYSYMBOL_firstparm = 284,                /* firstparm  */
  YYSYMBOL_setspecs_fp = 285,              /* setspecs_fp  */
  YYSYMBOL_parmlist_or_identifiers = 286,  /* parmlist_or_identifiers  */
  YYSYMBOL_287_42 = 287,                   /* $@42  */
  YYSYMBOL_parmlist_or_identifiers_1 = 288, /* parmlist_or_identifiers_1  */
  YYSYMBOL_identifiers = 289,              /* identifiers  */
  YYSYMBOL_identifiers_or_typenames = 290, /* identifiers_or_typenames  */
  YYSYMBOL_extension = 291                 /* extension  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */
#line 200 "c-parse.y"

/* Number of statements (loosely speaking) and compound statements 
   seen so far.  */
static int stmt_count;
static int compstmt_count;
  
/* Input file and line number of the end of the body of last simple_if;
   used by the stmt-rule immediately after simple_if returns.  */
static const char *if_stmt_file;
static int if_stmt_line;

/* List of types and structure classes of the current declaration.  */
static tree current_declspecs = NULL_TREE;
static tree prefix_attributes = NULL_TREE;

/* List of all the attributes applying to the identifier currently being
   declared; includes prefix_attributes and possibly some more attributes
   just after a comma.  */
static tree all_prefix_attributes = NULL_TREE;

/* Stack of saved values of current_declspecs, prefix_attributes and
   all_prefix_attributes.  */
static tree declspec_stack;

/* PUSH_DECLSPEC_STACK is called from setspecs; POP_DECLSPEC_STACK
   should be called from the productions making use of setspecs.  */
#define PUSH_DECLSPEC_STACK						 \
  do {									 \
    declspec_stack = tree_cons (build_tree_list (prefix_attributes,	 \
						 all_prefix_attributes), \
				current_declspecs,			 \
				declspec_stack);			 \
  } while (0)

#define POP_DECLSPEC_STACK						\
  do {									\
    current_declspecs = TREE_VALUE (declspec_stack);			\
    prefix_attributes = TREE_PURPOSE (TREE_PURPOSE (declspec_stack));	\
    all_prefix_attributes = TREE_VALUE (TREE_PURPOSE (declspec_stack));	\
    declspec_stack = TREE_CHAIN (declspec_stack);			\
  } while (0)

/* For __extension__, save/restore the warning flags which are
   controlled by __extension__.  */
#define SAVE_WARN_FLAGS()			\
	size_int (pedantic			\
		  | (warn_pointer_arith << 1)	\
		  | (warn_traditional << 2))

#define RESTORE_WARN_FLAGS(tval)		\
  do {						\
    int val = tree_low_cst (tval, 0);		\
    pedantic = val & 1;				\
    warn_pointer_arith = (val >> 1) & 1;	\
    warn_traditional = (val >> 2) & 1;		\
  } while (0)


#define OBJC_NEED_RAW_IDENTIFIER(VAL)	/* nothing */

/* Tell yyparse how to print a token's value, if yydebug is set.  */

#define YYPRINT(FILE,YYCHAR,YYLVAL) yyprint(FILE,YYCHAR,YYLVAL)

static void yyprint	  PARAMS ((FILE *, int, YYSTYPE));
static void yyerror	  PARAMS ((const char *));
static int yylexname	  PARAMS ((void));
static inline int _yylex  PARAMS ((void));
static int  yylex	  PARAMS ((void));
static void init_reswords PARAMS ((void));

/* Add GC roots for variables local to this file.  */
void
c_parse_init ()
{
  init_reswords ();

  ggc_add_tree_root (&declspec_stack, 1);
  ggc_add_tree_root (&current_declspecs, 1);
  ggc_add_tree_root (&prefix_attributes, 1);
  ggc_add_tree_root (&all_prefix_attributes, 1);
}


#line 623 "c-p12907.c"


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
typedef yytype_int16 yy_state_t;

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
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   3247

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  200
/* YYNRULES -- Number of rules.  */
#define YYNRULES  558
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  896

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   324


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
       2,     2,     2,    88,     2,     2,     2,    61,    52,     2,
      68,    84,    59,    57,    89,    58,    67,    60,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    47,    85,
       2,    45,     2,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    69,     2,    91,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    90,    50,    86,    87,     2,     2,     2,
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
      48,    49,    53,    54,    55,    56,    62,    63,    64,    65,
      66,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   287,   287,   291,   310,   310,   311,   311,   315,   316,
     317,   325,   330,   337,   339,   341,   343,   344,   345,   352,
     357,   351,   363,   366,   371,   365,   377,   380,   385,   379,
     391,   396,   397,   400,   402,   404,   409,   411,   413,   415,
     419,   425,   426,   430,   432,   437,   438,   441,   444,   448,
     465,   471,   474,   477,   480,   482,   487,   491,   495,   496,
     501,   502,   504,   506,   508,   510,   512,   514,   516,   518,
     520,   522,   524,   527,   526,   534,   533,   541,   545,   540,
     551,   550,   561,   568,   580,   586,   587,   589,   592,   591,
     604,   609,   611,   627,   634,   636,   639,   649,   659,   661,
     665,   671,   673,   679,   680,   697,   699,   700,   711,   716,
     717,   718,   719,   727,   729,   731,   734,   743,   752,   762,
     767,   769,   771,   773,   775,   777,   834,   837,   840,   846,
     852,   855,   861,   864,   870,   873,   876,   879,   882,   885,
     888,   894,   897,   900,   903,   906,   909,   915,   918,   921,
     924,   927,   930,   936,   939,   942,   945,   948,   954,   957,
     960,   963,   969,   975,   981,   990,   996,   999,  1002,  1008,
    1014,  1020,  1029,  1035,  1038,  1041,  1044,  1047,  1050,  1053,
    1059,  1065,  1071,  1080,  1083,  1086,  1089,  1092,  1098,  1101,
    1104,  1107,  1110,  1113,  1116,  1122,  1128,  1134,  1143,  1146,
    1149,  1152,  1155,  1162,  1163,  1164,  1165,  1166,  1167,  1168,
    1169,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1184,
    1185,  1186,  1187,  1191,  1192,  1193,  1194,  1198,  1199,  1200,
    1201,  1205,  1206,  1207,  1208,  1212,  1213,  1214,  1215,  1216,
    1217,  1218,  1219,  1223,  1224,  1225,  1226,  1227,  1228,  1229,
    1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,  1238,  1244,
    1245,  1271,  1272,  1276,  1280,  1282,  1286,  1290,  1294,  1296,
    1303,  1304,  1308,  1309,  1314,  1315,  1323,  1322,  1330,  1339,
    1338,  1346,  1355,  1356,  1361,  1363,  1368,  1373,  1375,  1381,
    1382,  1384,  1386,  1388,  1396,  1397,  1398,  1399,  1403,  1404,
    1410,  1412,  1411,  1415,  1422,  1424,  1428,  1429,  1435,  1438,
    1442,  1441,  1446,  1451,  1450,  1454,  1456,  1460,  1461,  1465,
    1470,  1474,  1480,  1492,  1479,  1510,  1522,  1509,  1542,  1543,
    1549,  1551,  1556,  1558,  1560,  1568,  1569,  1573,  1578,  1580,
    1584,  1589,  1591,  1593,  1595,  1603,  1608,  1610,  1612,  1614,
    1618,  1620,  1625,  1627,  1632,  1634,  1646,  1645,  1651,  1656,
    1655,  1659,  1664,  1663,  1669,  1668,  1676,  1678,  1680,  1688,
    1690,  1693,  1695,  1701,  1703,  1709,  1710,  1712,  1718,  1721,
    1731,  1734,  1739,  1741,  1747,  1748,  1753,  1754,  1759,  1762,
    1766,  1772,  1775,  1779,  1790,  1791,  1796,  1802,  1804,  1810,
    1809,  1818,  1819,  1824,  1827,  1831,  1838,  1839,  1843,  1844,
    1849,  1851,  1856,  1858,  1860,  1862,  1864,  1871,  1873,  1875,
    1877,  1880,  1891,  1892,  1893,  1897,  1901,  1902,  1903,  1904,
    1905,  1909,  1910,  1913,  1914,  1918,  1919,  1920,  1921,  1922,
    1926,  1927,  1931,  1932,  1933,  1934,  1937,  1941,  1948,  1953,
    1969,  1983,  1985,  1991,  1992,  1996,  2010,  2012,  2015,  2019,
    2021,  2029,  2030,  2034,  2051,  2059,  2064,  2077,  2076,  2091,
    2090,  2110,  2116,  2122,  2123,  2128,  2134,  2148,  2158,  2157,
    2165,  2177,  2188,  2191,  2187,  2197,  2200,  2203,  2207,  2210,
    2214,  2202,  2218,  2217,  2225,  2227,  2233,  2235,  2238,  2242,
    2245,  2248,  2251,  2254,  2258,  2262,  2267,  2271,  2283,  2289,
    2297,  2300,  2303,  2306,  2323,  2325,  2331,  2332,  2338,  2339,
    2343,  2344,  2349,  2351,  2358,  2360,  2371,  2370,  2381,  2383,
    2391,  2382,  2395,  2402,  2403,  2413,  2415,  2420,  2422,  2429,
    2434,  2439,  2442,  2448,  2456,  2461,  2466,  2469,  2475,  2481,
    2491,  2490,  2501,  2502,  2520,  2522,  2528,  2530,  2535
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
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "TYPENAME", "SCSPEC", "STATIC", "TYPESPEC", "TYPE_QUAL", "CONSTANT",
  "STRING", "ELLIPSIS", "SIZEOF", "ENUM", "STRUCT", "UNION", "IF", "ELSE",
  "WHILE", "DO", "FOR", "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE",
  "RETURN", "GOTO", "ASM_KEYWORD", "TYPEOF", "ALIGNOF", "ATTRIBUTE",
  "EXTENSION", "LABEL", "REALPART", "IMAGPART", "VA_ARG", "CHOOSE_EXPR",
  "TYPES_COMPATIBLE_P", "PTR_VALUE", "PTR_BASE", "PTR_EXTENT",
  "STRING_FUNC_NAME", "VAR_FUNC_NAME", "ASSIGN", "'='", "'?'", "':'",
  "OROR", "ANDAND", "'|'", "'^'", "'&'", "EQCOMPARE", "ARITHCOMPARE",
  "LSHIFT", "RSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY",
  "PLUSPLUS", "MINUSMINUS", "HYPERUNARY", "POINTSAT", "'.'", "'('", "'['",
  "INTERFACE", "IMPLEMENTATION", "END", "SELECTOR", "DEFS", "ENCODE",
  "CLASSNAME", "PUBLIC", "PRIVATE", "PROTECTED", "PROTOCOL", "OBJECTNAME",
  "CLASS", "ALIAS", "')'", "';'", "'}'", "'~'", "'!'", "','", "'{'", "']'",
  "$accept", "program", "extdefs", "@1", "@2", "extdef", "datadef",
  "fndef", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "identifier", "unop",
  "expr", "exprlist", "nonnull_exprlist", "unary_expr", "sizeof",
  "alignof", "cast_expr", "expr_no_commas", "$@9", "$@10", "$@11", "$@12",
  "$@13", "primary", "$@14", "string", "old_style_parm_decls",
  "lineno_datadecl", "datadecls", "datadecl", "lineno_decl", "setspecs",
  "maybe_resetattrs", "decl", "declspecs_nosc_nots_nosa_noea",
  "declspecs_nosc_nots_nosa_ea", "declspecs_nosc_nots_sa_noea",
  "declspecs_nosc_nots_sa_ea", "declspecs_nosc_ts_nosa_noea",
  "declspecs_nosc_ts_nosa_ea", "declspecs_nosc_ts_sa_noea",
  "declspecs_nosc_ts_sa_ea", "declspecs_sc_nots_nosa_noea",
  "declspecs_sc_nots_nosa_ea", "declspecs_sc_nots_sa_noea",
  "declspecs_sc_nots_sa_ea", "declspecs_sc_ts_nosa_noea",
  "declspecs_sc_ts_nosa_ea", "declspecs_sc_ts_sa_noea",
  "declspecs_sc_ts_sa_ea", "declspecs_ts", "declspecs_nots",
  "declspecs_ts_nosa", "declspecs_nots_nosa", "declspecs_nosc_ts",
  "declspecs_nosc_nots", "declspecs_nosc", "declspecs",
  "maybe_type_quals_attrs", "typespec_nonattr", "typespec_attr",
  "typespec_reserved_nonattr", "typespec_reserved_attr",
  "typespec_nonreserved_nonattr", "initdecls", "notype_initdecls",
  "maybeasm", "initdcl", "@15", "notype_initdcl", "@16", "maybe_attribute",
  "attributes", "attribute", "attribute_list", "attrib", "any_word",
  "scspec", "init", "$@17", "initlist_maybe_comma", "initlist1", "initelt",
  "$@18", "initval", "$@19", "designator_list", "designator",
  "nested_function", "$@20", "$@21", "notype_nested_function", "$@22",
  "$@23", "declarator", "after_type_declarator", "parm_declarator",
  "parm_declarator_starttypename", "parm_declarator_nostarttypename",
  "notype_declarator", "struct_head", "union_head", "enum_head",
  "structsp_attr", "@24", "@25", "@26", "@27", "structsp_nonattr",
  "maybecomma", "maybecomma_warn", "component_decl_list",
  "component_decl_list2", "component_decl", "components",
  "components_notype", "component_declarator",
  "component_notype_declarator", "enumlist", "enumerator", "typename",
  "@28", "absdcl", "absdcl_maybe_attribute", "absdcl1", "absdcl1_noea",
  "absdcl1_ea", "direct_absdcl1", "array_declarator", "stmts_and_decls",
  "lineno_stmt_decl_or_labels_ending_stmt",
  "lineno_stmt_decl_or_labels_ending_decl",
  "lineno_stmt_decl_or_labels_ending_label",
  "lineno_stmt_decl_or_labels_ending_error", "lineno_stmt_decl_or_labels",
  "errstmt", "pushlevel", "poplevel", "c99_block_start", "c99_block_end",
  "maybe_label_decls", "label_decls", "label_decl", "compstmt_or_error",
  "compstmt_start", "compstmt_nostart", "compstmt_contents_nonempty",
  "compstmt_primary_start", "compstmt", "simple_if", "if_prefix", "@29",
  "do_stmt_start", "@30", "save_filename", "save_lineno",
  "lineno_labeled_stmt", "c99_block_lineno_labeled_stmt", "lineno_stmt",
  "lineno_label", "select_or_iter_stmt", "$@31", "@32", "@33", "@34",
  "$@35", "$@36", "$@37", "@38", "for_init_stmt", "stmt", "label",
  "maybe_type_qual", "xexpr", "asm_operands", "nonnull_asm_operands",
  "asm_operand", "asm_clobbers", "parmlist", "$@39", "parmlist_1", "$@40",
  "$@41", "parmlist_2", "parms", "parm", "firstparm", "setspecs_fp",
  "parmlist_or_identifiers", "$@42", "parmlist_or_identifiers_1",
  "identifiers", "identifiers_or_typenames", "extension", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-648)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-534)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      50,    58,   115,  2840,  -648,  2840,    90,  -648,  -648,  -648,
    -648,  -648,    96,    96,    96,    62,   114,   123,  -648,  -648,
    -648,  -648,  -648,   234,   771,   481,  1597,  1342,  1005,  1100,
    1182,  1402,  1685,  1852,  2018,  2259,  1880,  1601,  1949,  1689,
    -648,  -648,   133,  -648,  -648,  -648,  -648,  -648,    96,  -648,
    -648,    80,   146,   151,  -648,  -648,  2840,  -648,  -648,  -648,
      96,    96,    96,  2630,  2548,   159,  -648,   165,    96,   302,
    -648,   940,  -648,  -648,  -648,    96,  -648,  -648,  -648,  -648,
    -648,  -648,  -648,  -648,    96,  -648,  -648,  -648,  -648,  -648,
    -648,  -648,  -648,    96,  -648,  -648,  -648,  -648,  -648,  -648,
    -648,  -648,    96,  -648,  -648,  -648,  -648,  -648,  -648,  -648,
    -648,    96,  -648,  -648,  -648,  -648,  -648,  -648,  -648,  -648,
      96,  -648,  -648,  -648,  -648,  -648,  -648,  -648,  -648,    96,
    -648,  -648,  -648,  -648,  -648,  -648,  -648,  -648,    96,  -648,
    -648,  -648,  -648,  -648,   280,   234,  -648,  -648,  -648,  -648,
    -648,   153,  -648,   156,  -648,   169,  -648,  -648,  -648,  -648,
    -648,  -648,  2630,  2630,   206,   221,   228,  -648,   358,  -648,
    -648,  -648,  2630,  -648,  -648,  1774,  -648,  -648,  2630,   230,
     232,  -648,  2692,  2733,  -648,  3157,  1084,   335,  1219,  2630,
     297,  2677,   560,  2821,  2232,   362,   602,   708,   721,  -648,
     300,  1314,   224,   409,   292,   411,  -648,   234,   234,    96,
    -648,    96,  -648,   333,    96,   260,   985,    96,  -648,  -648,
     165,    96,   354,  -648,  2136,   397,   471,   364,  2107,   350,
    1435,  -648,   360,  -648,   504,  -648,  -648,  -648,  2630,  2630,
    3112,  -648,  -648,   368,  -648,   371,   406,  -648,   413,  2630,
    1774,  -648,  1774,  -648,  2630,  2630,   497,  -648,  -648,  2630,
    2630,  2630,  2630,  2630,  2630,  2630,  2630,  2630,  2630,  2630,
    2630,  -648,  -648,   358,   358,  2630,  2630,  -648,   464,  -648,
     518,   473,  -648,  -648,   382,  -648,  -648,  -648,  -648,   217,
    -648,   493,  -648,   471,   290,   234,  -648,   553,  -648,  -648,
     165,   571,  2150,   498,  -648,   893,    66,  -648,  -648,   545,
     280,   280,  -648,    96,  -648,   985,    96,    96,  -648,  -648,
    -648,   985,    96,  -648,  -648,  2677,   560,  2821,  2232,   362,
     602,   708,   721,  -648,   470,   510,  2405,  -648,    96,  -648,
    -648,   554,   519,  -648,   504,  2918,  2944,   529,  -648,  -648,
    2420,  -648,  3157,   548,   552,  3157,  3157,  2630,   593,  2630,
    2630,  1065,  2857,   879,   641,   917,   614,   614,   472,   472,
    -648,  -648,  -648,  -648,  -648,   557,   232,   556,  -648,   358,
    1532,   518,  -648,  -648,   165,    96,  -648,  -648,  -648,  -648,
     517,  -648,   559,  1314,  2774,  -648,  -648,   242,    34,  1285,
    2630,  2630,  2219,  -648,   562,  -648,  -648,  -648,  -648,  -648,
     819,  -648,   397,   330,   280,  -648,   600,  -648,  -648,  -648,
    -648,  -648,  -648,  -648,   565,  -648,   573,  2630,   358,   574,
     519,  3112,  2630,  3112,  -648,  -648,   567,   567,   617,  2630,
    3186,  3005,  -648,  -648,  -648,   369,   498,  -648,  -648,    88,
      99,   103,   106,   678,  -648,   575,  -648,  -648,  -648,  -648,
     382,   382,  -648,    96,  -648,  -648,  -648,   279,   601,  -648,
     603,  -648,  -648,   831,  2498,   676,   456,  1455,  3124,  1748,
     522,  -648,  -648,  -648,   608,   383,  -648,  -648,   319,   613,
     620,  -648,  -648,  -648,  -648,   623,   628,  1978,  -648,  -648,
     677,  -648,  -648,  -648,   630,  -648,  -648,   631,  -648,  -648,
      96,    96,  3157,  -648,    96,   638,   648,  2964,   649,  1840,
    -648,  3173,  -648,   358,  -648,  -648,  -648,  -648,  -648,  -648,
    -648,  -648,  -648,  -648,  -648,  -648,  -648,  -648,  2289,  -648,
    -648,   653,  1329,  -648,  -648,  -648,  2630,  -648,  -648,  -648,
     257,   125,  -648,  -648,  2890,  -648,   722,  -648,  -648,    68,
    -648,   280,  -648,   234,  -648,  -648,  3157,  -648,  -648,  1978,
    -648,    96,   343,    96,   110,  -648,  -648,  -648,    96,  -648,
    2630,  -648,  -648,   691,   358,  2630,  -648,   693,  3157,   655,
     657,  -648,  -648,   328,  1388,  2630,  -648,  2358,  -648,   697,
    2630,   700,   670,   680,  2589,   163,   758,  -648,  -648,  -648,
     682,  -648,  -648,  -648,   683,   785,   687,  -648,  -648,  -648,
    2486,  -648,  -648,   332,  -648,   165,    96,    96,   537,   544,
     209,  -648,  -648,    96,   165,    96,   209,  -648,    96,  -648,
     831,  2498,  3081,  3141,   676,   456,  1814,  1168,  1455,  3124,
    3093,  3153,  1748,   522,  2056,  1517,  -648,  -648,  -648,  -648,
     684,  -648,  -648,   393,   395,  1840,    68,  -648,    68,  -648,
    2630,   141,  -648,  2630,   303,  -648,  2986,  -648,  2829,  1840,
    -648,  -648,  1909,  -648,  2042,  -648,  -648,  3173,  2882,  -648,
    -648,  -648,  -648,   695,  2630,   696,  -648,   714,  -648,  -648,
     280,   234,  -648,  -648,  -648,  -648,  -648,   715,   774,  1620,
      78,  -648,  -648,  -648,  -648,   257,   212,  -648,    96,  -648,
      96,  -648,  -648,    96,   125,   125,  -648,  -648,   257,   125,
    -648,  -648,  -648,   702,  -648,  -648,  -648,  -648,  3023,  2630,
    -648,  -648,  3023,  2630,  -648,  -648,  2630,  -648,   706,  2042,
    -648,  -648,  2630,  -648,  -648,   709,  -648,  2630,   750,   408,
    -648,   321,   415,  -648,   355,   730,   739,  -648,   741,  2630,
    1708,  -648,  -648,  -648,  -648,  2630,  -648,   537,   544,   344,
    -648,  -648,  1329,    96,   209,  -648,   209,  -648,  -648,   343,
    -648,  3023,  -648,  3023,  2900,  -648,  -648,  3139,  -648,    75,
      96,  -648,   985,  -648,   985,  2630,  2630,   792,  2486,   727,
    -648,  -648,  -648,  -648,  -648,   731,  -648,  -648,  -648,  -648,
    -648,  -648,  -648,  -648,  -648,    82,   729,  -648,  -648,  -648,
     733,   738,  -648,  -648,  -648,  -648,   736,  -648,  -648,  -648,
    -648,   744,   762,   358,    77,   751,  -648,  -648,  -648,  -648,
    -648,  -648,  2630,  -648,  -648,  -648,  2630,   756,    82,   764,
      82,  -648,  -648,  -648,   767,  -648,   757,   843,   121,  -648,
    -648,   684,   684,  -648,  -648,  -648,   787,   553,   773,  -648,
    -648,  2630,  2630,   335,   345,  -648,   775,   777,   784,   553,
    -648,  -648,  -648,   335,  -648,  -648
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     6,     0,     1,     0,     0,   267,   299,   298,
     264,   126,   354,   350,   352,     0,     0,     0,   558,    18,
       5,     9,     8,     0,   211,   212,   213,   214,   203,   204,
     205,   206,   215,   216,   217,   218,   207,   208,   209,   210,
     118,   118,     0,   134,   141,   261,   263,   262,   132,   284,
     158,     0,     0,     0,   266,   265,     0,     7,    16,    17,
     355,   351,   353,     0,     0,     0,   349,   259,   282,     0,
     272,     0,   127,   139,   145,   129,   161,   128,   140,   146,
     162,   130,   151,   156,   133,   168,   131,   152,   157,   169,
     135,   137,   143,   142,   179,   136,   138,   144,   180,   147,
     149,   154,   153,   194,   148,   150,   155,   195,   159,   177,
     186,   165,   163,   160,   178,   187,   164,   166,   192,   201,
     172,   170,   167,   193,   202,   171,   173,   175,   184,   183,
     181,   174,   176,   185,   182,   188,   190,   199,   198,   196,
     189,   191,   200,   197,     0,     0,    15,   285,    31,    32,
     375,   366,   375,   367,   364,   368,    11,    84,    85,   103,
      56,    57,     0,     0,     0,     0,     0,    87,     0,    33,
      35,    34,     0,    36,    37,     0,    38,    39,     0,     0,
      40,    58,     0,     0,    60,    43,    45,    86,     0,     0,
       0,   239,   240,   241,   242,   235,   236,   237,   238,   399,
       0,   289,   231,   232,   233,   234,   260,     0,     0,   283,
      12,   282,    30,     0,   282,   259,     0,   282,   348,   334,
     259,   282,     0,   270,     0,   328,   329,     0,     0,     0,
       0,   356,     0,   359,     0,   362,    54,    55,     0,     0,
       0,    49,    46,     0,   463,     0,     0,    48,     0,     0,
       0,    50,     0,    52,     0,     0,    77,    75,    73,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,   102,     0,     0,    41,     0,   104,     0,   459,
     451,     0,    47,   268,   401,   269,   296,   297,   294,     0,
     287,   290,   295,   347,     0,     0,   119,     0,   550,   345,
     259,   260,     0,     0,    28,     0,   471,   110,   472,   281,
       0,     0,    14,   282,    22,     0,   282,   282,   332,    13,
      26,     0,   282,   382,   377,   231,   232,   233,   234,   227,
     228,   229,   230,   118,   118,   374,     0,   375,   282,   375,
     396,   397,   371,   394,     0,     0,     0,     0,    91,    90,
       0,    10,    44,     0,     0,    83,    82,     0,     0,     0,
       0,    71,    72,    70,    69,    68,    66,    67,    61,    62,
      63,    64,    65,   100,    99,     0,    42,     0,    93,     0,
       0,   452,   453,    92,   259,   282,   400,   402,   407,   406,
     408,   416,     0,   289,    41,   346,   273,   274,     0,     0,
       0,     0,     0,   418,     0,   446,   471,   112,   107,   111,
       0,   279,   333,     0,     0,    20,   278,   331,    24,   358,
     471,   471,   376,   383,     0,   361,     0,     0,   372,     0,
     371,     0,     0,     0,    88,    59,    51,    53,     0,     0,
      76,    74,    94,    98,   556,     0,   462,   431,   461,   471,
     471,   471,   471,     0,   440,     0,   472,   426,   435,   454,
     410,   526,   415,   282,   414,   286,   288,    84,     0,   275,
       0,   554,   534,   223,   224,   219,   220,   225,   226,   221,
     222,   118,   118,   552,     0,   535,   537,   551,     0,     0,
       0,   419,   417,   472,   108,   118,   118,     0,   330,   271,
     274,   471,   276,   471,   378,   384,   472,   380,   386,   472,
     282,   282,   398,   395,   282,     0,     0,     0,     0,     0,
      78,    81,   455,     0,   432,   427,   436,   433,   428,   437,
     472,   429,   438,   434,   430,   439,   441,   448,   449,   409,
     411,     0,     0,   526,   413,   291,     0,   293,   532,   549,
     403,   403,   528,   529,     0,   553,     0,   420,   421,     0,
     115,     0,   116,     0,   303,   301,   300,   280,   472,     0,
     472,   282,   379,   282,     0,   357,   360,   365,   282,    95,
       0,    97,   316,    84,     0,     0,   313,     0,   315,     0,
     369,   306,   312,     0,     0,     0,   557,   449,   460,   267,
       0,     0,     0,     0,     0,     0,   514,   509,   458,   471,
       0,   117,   118,   118,     0,     0,   447,   496,   476,   477,
       0,   412,   527,     0,   339,   259,   282,   282,   335,   336,
     282,   546,   404,   407,   259,   282,   282,   548,   282,   536,
     211,   212,   213,   214,   203,   204,   205,   206,   215,   216,
     217,   218,   207,   208,   209,   210,   118,   118,   538,   555,
       0,    29,   456,     0,     0,     0,     0,   277,     0,   471,
       0,   282,   471,     0,   282,   363,     0,   319,     0,     0,
     310,    89,     0,   305,     0,   318,   309,    79,     0,   512,
     499,   500,   501,     0,     0,     0,   515,     0,   472,   497,
       0,     0,   124,   467,   482,   469,   487,     0,   480,     0,
       0,   450,   464,   125,   292,   410,   526,   544,   282,   338,
     282,   341,   545,   405,   410,   526,   547,   530,   403,   403,
     457,   113,   114,     0,    21,    25,   385,   472,   282,     0,
     388,   387,   282,     0,   391,    96,     0,   321,     0,     0,
     307,   308,     0,   510,   502,     0,   507,     0,     0,     0,
     122,   322,     0,   123,   325,     0,     0,   449,     0,     0,
       0,   466,   471,   465,   486,     0,   498,   342,   343,     0,
     337,   340,     0,   282,   282,   541,   282,   543,   302,     0,
     390,   282,   393,   282,     0,   314,   311,     0,   508,     0,
     282,   120,     0,   121,     0,     0,     0,     0,   516,     0,
     481,   449,   450,   473,   471,     0,   344,   531,   539,   540,
     542,   389,   392,   320,   511,   518,     0,   513,   323,   326,
       0,     0,   470,   517,   495,   488,     0,   492,   479,   475,
     474,     0,     0,     0,     0,   519,   520,   503,   471,   471,
     468,   483,   516,   494,   449,   485,     0,     0,   518,     0,
       0,   472,   472,   449,     0,   493,     0,     0,     0,   504,
     521,     0,     0,   484,   489,   522,     0,     0,     0,   324,
     327,   516,     0,   524,     0,   505,     0,     0,     0,     0,
     490,   523,   506,   525,   449,   491
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -648,  -648,  -648,  -648,  -648,   107,  -648,  -648,  -648,  -648,
    -648,  -648,  -648,  -648,    81,  -648,   -63,   476,  -254,   439,
    -648,  -648,   -75,   834,  -648,  -648,  -648,  -648,  -648,  -648,
    -648,  -295,  -301,   576,  -648,  -648,   120,    97,  -286,  -577,
       5,     8,    42,    43,    46,    49,     0,    60,  -377,  -340,
     317,   329,  -325,  -309,   334,   338,  -501,  -499,   479,   483,
    -648,  -124,  -648,  -509,  -158,   725,   737,   836,   848,  -648,
    -520,  -130,  -212,   482,  -648,   607,  -648,   285,     4,    33,
    -648,   502,  -648,   487,   336,  -648,  -285,  -648,   227,  -648,
    -559,  -648,  -648,   310,  -648,  -648,  -648,  -648,  -648,  -648,
    -140,   316,   182,   198,   -81,     2,  -648,  -648,  -648,  -648,
    -648,  -648,  -648,  -648,  -648,  -648,   484,   -72,  -648,   589,
    -648,  -648,   258,   254,   598,   515,  -109,  -648,  -648,  -525,
    -278,  -364,  -429,  -648,   430,  -648,  -648,  -648,  -648,  -648,
    -648,  -232,  -648,  -648,  -386,   137,  -648,  -648,   569,  -336,
    -648,   341,  -648,  -648,  -515,  -648,  -648,  -648,  -648,  -648,
     660,  -370,   138,  -647,  -199,  -132,  -648,  -648,  -648,  -648,
    -648,  -648,  -648,  -648,  -648,  -648,  -648,  -648,  -648,  -583,
      93,  -648,    98,  -648,   507,  -648,  -518,  -648,  -648,  -648,
    -648,  -648,  -648,   500,  -298,  -648,  -648,  -648,  -648,    73
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,     3,     5,    20,    21,    22,   315,   501,
     321,   503,   216,   406,   587,   178,   245,   375,   180,   181,
     182,   183,   184,   185,   360,   359,   357,   595,   358,   186,
     519,   187,   304,   305,   306,   494,   447,    23,   295,   611,
     191,   192,   193,   194,   195,   196,   197,   198,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,   481,   482,
     333,   206,   199,    42,   207,    43,    44,    45,    46,    47,
     222,    69,   217,   223,   569,    70,   497,   296,   209,    49,
     289,   290,   291,    50,   567,   665,   589,   590,   591,   749,
     592,   679,   593,   594,   760,   802,   848,   763,   804,   849,
     500,   225,   627,   628,   629,   226,    51,    52,    53,    54,
     337,   339,   344,   234,    55,   683,   429,   229,   230,   335,
     504,   507,   505,   508,   342,   343,   200,   284,   386,   631,
     632,   388,   389,   390,   218,   448,   449,   450,   451,   452,
     453,   307,   280,   598,   772,   776,   380,   381,   382,   661,
     616,   281,   455,   188,   662,   708,   709,   765,   710,   767,
     308,   410,   812,   773,   813,   814,   711,   811,   766,   863,
     768,   852,   881,   894,   854,   835,   618,   619,   697,   836,
     844,   845,   846,   884,   462,   542,   483,   638,   782,   484,
     485,   658,   486,   550,   299,   399,   487,   488,   445,   189
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     179,   190,   398,    30,   224,    30,   387,    48,    24,    48,
      24,    25,   316,    25,   415,   227,    60,    61,    62,   417,
     418,   376,   477,   617,   622,    71,   637,   414,    75,   614,
      84,   540,    93,   477,   102,   686,   111,   612,   120,   613,
     129,   663,   138,   713,   277,    26,    27,    26,    27,    28,
      -2,    28,    29,   656,    29,   657,    30,   302,     4,   478,
      48,    24,   310,    31,    25,    31,   246,  -106,    48,   660,
     478,    48,   202,   407,   479,   203,    56,   408,    56,   774,
     232,   147,   617,   148,   149,   479,   538,   236,   237,  -442,
     480,   301,   842,   147,   147,   147,   539,   242,    26,    27,
    -443,   480,    28,   247,  -444,    29,   334,  -445,   147,   204,
     205,   614,    57,    66,   282,    -3,    31,   147,   469,   612,
     807,   613,   825,   559,   858,   751,   147,    17,    66,    56,
      63,   347,   151,   153,   155,   147,   572,   144,   145,   574,
     376,   353,   400,   354,   147,   730,   775,   228,   454,   148,
     149,   843,   615,   147,   148,   149,  -106,   673,   608,   826,
     597,   859,   147,   156,   838,   477,   148,   149,   877,    67,
     150,   147,    17,    11,  -422,    58,    59,   648,    68,    48,
     759,   457,    64,   541,   634,  -423,   633,   633,   739,  -424,
     796,    65,  -425,   635,   215,    75,    17,    84,   666,    93,
     668,   102,   478,   785,   787,   878,    75,   865,    84,   293,
     294,   615,   334,   377,   649,    66,   873,   479,   146,    48,
     202,   536,   694,   203,    48,   202,   460,   201,   203,   652,
     331,   834,    72,   480,    48,   325,   152,    66,   326,   404,
      17,   154,   147,   231,    48,   653,   233,   895,   458,   241,
     525,   528,   531,   534,    48,    17,    48,   204,   205,   235,
      66,   624,   204,   205,   817,   424,   300,   426,    11,   864,
     213,   625,   327,   328,   238,   435,   329,   214,   215,   330,
     626,   215,   288,    66,   219,   669,   540,   672,   316,   239,
     332,    17,   623,    67,   438,   540,   240,   397,   886,   614,
      81,   392,    68,   336,    48,   202,   393,   612,   203,   613,
     214,   215,   293,   294,   248,   341,   625,   526,   529,   532,
     535,   249,   516,    17,   518,   626,   215,   242,   758,    75,
     734,    84,   735,    93,    17,   102,   331,   489,   490,   220,
      48,   325,   204,   205,   326,   277,    66,   219,   221,   213,
     743,   539,  -274,   208,   373,   374,   879,   880,   214,   215,
     539,   148,   149,   545,   633,   633,  -274,   789,   546,    10,
      90,   214,   215,   684,   395,    12,    13,    14,   327,   328,
     733,   283,   329,   213,   285,   330,  -274,   210,    48,   202,
     670,   211,   203,    17,   748,   584,   332,   585,   317,   215,
    -274,   297,   220,   555,   473,   477,  -274,   474,   556,   336,
    -274,   221,   720,   215,   498,   473,   714,    77,   474,    86,
     780,   249,   781,   214,   215,   341,   204,   205,   816,   888,
     420,   421,   671,   664,   889,    48,   322,    48,   541,   312,
    -274,   384,   478,   313,  -274,   475,   338,   541,   476,   319,
     385,   215,   348,   211,   522,   349,   475,   479,   523,   476,
     444,     8,     9,    10,    95,   317,   215,   715,   553,    12,
      13,    14,   554,   480,   288,   610,   724,    75,   731,    93,
     732,   111,   313,   129,   211,     7,     8,     9,    10,    77,
     350,   871,   872,   801,    12,    13,    14,   313,   351,   298,
     803,   828,   309,   829,   211,   340,   311,   148,   149,   341,
      16,    76,    80,    85,    89,    94,    98,   103,   107,   112,
     116,   121,   125,   130,   134,   139,   143,     8,     9,    10,
     131,   268,   269,   270,   610,    12,    13,    14,    30,   214,
     215,   693,    48,    24,   -80,   282,    25,   473,   378,   316,
     474,   379,   630,   636,   646,  -381,  -381,   383,    48,   640,
     761,   394,   641,   159,     7,   397,  -244,    10,    77,   524,
     527,   762,   533,    12,    13,    14,   674,   401,   549,   549,
      26,    27,   883,   405,    28,   463,   215,    29,   475,    16,
     411,   476,   561,   563,   893,   422,   642,   643,    31,   427,
     644,   416,   298,   645,   596,   718,   215,   419,   428,    10,
      95,   620,   720,   215,   647,    12,    13,    14,   433,   609,
      30,   251,   253,   425,    48,    24,   412,   413,    25,    48,
     202,   755,   436,   203,   778,   779,   437,   723,    48,   202,
     439,   442,   203,   465,    75,   502,    84,   443,    93,   671,
     102,   510,   111,   492,   120,   318,   129,   434,   138,   511,
     514,   537,    26,    27,   520,   677,    28,   204,   205,    29,
     461,   266,   267,   268,   269,   270,   204,   205,   609,   303,
      31,     8,     9,    10,    90,   547,   695,   548,   292,    12,
      13,    14,   552,   620,   799,   263,   264,   265,   266,   267,
     268,   269,   270,   764,   557,   213,   809,    17,   560,   700,
     701,   558,   815,   562,   391,    10,    99,   293,   294,   571,
     573,    12,    13,    14,   578,   659,   293,   294,    10,   104,
     784,   786,   579,   581,    12,    13,    14,   621,   -31,    17,
     680,   681,   830,   831,   -32,   833,   682,   689,   543,    73,
      78,    82,    87,   728,   729,   690,   147,   109,   114,   118,
     123,    74,    79,    83,    88,   691,   696,   699,   702,   110,
     115,   119,   124,   279,   608,     7,     8,     9,    10,    72,
     754,   756,   757,   769,    12,    13,    14,   473,   788,   833,
     474,   770,   795,   866,   798,   575,   576,   800,   805,   577,
      16,   703,    17,   704,   705,   706,   707,   806,    30,   808,
     832,   837,    48,    24,   847,   841,    25,   850,   833,   887,
     464,   853,   851,     7,     8,     9,    10,    11,   475,   855,
     856,   476,    12,    13,    14,     7,     8,     9,    10,    72,
     860,   875,   318,   318,    12,    13,    14,   867,    16,   869,
      26,    27,   874,   876,    28,   882,  -243,    29,   885,   890,
      16,   891,    17,   675,    91,    96,   100,   105,    31,   892,
     468,   650,   127,   132,   136,   141,    92,    97,   101,   106,
     292,   620,   409,   651,   128,   133,   137,   142,   654,   495,
     391,   391,   655,   496,   303,   466,   499,  -109,  -109,  -109,
    -109,  -109,   396,   685,  -109,   667,  -109,  -109,  -109,   750,
     783,   716,   717,   777,   515,   722,    73,    78,    82,    87,
     725,   726,  -109,   727,   857,   423,   741,   736,    74,    79,
      83,    88,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   212,   430,   513,   -27,   -27,   -27,   -27,   -27,   839,
     459,   868,   840,   -27,   -27,   -27,   740,   712,   870,   744,
      76,    80,    94,    98,   112,   116,   130,   134,   213,   -27,
     544,  -274,   264,   265,   266,   267,   268,   269,   270,     0,
     391,   391,   551,  -109,     0,  -274,   303,     0,     0,  -471,
    -471,  -471,  -471,  -471,     0,     0,     0,     0,  -471,  -471,
    -471,     0,     0,   298,     0,   298,     0,     0,   214,   215,
       8,     9,    10,    90,  -471,     0,     0,     0,    12,    13,
      14,     0,     0,   790,     0,  -274,     0,   792,     0,  -274,
     -27,    91,    96,   100,   105,     0,    17,     0,     0,     0,
     456,     0,     0,    92,    97,   101,   106,     0,     0,     0,
      73,    78,    82,    87,     0,     0,     0,     0,   719,   721,
       0,     0,    74,    79,    83,    88,   493,     0,   818,   819,
       0,   820,   345,   346,     0,  -105,   821,     0,   822,     0,
     506,   509,     0,   352,     0,   827,     0,     0,   355,   356,
    -247,     0,     0,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,     8,     9,    10,    95,   456,
     456,   530,   456,    12,    13,    14,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,    76,    80,    85,
      89,    94,    98,   103,   107,   112,   116,   121,   125,   130,
     134,   139,   143,     0,     0,   391,   391,   271,   272,     0,
     273,   274,   275,   276,   391,   391,     0,     0,   391,   391,
       0,   568,     0,   570,     0,    91,    96,   100,   105,     0,
       0,     0,     0,     8,     9,    10,   104,    92,    97,   101,
     106,    12,    13,    14,     0,  -248,     0,     8,     9,    10,
      99,     0,     0,   440,   441,    12,    13,    14,    73,    78,
       0,     0,   109,   114,     0,     0,     0,   719,   721,   721,
      74,    79,     0,    17,   110,   115,     0,     0,     0,     0,
     278,     0,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,
       0,  -447,  -447,  -447,  -447,  -447,     0,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,     0,     0,
       0,   512,  -447,     0,     0,     0,   517,  -249,  -447,   698,
       0,  -447,     0,   521,     0,     0,  -447,  -447,  -447,     0,
       0,     0,  -447,  -447,     0,     0,   470,  -447,   471,     7,
       8,     9,    10,    11,     0,     0,   472,     0,    12,    13,
      14,     0,     0,     0,  -447,   279,  -447,  -447,     0,  -447,
       0,    91,    96,     0,    16,   127,   132,   148,   149,     8,
       9,   286,   287,    92,    97,     0,     0,   128,   133,   737,
     470,   566,   509,     7,     8,     9,    10,    11,     0,     0,
     472,     0,    12,    13,    14,     0,     7,     8,     9,    10,
      86,     0,     0,   588,     0,    12,    13,    14,    16,     0,
       0,     0,     0,     0,     0,    73,    78,    82,    87,  -533,
       0,    16,     0,   109,   114,   118,   123,    74,    79,    83,
      88,     0,     0,     0,     0,   110,   115,   119,   124,   582,
       0,   157,     0,     0,     0,     0,     0,   158,   159,     0,
     160,     0,     0,   566,     0,     0,     0,     8,     9,    10,
     104,     0,     0,  -533,   676,    12,    13,    14,   161,   678,
      18,     0,   162,   163,   164,   165,   166,  -246,   588,   687,
       0,   167,   530,  -317,   688,     0,   323,   168,     0,     7,
     169,     0,    10,    11,     0,   170,   171,   172,    12,    13,
      14,   173,   174,     0,     0,  -317,   175,  -317,     0,     7,
       8,     9,    10,   108,    16,     0,    17,    18,    12,    13,
      14,     0,     0,     0,   530,   176,   177,     0,   586,     0,
      91,    96,   100,   105,    16,     0,    17,  -250,   127,   132,
     136,   141,    92,    97,   101,   106,     0,     0,     0,   588,
     128,   133,   137,   142,   738,     0,     0,   742,   861,   862,
       0,     0,     0,   588,     0,     0,   588,     0,   588,     0,
     324,  -373,     8,     9,    10,   140,     0,     0,     0,     0,
      12,    13,    14,   446,     0,  -471,  -471,  -471,  -471,  -471,
    -471,  -471,  -471,     0,  -471,  -471,  -471,  -471,  -471,     0,
    -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,  -471,
    -471,  -471,  -471,  -471,  -471,     0,  -471,  -471,  -471,  -471,
    -471,     0,     0,   791,     0,  -471,     0,   793,     0,     0,
     794,  -471,     0,   588,  -471,     0,   797,     0,     0,  -471,
    -471,  -471,     0,     0,     0,  -471,  -471,     0,     0,     0,
    -471,     7,     8,     9,    10,    81,     8,     9,    10,   131,
      12,    13,    14,     0,    12,    13,    14,  -471,     0,  -471,
    -471,   771,  -471,  -449,  -449,     0,    16,     0,    17,  -449,
    -449,     0,  -449,     0,     0,     0,  -449,     0,  -449,  -449,
    -449,  -449,  -449,  -449,  -449,  -449,  -449,  -449,  -449,     0,
    -449,     0,  -449,     0,  -449,  -449,  -449,  -449,  -449,     0,
       0,     0,     0,  -449,     0,     0,     0,     0,     0,  -449,
       0,     0,  -449,     0,     0,     0,     0,  -449,  -449,  -449,
       0,     0,  -245,  -449,  -449,     0,  -256,     0,  -449,     7,
       8,     9,    10,   108,     8,     9,    10,   140,    12,    13,
      14,     0,    12,    13,    14,  -449,     0,  -449,  -449,   810,
    -449,  -478,  -478,     0,    16,     0,    17,  -478,  -478,     0,
    -478,     0,     0,     0,  -478,     0,  -478,  -478,  -478,  -478,
    -478,  -478,  -478,  -478,  -478,  -478,  -478,     0,  -478,     0,
    -478,     0,  -478,  -478,  -478,  -478,  -478,     0,     0,     0,
       0,  -478,     0,     8,     9,    10,   126,  -478,     0,     0,
    -478,    12,    13,    14,     0,  -478,  -478,  -478,     0,     0,
    -251,  -478,  -478,     0,  -258,   243,  -478,   157,     7,    17,
       0,    10,    11,   158,   159,     0,   160,    12,    13,    14,
       0,     0,     0,  -478,     0,  -478,  -478,     0,  -478,     0,
       0,     0,     0,    16,   161,    17,    18,     0,   162,   163,
     164,   165,   166,     0,     0,     0,     0,   167,     0,     8,
       9,    10,    99,   168,     0,     0,   169,    12,    13,    14,
       0,   170,   171,   172,     0,     0,     0,   173,   174,     0,
       0,   582,   175,   583,   149,    17,     0,     0,     0,   158,
     159,     0,   160,     0,     0,     0,     7,     8,     9,    10,
     113,   176,   177,     0,   244,    12,    13,    14,     0,     0,
     161,     0,    18,     0,   162,   163,   164,   165,   166,     0,
       0,    16,     0,   167,     0,     8,     9,    10,   126,   168,
       0,     0,   169,    12,    13,    14,     0,   170,   171,   172,
       0,     0,     0,   173,   174,     0,     0,   584,   175,   585,
     582,    17,   583,   149,     0,     0,     0,     0,   158,   159,
       0,   160,     0,     0,     0,     0,  -304,   176,   177,     0,
     586,     0,     0,     0,     0,     0,     0,  -252,     0,   161,
       0,    18,     0,   162,   163,   164,   165,   166,     0,     0,
       0,     0,   167,     0,     8,     9,    10,   135,   168,     0,
       0,   169,    12,    13,    14,  -255,   170,   171,   172,     0,
       0,     0,   173,   174,     0,     0,   584,   175,   585,   564,
      17,   157,     0,     0,     0,     0,     0,   158,   159,     0,
     160,     0,     0,     0,     0,  -370,   176,   177,     0,   586,
       0,     0,     0,     0,     0,     0,     0,     0,   161,     0,
      18,     0,   162,   163,   164,   165,   166,     0,     0,     0,
       0,   167,     7,     8,     9,    10,   117,   168,     0,     0,
     169,    12,    13,    14,  -257,   170,   171,   172,     0,     0,
       0,   173,   174,   582,     0,   157,   175,    16,     0,    17,
       0,   158,   159,     0,   160,     0,     0,     0,     0,     0,
       0,     8,     9,    10,   135,   176,   177,     0,   565,    12,
      13,    14,   161,     0,    18,     0,   162,   163,   164,   165,
     166,     0,     0,     0,     0,   167,     0,    17,     0,     0,
       0,   168,     0,     0,   169,     0,     0,     0,     0,   170,
     171,   172,     0,  -253,     0,   173,   174,     0,   320,     0,
     175,   -23,   -23,   -23,   -23,   -23,     0,     0,     0,     0,
     -23,   -23,   -23,     0,     0,     0,     0,     0,     0,   176,
     177,     0,   586,     0,     0,   213,   -23,   314,  -274,     0,
     -19,   -19,   -19,   -19,   -19,     0,     0,     0,     0,   -19,
     -19,   -19,  -274,   157,     0,     0,     0,     0,     0,   158,
     159,     0,   160,     0,   213,   -19,     0,  -274,     0,     0,
       0,     0,     0,     0,     0,   214,   215,     0,     0,     0,
     161,  -274,    18,     0,   162,   163,   164,   165,   166,     0,
       0,     0,  -274,   167,     0,     0,  -274,   -23,     0,   168,
       0,     0,   169,     0,     0,     0,     0,   170,   171,   402,
       0,     0,     0,   173,   174,     0,     0,     0,   175,     0,
       0,  -274,   157,     0,     0,  -274,   -19,     0,   158,   159,
       0,   160,     0,     0,     0,     0,     7,   176,   177,    10,
      86,   403,     0,     0,     0,    12,    13,    14,     0,   161,
       0,    18,     0,   162,   163,   164,   165,   166,     0,     0,
       0,    16,   167,     7,     8,     9,    10,   122,   168,     0,
       0,   169,    12,    13,    14,     0,   170,   171,   172,     0,
       0,     0,   173,   174,     0,     0,     0,   175,    16,     0,
       0,     0,   583,   599,     8,     9,    10,    11,   158,   159,
       0,   160,    12,    13,    14,     0,   176,   177,     0,     0,
     491,   600,   601,   602,   603,   604,   605,   606,    16,   161,
      17,    18,     0,   162,   163,   164,   165,   166,     0,     0,
       0,     0,   167,     0,     0,     0,     0,     0,   168,     0,
       0,   169,     0,     0,  -254,     0,   170,   171,   172,     0,
       0,     0,   173,   174,     0,     0,     0,   175,     0,     0,
       0,   583,   149,     0,     0,     0,     0,   158,   159,     0,
     160,     0,     0,     0,   607,     0,   176,   177,     0,   608,
     600,   601,   602,   603,   604,   605,   606,     0,   161,     0,
      18,     0,   162,   163,   164,   165,   166,     0,     0,     0,
       0,   167,     0,     0,     0,     0,   323,   168,     0,     7,
     169,     0,    10,    11,     0,   170,   171,   172,    12,    13,
      14,   173,   174,   157,     0,     0,   175,     0,     0,   158,
     159,     0,   160,     0,    16,     0,    17,    18,     0,     0,
       0,     0,     0,   607,     0,   176,   177,     0,   608,     0,
     161,     0,    18,     0,   162,   163,   164,   165,   166,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,   168,
       0,     0,   169,     0,     0,     0,     0,   170,   171,   172,
       0,     0,     0,   173,   174,     0,     0,     0,   175,   157,
       7,     8,     9,    10,    11,   158,   159,     0,   160,    12,
      13,    14,     7,     8,     9,    10,    77,   176,   177,     0,
     434,    12,    13,    14,     0,    16,   161,    17,    18,     0,
     162,   163,   164,   165,   166,     0,     0,    16,     0,   167,
       0,     0,     0,     0,     0,   168,     0,     0,   169,     0,
       0,     0,     0,   170,   171,   172,     0,     0,     0,   173,
     174,   157,     7,     0,   175,    10,    11,   158,   159,     0,
     160,    12,    13,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,   177,     0,     0,    16,   161,    17,
      18,     0,   162,   163,   164,   165,   166,     0,     0,     0,
       0,   167,   157,     0,     0,     0,     0,   168,   158,   159,
     169,   160,     0,     0,     0,   170,   171,   172,     0,     0,
       0,   173,   174,     0,     0,     0,   175,     0,     0,   161,
       0,    18,     0,   162,   163,   164,   165,   166,     0,     0,
       0,     0,   167,   157,     0,   176,   177,     0,   168,   158,
     159,   169,   160,     0,     0,     0,   170,   171,   172,     0,
       0,     0,   173,   174,     0,     0,     0,   175,     0,     0,
     161,     0,    18,     0,   162,   163,   164,   165,   166,     0,
       0,     0,     0,   167,   692,     0,   176,   177,     0,   168,
       0,     7,   169,     0,    10,    72,     0,   170,   171,   172,
      12,    13,    14,   173,   174,   157,     0,     0,   175,     0,
       0,   158,   159,     0,   160,     0,    16,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,   176,   177,     0,
       0,     0,   161,     0,    18,     0,   162,   163,   164,   165,
     166,     0,     0,     0,     0,   167,   157,     0,     0,     0,
       0,   168,   158,   159,   169,   160,     0,     0,     0,   170,
     171,   172,     0,     0,     0,   173,   174,     0,     0,     0,
     250,     0,     0,   161,     0,    18,     0,   162,   163,   164,
     165,   166,     0,     0,     0,     0,   167,   467,     0,   176,
     177,     0,   168,   158,   159,   169,   160,     0,     0,     0,
     170,   171,   172,     0,     0,     0,   173,   174,     0,     0,
       0,   252,     0,     0,   161,     0,    18,     0,   162,   163,
     164,   165,   166,     0,     0,     0,     0,   167,     0,     0,
     176,   177,     0,   168,     0,     7,   169,     0,    10,    81,
       0,   170,   171,   172,    12,    13,    14,   173,   174,     0,
     746,     6,   175,  -118,     7,     8,     9,    10,    11,     0,
      16,     0,    17,    12,    13,    14,     0,     0,     0,     0,
       0,   176,   177,     0,     0,     0,     0,     0,    15,    16,
       0,    17,    18,   254,   255,   256,     0,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,     0,     0,   752,     7,     8,     9,    10,    11,  -118,
       0,   639,     0,    12,    13,    14,     0,     0,  -118,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,    16,
     747,    17,     0,     0,     0,    19,   254,   255,   256,   753,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   254,   255,   256,     0,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   254,   255,   256,     0,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
       0,     0,     0,     0,     0,     0,     0,     0,   254,   255,
     256,   823,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,     0,   431,   254,   255,
     256,     0,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,     0,     0,     0,     0,
     254,   255,   256,   432,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,     0,     0,
       0,     0,     0,   580,    17,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   254,   255,   256,
     745,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,     7,     8,     9,    10,    81,
       0,     0,     0,     0,    12,    13,    14,     7,     8,     9,
      10,   117,     0,     0,     0,     0,    12,    13,    14,     0,
      16,     0,    17,     0,     0,     0,     7,     0,     0,    10,
      11,     0,    16,     0,    17,    12,    13,    14,     7,     8,
       9,    10,   113,     0,     0,     0,     0,    12,    13,    14,
       0,    16,     0,    17,     0,     7,     8,     9,    10,    86,
       0,     0,     0,    16,    12,    13,    14,     7,     8,     9,
      10,   122,     0,     0,     0,     0,    12,    13,    14,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,   254,   255,   256,   824,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   254,   255,   256,     0,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   256,
       0,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270
};

static const yytype_int16 yycheck[] =
{
      63,    64,   297,     3,   144,     5,   284,     3,     3,     5,
       5,     3,   224,     5,   315,   145,    12,    13,    14,   317,
     321,   275,   399,   538,   542,    23,   551,   313,    24,   538,
      26,   460,    28,   410,    30,   594,    32,   538,    34,   538,
      36,   561,    38,   620,    10,     3,     3,     5,     5,     3,
       0,     5,     3,   554,     5,   554,    56,   215,     0,   399,
      56,    56,   220,     3,    56,     5,   175,     1,    64,     1,
     410,    67,    67,   305,   399,    67,     3,    11,     5,     1,
     152,    48,   597,     3,     4,   410,   456,   162,   163,     1,
     399,   215,    10,    60,    61,    62,   460,   172,    56,    56,
       1,   410,    56,   178,     1,    56,   230,     1,    75,    67,
      67,   620,     5,     3,   189,     0,    56,    84,    84,   620,
     767,   620,    47,   493,    47,   684,    93,    31,     3,    56,
      68,   240,    51,    52,    53,   102,   506,    40,    41,   509,
     394,   250,   300,   252,   111,   660,    68,   145,   380,     3,
       4,    69,   538,   120,     3,     4,    90,    47,    90,    84,
     530,    84,   129,    56,   811,   542,     3,     4,    47,    59,
      90,   138,    31,     8,    86,    85,    86,   554,    68,   175,
     700,   380,    68,   461,    59,    86,   550,   551,    47,    86,
     749,    68,    86,    68,    69,   191,    31,   193,   568,   195,
     570,   197,   542,   728,   729,    84,   202,   854,   204,   207,
     208,   597,   336,   276,   554,     3,   863,   542,    85,   215,
     215,   453,    59,   215,   220,   220,   384,    68,   220,   554,
     230,   808,     8,   542,   230,   230,    90,     3,   230,   302,
      31,    90,   209,    90,   240,   554,    90,   894,   380,   168,
     449,   450,   451,   452,   250,    31,   252,   215,   215,    90,
       3,     4,   220,   220,   782,   337,     6,   339,     8,   852,
      28,    59,   230,   230,    68,   350,   230,    68,    69,   230,
      68,    69,   201,     3,     4,   571,   715,   573,   500,    68,
     230,    31,   546,    59,   357,   724,    68,   295,   881,   808,
       8,    84,    68,   230,   300,   300,    89,   808,   300,   808,
      68,    69,   310,   311,    84,   234,    59,   449,   450,   451,
     452,    89,   431,    31,   433,    68,    69,   402,   698,   325,
     666,   327,   668,   329,    31,   331,   336,   400,   401,    59,
     336,   336,   300,   300,   336,    10,     3,     4,    68,    28,
      47,   715,    31,    68,   273,   274,   871,   872,    68,    69,
     724,     3,     4,    84,   728,   729,    45,   737,    89,     7,
       8,    68,    69,    45,    84,    13,    14,    15,   336,   336,
     665,    84,   336,    28,    84,   336,    31,    85,   384,   384,
      47,    89,   384,    31,   679,    67,   336,    69,    68,    69,
      45,    68,    59,    84,   399,   782,    85,   399,    89,   336,
      89,    68,    68,    69,    84,   410,    84,     8,   410,     8,
     718,    89,   720,    68,    69,   344,   384,   384,    84,    84,
     333,   334,   572,   563,    89,   431,    86,   433,   716,    85,
      85,    59,   782,    89,    89,   399,    86,   725,   399,    85,
      68,    69,    84,    89,    85,    84,   410,   782,    89,   410,
     379,     5,     6,     7,     8,    68,    69,   625,    85,    13,
      14,    15,    89,   782,   393,   538,   634,   473,    85,   475,
      85,   477,    89,   479,    89,     4,     5,     6,     7,     8,
      84,   861,   862,    85,    13,    14,    15,    89,    85,   214,
      85,   802,   217,   804,    89,     1,   221,     3,     4,   428,
      29,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     5,     6,     7,
       8,    59,    60,    61,   597,    13,    14,    15,   538,    68,
      69,   604,   538,   538,    47,   620,   538,   542,    84,   761,
     542,    33,   550,   551,   554,    85,    86,    84,   554,   554,
     700,    68,   554,    10,     4,   563,    85,     7,     8,   449,
     450,   701,   452,    13,    14,    15,   574,     6,   481,   482,
     538,   538,   877,    85,   538,    68,    69,   538,   542,    29,
      45,   542,   495,   496,   889,    85,   554,   554,   538,    45,
     554,   316,   317,   554,   523,    68,    69,   322,    89,     7,
       8,   538,    68,    69,   554,    13,    14,    15,    89,   538,
     620,   182,   183,   338,   620,   620,   310,   311,   620,   625,
     625,   694,    84,   625,   715,   716,    84,   633,   634,   634,
      47,    84,   634,    84,   640,    45,   642,    91,   644,   789,
     646,    86,   648,    91,   650,   225,   652,    90,   654,    86,
      86,    86,   620,   620,    47,   584,   620,   625,   625,   620,
     385,    57,    58,    59,    60,    61,   634,   634,   597,     1,
     620,     5,     6,     7,     8,    84,   605,    84,   201,    13,
      14,    15,    84,   620,   757,    54,    55,    56,    57,    58,
      59,    60,    61,   701,    91,    28,   769,    31,    85,   612,
     613,    91,   775,    85,   284,     7,     8,   715,   716,    89,
      89,    13,    14,    15,    86,     3,   724,   725,     7,     8,
     728,   729,    84,    84,    13,    14,    15,    84,    47,    31,
      47,    86,   805,   806,    47,   808,    89,    47,   463,    24,
      25,    26,    27,   656,   657,    85,   723,    32,    33,    34,
      35,    24,    25,    26,    27,    85,     8,    85,    85,    32,
      33,    34,    35,    86,    90,     4,     5,     6,     7,     8,
      85,    85,    68,    68,    13,    14,    15,   782,    86,   852,
     782,    17,    86,   856,    85,   510,   511,    47,    68,   514,
      29,    16,    31,    18,    19,    20,    21,    68,   808,    68,
      18,    84,   808,   808,    85,    84,   808,    84,   881,   882,
     390,    85,    84,     4,     5,     6,     7,     8,   782,    85,
      68,   782,    13,    14,    15,     4,     5,     6,     7,     8,
      89,    84,   412,   413,    13,    14,    15,    91,    29,    85,
     808,   808,    85,    10,   808,    68,    85,   808,    85,    84,
      29,    84,    31,   578,    28,    29,    30,    31,   808,    85,
     394,   554,    36,    37,    38,    39,    28,    29,    30,    31,
     393,   808,   306,   554,    36,    37,    38,    39,   554,   410,
     460,   461,   554,   410,     1,   393,   414,     4,     5,     6,
       7,     8,   295,   593,    11,   569,    13,    14,    15,   682,
     728,   626,   627,   715,   430,   630,   191,   192,   193,   194,
     635,   636,    29,   638,   843,   336,   672,   669,   191,   192,
     193,   194,    53,    54,    55,    56,    57,    58,    59,    60,
      61,     1,   344,   428,     4,     5,     6,     7,     8,   812,
     381,   858,   814,    13,    14,    15,   671,   616,   860,   674,
     473,   474,   475,   476,   477,   478,   479,   480,    28,    29,
     463,    31,    55,    56,    57,    58,    59,    60,    61,    -1,
     550,   551,   482,    90,    -1,    45,     1,    -1,    -1,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,    14,
      15,    -1,    -1,   718,    -1,   720,    -1,    -1,    68,    69,
       5,     6,     7,     8,    29,    -1,    -1,    -1,    13,    14,
      15,    -1,    -1,   738,    -1,    85,    -1,   742,    -1,    89,
      90,   195,   196,   197,   198,    -1,    31,    -1,    -1,    -1,
     380,    -1,    -1,   195,   196,   197,   198,    -1,    -1,    -1,
     325,   326,   327,   328,    -1,    -1,    -1,    -1,   628,   629,
      -1,    -1,   325,   326,   327,   328,   406,    -1,   783,   784,
      -1,   786,   238,   239,    -1,    90,   791,    -1,   793,    -1,
     420,   421,    -1,   249,    -1,   800,    -1,    -1,   254,   255,
      85,    -1,    -1,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,     5,     6,     7,     8,   449,
     450,   451,   452,    13,    14,    15,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,   640,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,   654,   655,    -1,    -1,   715,   716,    63,    64,    -1,
      66,    67,    68,    69,   724,   725,    -1,    -1,   728,   729,
      -1,   501,    -1,   503,    -1,   329,   330,   331,   332,    -1,
      -1,    -1,    -1,     5,     6,     7,     8,   329,   330,   331,
     332,    13,    14,    15,    -1,    85,    -1,     5,     6,     7,
       8,    -1,    -1,   359,   360,    13,    14,    15,   473,   474,
      -1,    -1,   477,   478,    -1,    -1,    -1,   777,   778,   779,
     473,   474,    -1,    31,   477,   478,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      -1,   427,    43,    -1,    -1,    -1,   432,    85,    49,   609,
      -1,    52,    -1,   439,    -1,    -1,    57,    58,    59,    -1,
      -1,    -1,    63,    64,    -1,    -1,     1,    68,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    -1,    13,    14,
      15,    -1,    -1,    -1,    85,    86,    87,    88,    -1,    90,
      -1,   475,   476,    -1,    29,   479,   480,     3,     4,     5,
       6,     7,     8,   475,   476,    -1,    -1,   479,   480,   669,
       1,   497,   672,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    13,    14,    15,    -1,     4,     5,     6,     7,
       8,    -1,    -1,   519,    -1,    13,    14,    15,    29,    -1,
      -1,    -1,    -1,    -1,    -1,   640,   641,   642,   643,    84,
      -1,    29,    -1,   648,   649,   650,   651,   640,   641,   642,
     643,    -1,    -1,    -1,    -1,   648,   649,   650,   651,     1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,     9,    10,    -1,
      12,    -1,    -1,   569,    -1,    -1,    -1,     5,     6,     7,
       8,    -1,    -1,    84,   580,    13,    14,    15,    30,   585,
      32,    -1,    34,    35,    36,    37,    38,    85,   594,   595,
      -1,    43,   772,    45,   600,    -1,     1,    49,    -1,     4,
      52,    -1,     7,     8,    -1,    57,    58,    59,    13,    14,
      15,    63,    64,    -1,    -1,    67,    68,    69,    -1,     4,
       5,     6,     7,     8,    29,    -1,    31,    32,    13,    14,
      15,    -1,    -1,    -1,   814,    87,    88,    -1,    90,    -1,
     644,   645,   646,   647,    29,    -1,    31,    85,   652,   653,
     654,   655,   644,   645,   646,   647,    -1,    -1,    -1,   665,
     652,   653,   654,   655,   670,    -1,    -1,   673,   848,   849,
      -1,    -1,    -1,   679,    -1,    -1,   682,    -1,   684,    -1,
      85,    86,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      13,    14,    15,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    -1,    -1,   739,    -1,    43,    -1,   743,    -1,    -1,
     746,    49,    -1,   749,    52,    -1,   752,    -1,    -1,    57,
      58,    59,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,     4,     5,     6,     7,     8,     5,     6,     7,     8,
      13,    14,    15,    -1,    13,    14,    15,    85,    -1,    87,
      88,     1,    90,     3,     4,    -1,    29,    -1,    31,     9,
      10,    -1,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    -1,    85,    63,    64,    -1,    85,    -1,    68,     4,
       5,     6,     7,     8,     5,     6,     7,     8,    13,    14,
      15,    -1,    13,    14,    15,    85,    -1,    87,    88,     1,
      90,     3,     4,    -1,    29,    -1,    31,     9,    10,    -1,
      12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,     5,     6,     7,     8,    49,    -1,    -1,
      52,    13,    14,    15,    -1,    57,    58,    59,    -1,    -1,
      85,    63,    64,    -1,    85,     1,    68,     3,     4,    31,
      -1,     7,     8,     9,    10,    -1,    12,    13,    14,    15,
      -1,    -1,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    -1,    34,    35,
      36,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,     5,
       6,     7,     8,    49,    -1,    -1,    52,    13,    14,    15,
      -1,    57,    58,    59,    -1,    -1,    -1,    63,    64,    -1,
      -1,     1,    68,     3,     4,    31,    -1,    -1,    -1,     9,
      10,    -1,    12,    -1,    -1,    -1,     4,     5,     6,     7,
       8,    87,    88,    -1,    90,    13,    14,    15,    -1,    -1,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    -1,
      -1,    29,    -1,    43,    -1,     5,     6,     7,     8,    49,
      -1,    -1,    52,    13,    14,    15,    -1,    57,    58,    59,
      -1,    -1,    -1,    63,    64,    -1,    -1,    67,    68,    69,
       1,    31,     3,     4,    -1,    -1,    -1,    -1,     9,    10,
      -1,    12,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,     5,     6,     7,     8,    49,    -1,
      -1,    52,    13,    14,    15,    85,    57,    58,    59,    -1,
      -1,    -1,    63,    64,    -1,    -1,    67,    68,    69,     1,
      31,     3,    -1,    -1,    -1,    -1,    -1,     9,    10,    -1,
      12,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      -1,    43,     4,     5,     6,     7,     8,    49,    -1,    -1,
      52,    13,    14,    15,    85,    57,    58,    59,    -1,    -1,
      -1,    63,    64,     1,    -1,     3,    68,    29,    -1,    31,
      -1,     9,    10,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,     5,     6,     7,     8,    87,    88,    -1,    90,    13,
      14,    15,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    31,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    85,    -1,    63,    64,    -1,     1,    -1,
      68,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    90,    -1,    -1,    28,    29,     1,    31,    -1,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,
      14,    15,    45,     3,    -1,    -1,    -1,    -1,    -1,     9,
      10,    -1,    12,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
      30,    45,    32,    -1,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    85,    43,    -1,    -1,    89,    90,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,
      -1,    85,     3,    -1,    -1,    89,    90,    -1,     9,    10,
      -1,    12,    -1,    -1,    -1,    -1,     4,    87,    88,     7,
       8,    91,    -1,    -1,    -1,    13,    14,    15,    -1,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    29,    43,     4,     5,     6,     7,     8,    49,    -1,
      -1,    52,    13,    14,    15,    -1,    57,    58,    59,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    29,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    13,    14,    15,    -1,    87,    88,    -1,    -1,
      91,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    85,    -1,    57,    58,    59,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,     3,     4,    -1,    -1,    -1,    -1,     9,    10,    -1,
      12,    -1,    -1,    -1,    85,    -1,    87,    88,    -1,    90,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,     1,    49,    -1,     4,
      52,    -1,     7,     8,    -1,    57,    58,    59,    13,    14,
      15,    63,    64,     3,    -1,    -1,    68,    -1,    -1,     9,
      10,    -1,    12,    -1,    29,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    58,    59,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    13,
      14,    15,     4,     5,     6,     7,     8,    87,    88,    -1,
      90,    13,    14,    15,    -1,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    -1,    -1,    29,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    57,    58,    59,    -1,    -1,    -1,    63,
      64,     3,     4,    -1,    68,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    29,    30,    31,
      32,    -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      -1,    43,     3,    -1,    -1,    -1,    -1,    49,     9,    10,
      52,    12,    -1,    -1,    -1,    57,    58,    59,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    -1,    43,     3,    -1,    87,    88,    -1,    49,     9,
      10,    52,    12,    -1,    -1,    -1,    57,    58,    59,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    43,    85,    -1,    87,    88,    -1,    49,
      -1,     4,    52,    -1,     7,     8,    -1,    57,    58,    59,
      13,    14,    15,    63,    64,     3,    -1,    -1,    68,    -1,
      -1,     9,    10,    -1,    12,    -1,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    43,     3,    -1,    -1,    -1,
      -1,    49,     9,    10,    52,    12,    -1,    -1,    -1,    57,
      58,    59,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    -1,    -1,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    -1,    -1,    -1,    -1,    43,     3,    -1,    87,
      88,    -1,    49,     9,    10,    52,    12,    -1,    -1,    -1,
      57,    58,    59,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    68,    -1,    -1,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      87,    88,    -1,    49,    -1,     4,    52,    -1,     7,     8,
      -1,    57,    58,    59,    13,    14,    15,    63,    64,    -1,
      11,     1,    68,     3,     4,     5,     6,     7,     8,    -1,
      29,    -1,    31,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    31,    32,    44,    45,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    11,     4,     5,     6,     7,     8,    59,
      -1,    11,    -1,    13,    14,    15,    -1,    -1,    68,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    29,
      91,    31,    -1,    -1,    -1,    85,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    44,    45,    46,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    44,    45,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    91,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    89,    44,    45,
      46,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    -1,    -1,    -1,
      44,    45,    46,    89,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    -1,    -1,
      -1,    -1,    -1,    89,    31,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    44,    45,    46,
      84,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    13,    14,    15,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    13,    14,    15,    -1,
      29,    -1,    31,    -1,    -1,    -1,     4,    -1,    -1,     7,
       8,    -1,    29,    -1,    31,    13,    14,    15,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    13,    14,    15,
      -1,    29,    -1,    31,    -1,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    29,    13,    14,    15,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    13,    14,    15,    -1,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    93,    94,    95,     0,    96,     1,     4,     5,     6,
       7,     8,    13,    14,    15,    28,    29,    31,    32,    85,
      97,    98,    99,   129,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   155,   157,   158,   159,   160,   161,   170,   171,
     175,   198,   199,   200,   201,   206,   291,    97,    85,    86,
     170,   170,   170,    68,    68,    68,     3,    59,    68,   163,
     167,   197,     8,   157,   158,   170,   175,     8,   157,   158,
     175,     8,   157,   158,   170,   175,     8,   157,   158,   175,
       8,   159,   160,   170,   175,     8,   159,   160,   175,     8,
     159,   160,   170,   175,     8,   159,   160,   175,     8,   157,
     158,   170,   175,     8,   157,   158,   175,     8,   157,   158,
     170,   175,     8,   157,   158,   175,     8,   159,   160,   170,
     175,     8,   159,   160,   175,     8,   159,   160,   170,   175,
       8,   159,   160,   175,   129,   129,    85,   171,     3,     4,
      90,   106,    90,   106,    90,   106,    97,     3,     9,    10,
      12,    30,    34,    35,    36,    37,    38,    43,    49,    52,
      57,    58,    59,    63,    64,    68,    87,    88,   107,   108,
     110,   111,   112,   113,   114,   115,   121,   123,   245,   291,
     108,   132,   133,   134,   135,   136,   137,   138,   139,   154,
     218,    68,   132,   133,   134,   135,   153,   156,   169,   170,
      85,    89,     1,    28,    68,    69,   104,   164,   226,     4,
      59,    68,   162,   165,   192,   193,   197,   163,   197,   209,
     210,    90,   209,    90,   205,    90,   114,   114,    68,    68,
      68,   106,   114,     1,    90,   108,   218,   114,    84,    89,
      68,   111,    68,   111,    44,    45,    46,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    63,    64,    66,    67,    68,    69,    10,     1,    86,
     234,   243,   114,    84,   219,    84,     7,     8,   106,   172,
     173,   174,   175,   197,   197,   130,   169,    68,   169,   286,
       6,   153,   156,     1,   124,   125,   126,   233,   252,   169,
     156,   169,    85,    89,     1,   100,   164,    68,   226,    85,
       1,   102,    86,     1,    85,   132,   133,   134,   135,   136,
     137,   138,   139,   152,   153,   211,   291,   202,    86,   203,
       1,   106,   216,   217,   204,   115,   115,   218,    84,    84,
      84,    85,   115,   218,   218,   115,   115,   118,   120,   117,
     116,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   106,   106,   109,   110,   108,    84,    33,
     238,   239,   240,    84,    59,    68,   220,   222,   223,   224,
     225,   226,    84,    89,    68,    84,   167,   197,   123,   287,
     156,     6,    59,    91,   108,    85,   105,   233,    11,   125,
     253,    45,   193,   193,   130,   124,   169,   286,   124,   169,
     129,   129,    85,   211,   209,   169,   209,    45,    89,   208,
     216,    89,    89,    89,    90,   114,    84,    84,   108,    47,
     115,   115,    84,    91,   106,   290,     1,   128,   227,   228,
     229,   230,   231,   232,   233,   244,   252,   256,   257,   240,
     156,   169,   276,    68,   226,    84,   173,     3,   109,    84,
       1,     3,    11,   132,   133,   136,   137,   140,   141,   144,
     145,   150,   151,   278,   281,   282,   284,   288,   289,   108,
     108,    91,    91,   252,   127,   150,   151,   168,    84,   165,
     192,   101,    45,   103,   212,   214,   252,   213,   215,   252,
      86,    86,   115,   217,    86,   208,   218,   115,   218,   122,
      47,   115,    85,    89,   128,   256,   257,   128,   256,   257,
     252,   256,   257,   128,   256,   257,   233,    86,   253,   223,
     224,   222,   277,   169,   276,    84,    89,    84,    84,   129,
     285,   285,    84,    85,    89,    84,    89,    91,    91,   253,
      85,   129,    85,   129,     1,    90,   115,   176,   252,   166,
     252,    89,   253,    89,   253,   169,   169,   169,    86,    84,
      89,    84,     1,     3,    67,    69,    90,   106,   115,   178,
     179,   180,   182,   184,   185,   119,   106,   253,   235,     4,
      22,    23,    24,    25,    26,    27,    28,    85,    90,   106,
     108,   131,   148,   149,   155,   236,   242,   246,   268,   269,
     291,    84,   278,   110,     4,    59,    68,   194,   195,   196,
     197,   221,   222,   223,    59,    68,   197,   221,   279,    11,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   283,     3,
       1,   241,   246,   162,   163,   177,   253,   176,   253,   130,
      47,   192,   130,    47,   197,   169,   115,   106,   115,   183,
      47,    86,    89,   207,    45,   185,   182,   115,   115,    47,
      85,    85,    85,   108,    59,   106,     8,   270,   252,    85,
     129,   129,    85,    16,    18,    19,    20,    21,   247,   248,
     250,   258,   243,   131,    84,   156,   169,   169,    68,   226,
      68,   226,   169,   170,   156,   169,   169,   169,   129,   129,
     246,    85,    85,   178,   241,   241,   214,   252,   115,    47,
     169,   215,   115,    47,   169,    84,    11,    91,   178,   181,
     180,   182,    11,    47,    85,   108,    85,    68,   253,   162,
     186,   192,   163,   189,   197,   249,   260,   251,   262,    68,
      17,     1,   236,   255,     1,    68,   237,   195,   196,   196,
     286,   286,   280,   194,   197,   221,   197,   221,    86,   253,
     169,   115,   169,   115,   115,    86,   182,   115,    85,   108,
      47,    85,   187,    85,   190,    68,    68,   255,    68,   108,
       1,   259,   254,   256,   257,   108,    84,   278,   169,   169,
     169,   169,   169,    91,    47,    47,    84,   169,   124,   124,
     108,   108,    18,   108,   131,   267,   271,    84,   255,   237,
     254,    84,    10,    69,   272,   273,   274,    85,   188,   191,
      84,    84,   263,    85,   266,    85,    68,   106,    47,    84,
      89,   252,   252,   261,   271,   255,   108,    91,   272,    85,
     274,   253,   253,   255,    85,    84,    10,    47,    84,   246,
     246,   264,    68,   123,   275,    85,   271,   108,    84,    89,
      84,    84,    85,   123,   265,   255
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,    92,    93,    93,    95,    94,    96,    94,    97,    97,
      97,    97,    98,    98,    98,    98,    98,    98,    98,   100,
     101,    99,    99,   102,   103,    99,    99,   104,   105,    99,
      99,   106,   106,   107,   107,   107,   107,   107,   107,   107,
     108,   109,   109,   110,   110,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   112,   113,   114,   114,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   116,   115,   117,   115,   118,   119,   115,
     120,   115,   115,   115,   121,   121,   121,   121,   122,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   123,   123,   124,   124,   124,   125,   126,
     126,   126,   126,   127,   127,   127,   127,   128,   129,   130,
     131,   131,   131,   131,   131,   131,   132,   132,   132,   133,
     134,   134,   135,   135,   136,   136,   136,   136,   136,   136,
     136,   137,   137,   137,   137,   137,   137,   138,   138,   138,
     138,   138,   138,   139,   139,   139,   139,   139,   140,   140,
     140,   140,   140,   140,   140,   141,   142,   142,   142,   142,
     142,   142,   143,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   145,   145,   145,   145,   145,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   147,   147,
     147,   147,   147,   148,   148,   148,   148,   148,   148,   148,
     148,   149,   149,   149,   149,   149,   149,   149,   149,   150,
     150,   150,   150,   151,   151,   151,   151,   152,   152,   152,
     152,   153,   153,   153,   153,   154,   154,   154,   154,   154,
     154,   154,   154,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     156,   157,   157,   158,   159,   159,   160,   161,   161,   161,
     162,   162,   163,   163,   164,   164,   166,   165,   165,   168,
     167,   167,   169,   169,   170,   170,   171,   172,   172,   173,
     173,   173,   173,   173,   174,   174,   174,   174,   175,   175,
     176,   177,   176,   176,   178,   178,   179,   179,   180,   180,
     181,   180,   180,   183,   182,   182,   182,   184,   184,   185,
     185,   185,   187,   188,   186,   190,   191,   189,   192,   192,
     193,   193,   193,   193,   193,   194,   194,   195,   195,   195,
     196,   196,   196,   196,   196,   197,   197,   197,   197,   197,
     198,   198,   199,   199,   200,   200,   202,   201,   201,   203,
     201,   201,   204,   201,   205,   201,   206,   206,   206,   207,
     207,   208,   208,   209,   209,   210,   210,   210,   211,   211,
     211,   211,   211,   211,   212,   212,   213,   213,   214,   214,
     214,   215,   215,   215,   216,   216,   216,   217,   217,   219,
     218,   220,   220,   221,   221,   221,   222,   222,   223,   223,
     224,   224,   225,   225,   225,   225,   225,   226,   226,   226,
     226,   226,   227,   227,   227,   227,   228,   228,   228,   228,
     228,   229,   229,   229,   229,   230,   230,   230,   230,   230,
     231,   231,   232,   232,   232,   232,   233,   234,   235,   236,
     237,   238,   238,   239,   239,   240,   241,   241,   242,   243,
     243,   244,   244,   245,   246,   247,   247,   249,   248,   251,
     250,   252,   253,   254,   254,   255,   256,   257,   259,   258,
     258,   258,   260,   261,   258,   258,   258,   262,   263,   264,
     265,   258,   266,   258,   267,   267,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     269,   269,   269,   269,   270,   270,   271,   271,   272,   272,
     273,   273,   274,   274,   275,   275,   277,   276,   278,   279,
     280,   278,   278,   281,   281,   281,   281,   282,   282,   283,
     283,   283,   283,   283,   284,   284,   284,   284,   284,   285,
     287,   286,   288,   288,   289,   289,   290,   290,   291
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     0,     2,     0,     3,     1,     1,
       5,     2,     3,     4,     4,     2,     2,     2,     1,     0,
       0,     9,     4,     0,     0,     9,     4,     0,     0,     8,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     3,     1,     2,     2,     2,     2,
       2,     4,     2,     4,     2,     2,     1,     1,     1,     4,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     0,     4,     0,     4,     0,     0,     7,
       0,     5,     3,     3,     1,     1,     1,     1,     0,     7,
       3,     3,     3,     3,     4,     6,     8,     6,     4,     3,
       3,     2,     2,     1,     2,     0,     1,     2,     3,     1,
       1,     2,     2,     4,     4,     2,     2,     3,     0,     1,
       4,     4,     3,     3,     2,     2,     1,     2,     2,     2,
       2,     2,     1,     2,     1,     2,     2,     2,     2,     2,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     4,     1,     4,     0,     4,     0,     6,     3,     0,
       6,     3,     0,     1,     1,     2,     6,     1,     3,     0,
       1,     4,     6,     4,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     1,     0,     2,     1,     3,     3,     2,
       0,     4,     1,     0,     4,     1,     1,     1,     2,     2,
       5,     3,     0,     0,     7,     0,     0,     7,     1,     1,
       4,     3,     2,     3,     1,     1,     1,     3,     2,     1,
       3,     2,     3,     3,     4,     3,     4,     3,     2,     1,
       1,     2,     1,     2,     1,     2,     0,     7,     5,     0,
       7,     5,     0,     8,     0,     7,     2,     2,     2,     0,
       1,     0,     1,     1,     2,     0,     3,     2,     3,     4,
       3,     1,     1,     2,     1,     4,     1,     4,     4,     6,
       5,     4,     6,     5,     1,     3,     1,     1,     3,     0,
       3,     0,     1,     0,     1,     2,     1,     1,     1,     3,
       2,     3,     4,     3,     2,     2,     1,     4,     3,     4,
       5,     5,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     2,     2,     2,     1,     2,     2,     2,     2,
       1,     2,     1,     1,     1,     1,     2,     0,     0,     0,
       0,     0,     1,     1,     2,     3,     1,     2,     1,     1,
       5,     1,     1,     2,     2,     2,     2,     0,     5,     0,
       4,     0,     0,     1,     2,     3,     3,     3,     0,     4,
       1,     3,     0,     0,     7,     5,     2,     0,     0,     0,
       0,    12,     0,     6,     2,     1,     1,     2,     3,     2,
       2,     2,     3,     6,     8,    10,    12,     3,     4,     1,
       3,     5,     2,     5,     0,     1,     0,     1,     0,     1,
       1,     3,     4,     7,     1,     3,     0,     3,     2,     0,
       0,     6,     2,     0,     1,     1,     3,     1,     3,     4,
       4,     3,     4,     3,     4,     4,     3,     4,     3,     1,
       0,     3,     1,     2,     1,     3,     1,     3,     1
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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: %empty  */
#line 287 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids an empty source file");
		  finish_file ();
		}
#line 2756 "c-p12907.c"
    break;

  case 3: /* program: extdefs  */
#line 292 "c-parse.y"
                {
		  /* In case there were missing closebraces,
		     get us back to the global binding level.  */
		  while (! global_bindings_p ())
		    poplevel (0, 0, 0);
		  /* __FUNCTION__ is defined at file scope ("").  This
		     call may not be necessary as my tests indicate it
		     still works without it.  */
		  finish_fname_decls ();
                  finish_file ();
		}
#line 2772 "c-p12907.c"
    break;

  case 4: /* @1: %empty  */
#line 310 "c-parse.y"
        {(yyval.ttype) = NULL_TREE; }
#line 2778 "c-p12907.c"
    break;

  case 6: /* @2: %empty  */
#line 311 "c-parse.y"
                  {(yyval.ttype) = NULL_TREE; ggc_collect(); }
#line 2784 "c-p12907.c"
    break;

  case 10: /* extdef: ASM_KEYWORD '(' expr ')' ';'  */
#line 318 "c-parse.y"
                { STRIP_NOPS ((yyvsp[-2].ttype));
		  if ((TREE_CODE ((yyvsp[-2].ttype)) == ADDR_EXPR
		       && TREE_CODE (TREE_OPERAND ((yyvsp[-2].ttype), 0)) == STRING_CST)
		      || TREE_CODE ((yyvsp[-2].ttype)) == STRING_CST)
		    assemble_asm ((yyvsp[-2].ttype));
		  else
		    error ("argument of `asm' is not a constant string"); }
#line 2796 "c-p12907.c"
    break;

  case 11: /* extdef: extension extdef  */
#line 326 "c-parse.y"
                { RESTORE_WARN_FLAGS ((yyvsp[-1].ttype)); }
#line 2802 "c-p12907.c"
    break;

  case 12: /* datadef: setspecs notype_initdecls ';'  */
#line 331 "c-parse.y"
                { if (pedantic)
		    error ("ISO C forbids data definition with no type or storage class");
		  else
		    warning ("data definition has no type or storage class"); 

		  POP_DECLSPEC_STACK; }
#line 2813 "c-p12907.c"
    break;

  case 13: /* datadef: declspecs_nots setspecs notype_initdecls ';'  */
#line 338 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 2819 "c-p12907.c"
    break;

  case 14: /* datadef: declspecs_ts setspecs initdecls ';'  */
#line 340 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 2825 "c-p12907.c"
    break;

  case 15: /* datadef: declspecs ';'  */
#line 342 "c-parse.y"
          { shadow_tag ((yyvsp[-1].ttype)); }
#line 2831 "c-p12907.c"
    break;

  case 18: /* datadef: ';'  */
#line 346 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C does not allow extra `;' outside of a function"); }
#line 2838 "c-p12907.c"
    break;

  case 19: /* $@3: %empty  */
#line 352 "c-parse.y"
                { if (! start_function (current_declspecs, (yyvsp[0].ttype),
					all_prefix_attributes))
		    YYERROR1;
		}
#line 2847 "c-p12907.c"
    break;

  case 20: /* $@4: %empty  */
#line 357 "c-parse.y"
                { store_parm_decls (); }
#line 2853 "c-p12907.c"
    break;

  case 21: /* fndef: declspecs_ts setspecs declarator $@3 old_style_parm_decls $@4 save_filename save_lineno compstmt_or_error  */
#line 359 "c-parse.y"
                { DECL_SOURCE_FILE (current_function_decl) = (yyvsp[-2].filename);
		  DECL_SOURCE_LINE (current_function_decl) = (yyvsp[-1].lineno);
		  finish_function (0, 1); 
		  POP_DECLSPEC_STACK; }
#line 2862 "c-p12907.c"
    break;

  case 22: /* fndef: declspecs_ts setspecs declarator error  */
#line 364 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 2868 "c-p12907.c"
    break;

  case 23: /* $@5: %empty  */
#line 366 "c-parse.y"
                { if (! start_function (current_declspecs, (yyvsp[0].ttype),
					all_prefix_attributes))
		    YYERROR1;
		}
#line 2877 "c-p12907.c"
    break;

  case 24: /* $@6: %empty  */
#line 371 "c-parse.y"
                { store_parm_decls (); }
#line 2883 "c-p12907.c"
    break;

  case 25: /* fndef: declspecs_nots setspecs notype_declarator $@5 old_style_parm_decls $@6 save_filename save_lineno compstmt_or_error  */
#line 373 "c-parse.y"
                { DECL_SOURCE_FILE (current_function_decl) = (yyvsp[-2].filename);
		  DECL_SOURCE_LINE (current_function_decl) = (yyvsp[-1].lineno);
		  finish_function (0, 1); 
		  POP_DECLSPEC_STACK; }
#line 2892 "c-p12907.c"
    break;

  case 26: /* fndef: declspecs_nots setspecs notype_declarator error  */
#line 378 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 2898 "c-p12907.c"
    break;

  case 27: /* $@7: %empty  */
#line 380 "c-parse.y"
                { if (! start_function (NULL_TREE, (yyvsp[0].ttype),
					all_prefix_attributes))
		    YYERROR1;
		}
#line 2907 "c-p12907.c"
    break;

  case 28: /* $@8: %empty  */
#line 385 "c-parse.y"
                { store_parm_decls (); }
#line 2913 "c-p12907.c"
    break;

  case 29: /* fndef: setspecs notype_declarator $@7 old_style_parm_decls $@8 save_filename save_lineno compstmt_or_error  */
#line 387 "c-parse.y"
                { DECL_SOURCE_FILE (current_function_decl) = (yyvsp[-2].filename);
		  DECL_SOURCE_LINE (current_function_decl) = (yyvsp[-1].lineno);
		  finish_function (0, 1); 
		  POP_DECLSPEC_STACK; }
#line 2922 "c-p12907.c"
    break;

  case 30: /* fndef: setspecs notype_declarator error  */
#line 392 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 2928 "c-p12907.c"
    break;

  case 33: /* unop: '&'  */
#line 401 "c-parse.y"
                { (yyval.code) = ADDR_EXPR; }
#line 2934 "c-p12907.c"
    break;

  case 34: /* unop: '-'  */
#line 403 "c-parse.y"
                { (yyval.code) = NEGATE_EXPR; }
#line 2940 "c-p12907.c"
    break;

  case 35: /* unop: '+'  */
#line 405 "c-parse.y"
                { (yyval.code) = CONVERT_EXPR;
  if (warn_traditional && !in_system_header)
    warning ("traditional C rejects the unary plus operator");
		}
#line 2949 "c-p12907.c"
    break;

  case 36: /* unop: PLUSPLUS  */
#line 410 "c-parse.y"
                { (yyval.code) = PREINCREMENT_EXPR; }
#line 2955 "c-p12907.c"
    break;

  case 37: /* unop: MINUSMINUS  */
#line 412 "c-parse.y"
                { (yyval.code) = PREDECREMENT_EXPR; }
#line 2961 "c-p12907.c"
    break;

  case 38: /* unop: '~'  */
#line 414 "c-parse.y"
                { (yyval.code) = BIT_NOT_EXPR; }
#line 2967 "c-p12907.c"
    break;

  case 39: /* unop: '!'  */
#line 416 "c-parse.y"
                { (yyval.code) = TRUTH_NOT_EXPR; }
#line 2973 "c-p12907.c"
    break;

  case 40: /* expr: nonnull_exprlist  */
#line 420 "c-parse.y"
                { (yyval.ttype) = build_compound_expr ((yyvsp[0].ttype)); }
#line 2979 "c-p12907.c"
    break;

  case 41: /* exprlist: %empty  */
#line 425 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 2985 "c-p12907.c"
    break;

  case 43: /* nonnull_exprlist: expr_no_commas  */
#line 431 "c-parse.y"
                { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].ttype)); }
#line 2991 "c-p12907.c"
    break;

  case 44: /* nonnull_exprlist: nonnull_exprlist ',' expr_no_commas  */
#line 433 "c-parse.y"
                { chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE, (yyvsp[0].ttype))); }
#line 2997 "c-p12907.c"
    break;

  case 46: /* unary_expr: '*' cast_expr  */
#line 439 "c-parse.y"
                { (yyval.ttype) = build_indirect_ref ((yyvsp[0].ttype), "unary *"); }
#line 3003 "c-p12907.c"
    break;

  case 47: /* unary_expr: extension cast_expr  */
#line 442 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  RESTORE_WARN_FLAGS ((yyvsp[-1].ttype)); }
#line 3010 "c-p12907.c"
    break;

  case 48: /* unary_expr: unop cast_expr  */
#line 445 "c-parse.y"
                { (yyval.ttype) = build_unary_op ((yyvsp[-1].code), (yyvsp[0].ttype), 0);
		  overflow_warning ((yyval.ttype)); }
#line 3017 "c-p12907.c"
    break;

  case 49: /* unary_expr: ANDAND identifier  */
#line 449 "c-parse.y"
                { (yyval.ttype) = finish_label_address_expr ((yyvsp[0].ttype)); }
#line 3023 "c-p12907.c"
    break;

  case 50: /* unary_expr: sizeof unary_expr  */
#line 466 "c-parse.y"
                { skip_evaluation--;
		  if (TREE_CODE ((yyvsp[0].ttype)) == COMPONENT_REF
		      && DECL_C_BIT_FIELD (TREE_OPERAND ((yyvsp[0].ttype), 1)))
		    error ("`sizeof' applied to a bit-field");
		  (yyval.ttype) = c_sizeof (TREE_TYPE ((yyvsp[0].ttype))); }
#line 3033 "c-p12907.c"
    break;

  case 51: /* unary_expr: sizeof '(' typename ')'  */
#line 472 "c-parse.y"
                { skip_evaluation--;
		  (yyval.ttype) = c_sizeof (groktypename ((yyvsp[-1].ttype))); }
#line 3040 "c-p12907.c"
    break;

  case 52: /* unary_expr: alignof unary_expr  */
#line 475 "c-parse.y"
                { skip_evaluation--;
		  (yyval.ttype) = c_alignof_expr ((yyvsp[0].ttype)); }
#line 3047 "c-p12907.c"
    break;

  case 53: /* unary_expr: alignof '(' typename ')'  */
#line 478 "c-parse.y"
                { skip_evaluation--;
		  (yyval.ttype) = c_alignof (groktypename ((yyvsp[-1].ttype))); }
#line 3054 "c-p12907.c"
    break;

  case 54: /* unary_expr: REALPART cast_expr  */
#line 481 "c-parse.y"
                { (yyval.ttype) = build_unary_op (REALPART_EXPR, (yyvsp[0].ttype), 0); }
#line 3060 "c-p12907.c"
    break;

  case 55: /* unary_expr: IMAGPART cast_expr  */
#line 483 "c-parse.y"
                { (yyval.ttype) = build_unary_op (IMAGPART_EXPR, (yyvsp[0].ttype), 0); }
#line 3066 "c-p12907.c"
    break;

  case 56: /* sizeof: SIZEOF  */
#line 487 "c-parse.y"
               { skip_evaluation++; }
#line 3072 "c-p12907.c"
    break;

  case 57: /* alignof: ALIGNOF  */
#line 491 "c-parse.y"
                { skip_evaluation++; }
#line 3078 "c-p12907.c"
    break;

  case 59: /* cast_expr: '(' typename ')' cast_expr  */
#line 497 "c-parse.y"
                { (yyval.ttype) = c_cast_expr ((yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3084 "c-p12907.c"
    break;

  case 61: /* expr_no_commas: expr_no_commas '+' expr_no_commas  */
#line 503 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3090 "c-p12907.c"
    break;

  case 62: /* expr_no_commas: expr_no_commas '-' expr_no_commas  */
#line 505 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3096 "c-p12907.c"
    break;

  case 63: /* expr_no_commas: expr_no_commas '*' expr_no_commas  */
#line 507 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3102 "c-p12907.c"
    break;

  case 64: /* expr_no_commas: expr_no_commas '/' expr_no_commas  */
#line 509 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3108 "c-p12907.c"
    break;

  case 65: /* expr_no_commas: expr_no_commas '%' expr_no_commas  */
#line 511 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3114 "c-p12907.c"
    break;

  case 66: /* expr_no_commas: expr_no_commas LSHIFT expr_no_commas  */
#line 513 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3120 "c-p12907.c"
    break;

  case 67: /* expr_no_commas: expr_no_commas RSHIFT expr_no_commas  */
#line 515 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3126 "c-p12907.c"
    break;

  case 68: /* expr_no_commas: expr_no_commas ARITHCOMPARE expr_no_commas  */
#line 517 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3132 "c-p12907.c"
    break;

  case 69: /* expr_no_commas: expr_no_commas EQCOMPARE expr_no_commas  */
#line 519 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3138 "c-p12907.c"
    break;

  case 70: /* expr_no_commas: expr_no_commas '&' expr_no_commas  */
#line 521 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3144 "c-p12907.c"
    break;

  case 71: /* expr_no_commas: expr_no_commas '|' expr_no_commas  */
#line 523 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3150 "c-p12907.c"
    break;

  case 72: /* expr_no_commas: expr_no_commas '^' expr_no_commas  */
#line 525 "c-parse.y"
                { (yyval.ttype) = parser_build_binary_op ((yyvsp[-1].code), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 3156 "c-p12907.c"
    break;

  case 73: /* $@9: %empty  */
#line 527 "c-parse.y"
                { (yyvsp[-1].ttype) = c_common_truthvalue_conversion
		    (default_conversion ((yyvsp[-1].ttype)));
		  skip_evaluation += (yyvsp[-1].ttype) == boolean_false_node; }
#line 3164 "c-p12907.c"
    break;

  case 74: /* expr_no_commas: expr_no_commas ANDAND $@9 expr_no_commas  */
#line 531 "c-parse.y"
                { skip_evaluation -= (yyvsp[-3].ttype) == boolean_false_node;
		  (yyval.ttype) = parser_build_binary_op (TRUTH_ANDIF_EXPR, (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
#line 3171 "c-p12907.c"
    break;

  case 75: /* $@10: %empty  */
#line 534 "c-parse.y"
                { (yyvsp[-1].ttype) = c_common_truthvalue_conversion
		    (default_conversion ((yyvsp[-1].ttype)));
		  skip_evaluation += (yyvsp[-1].ttype) == boolean_true_node; }
#line 3179 "c-p12907.c"
    break;

  case 76: /* expr_no_commas: expr_no_commas OROR $@10 expr_no_commas  */
#line 538 "c-parse.y"
                { skip_evaluation -= (yyvsp[-3].ttype) == boolean_true_node;
		  (yyval.ttype) = parser_build_binary_op (TRUTH_ORIF_EXPR, (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
#line 3186 "c-p12907.c"
    break;

  case 77: /* $@11: %empty  */
#line 541 "c-parse.y"
                { (yyvsp[-1].ttype) = c_common_truthvalue_conversion
		    (default_conversion ((yyvsp[-1].ttype)));
		  skip_evaluation += (yyvsp[-1].ttype) == boolean_false_node; }
#line 3194 "c-p12907.c"
    break;

  case 78: /* $@12: %empty  */
#line 545 "c-parse.y"
                { skip_evaluation += (((yyvsp[-4].ttype) == boolean_true_node)
				      - ((yyvsp[-4].ttype) == boolean_false_node)); }
#line 3201 "c-p12907.c"
    break;

  case 79: /* expr_no_commas: expr_no_commas '?' $@11 expr ':' $@12 expr_no_commas  */
#line 548 "c-parse.y"
                { skip_evaluation -= (yyvsp[-6].ttype) == boolean_true_node;
		  (yyval.ttype) = build_conditional_expr ((yyvsp[-6].ttype), (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
#line 3208 "c-p12907.c"
    break;

  case 80: /* $@13: %empty  */
#line 551 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids omitting the middle term of a ?: expression");
		  /* Make sure first operand is calculated only once.  */
		  (yyvsp[0].ttype) = save_expr ((yyvsp[-1].ttype));
		  (yyvsp[-1].ttype) = c_common_truthvalue_conversion
		    (default_conversion ((yyvsp[0].ttype)));
		  skip_evaluation += (yyvsp[-1].ttype) == boolean_true_node; }
#line 3220 "c-p12907.c"
    break;

  case 81: /* expr_no_commas: expr_no_commas '?' $@13 ':' expr_no_commas  */
#line 559 "c-parse.y"
                { skip_evaluation -= (yyvsp[-4].ttype) == boolean_true_node;
		  (yyval.ttype) = build_conditional_expr ((yyvsp[-4].ttype), (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
#line 3227 "c-p12907.c"
    break;

  case 82: /* expr_no_commas: expr_no_commas '=' expr_no_commas  */
#line 562 "c-parse.y"
                { char class;
		  (yyval.ttype) = build_modify_expr ((yyvsp[-2].ttype), NOP_EXPR, (yyvsp[0].ttype));
		  class = TREE_CODE_CLASS (TREE_CODE ((yyval.ttype)));
		  if (IS_EXPR_CODE_CLASS (class))
		    C_SET_EXP_ORIGINAL_CODE ((yyval.ttype), MODIFY_EXPR);
		}
#line 3238 "c-p12907.c"
    break;

  case 83: /* expr_no_commas: expr_no_commas ASSIGN expr_no_commas  */
#line 569 "c-parse.y"
                { char class;
		  (yyval.ttype) = build_modify_expr ((yyvsp[-2].ttype), (yyvsp[-1].code), (yyvsp[0].ttype));
		  /* This inhibits warnings in
		     c_common_truthvalue_conversion.  */
		  class = TREE_CODE_CLASS (TREE_CODE ((yyval.ttype)));
		  if (IS_EXPR_CODE_CLASS (class))
		    C_SET_EXP_ORIGINAL_CODE ((yyval.ttype), ERROR_MARK);
		}
#line 3251 "c-p12907.c"
    break;

  case 84: /* primary: IDENTIFIER  */
#line 581 "c-parse.y"
                {
		  if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  (yyval.ttype) = build_external_ref ((yyvsp[0].ttype), yychar == '(');
		}
#line 3261 "c-p12907.c"
    break;

  case 86: /* primary: string  */
#line 588 "c-parse.y"
                { (yyval.ttype) = combine_strings ((yyvsp[0].ttype)); }
#line 3267 "c-p12907.c"
    break;

  case 87: /* primary: VAR_FUNC_NAME  */
#line 590 "c-parse.y"
                { (yyval.ttype) = fname_decl (C_RID_CODE ((yyval.ttype)), (yyval.ttype)); }
#line 3273 "c-p12907.c"
    break;

  case 88: /* $@14: %empty  */
#line 592 "c-parse.y"
                { start_init (NULL_TREE, NULL, 0);
		  (yyvsp[-2].ttype) = groktypename ((yyvsp[-2].ttype));
		  really_start_incremental_init ((yyvsp[-2].ttype)); }
#line 3281 "c-p12907.c"
    break;

  case 89: /* primary: '(' typename ')' '{' $@14 initlist_maybe_comma '}'  */
#line 596 "c-parse.y"
                { tree constructor = pop_init_level (0);
		  tree type = (yyvsp[-5].ttype);
		  finish_init ();

		  if (pedantic && ! flag_isoc99)
		    pedwarn ("ISO C89 forbids compound literals");
		  (yyval.ttype) = build_compound_literal (type, constructor);
		}
#line 3294 "c-p12907.c"
    break;

  case 90: /* primary: '(' expr ')'  */
#line 605 "c-parse.y"
                { char class = TREE_CODE_CLASS (TREE_CODE ((yyvsp[-1].ttype)));
		  if (IS_EXPR_CODE_CLASS (class))
		    C_SET_EXP_ORIGINAL_CODE ((yyvsp[-1].ttype), ERROR_MARK);
		  (yyval.ttype) = (yyvsp[-1].ttype); }
#line 3303 "c-p12907.c"
    break;

  case 91: /* primary: '(' error ')'  */
#line 610 "c-parse.y"
                { (yyval.ttype) = error_mark_node; }
#line 3309 "c-p12907.c"
    break;

  case 92: /* primary: compstmt_primary_start compstmt_nostart ')'  */
#line 612 "c-parse.y"
                 { tree saved_last_tree;

		   if (pedantic)
		     pedwarn ("ISO C forbids braced-groups within expressions");
		  pop_label_level ();

		  saved_last_tree = COMPOUND_BODY ((yyvsp[-2].ttype));
		  RECHAIN_STMTS ((yyvsp[-2].ttype), COMPOUND_BODY ((yyvsp[-2].ttype)));
		  last_tree = saved_last_tree;
		  TREE_CHAIN (last_tree) = NULL_TREE;
		  if (!last_expr_type)
		    last_expr_type = void_type_node;
		  (yyval.ttype) = build1 (STMT_EXPR, last_expr_type, (yyvsp[-2].ttype));
		  TREE_SIDE_EFFECTS ((yyval.ttype)) = 1;
		}
#line 3329 "c-p12907.c"
    break;

  case 93: /* primary: compstmt_primary_start error ')'  */
#line 628 "c-parse.y"
                {
		  pop_label_level ();
		  last_tree = COMPOUND_BODY ((yyvsp[-2].ttype));
		  TREE_CHAIN (last_tree) = NULL_TREE;
		  (yyval.ttype) = error_mark_node;
		}
#line 3340 "c-p12907.c"
    break;

  case 94: /* primary: primary '(' exprlist ')'  */
#line 635 "c-parse.y"
                { (yyval.ttype) = build_function_call ((yyvsp[-3].ttype), (yyvsp[-1].ttype)); }
#line 3346 "c-p12907.c"
    break;

  case 95: /* primary: VA_ARG '(' expr_no_commas ',' typename ')'  */
#line 637 "c-parse.y"
                { (yyval.ttype) = build_va_arg ((yyvsp[-3].ttype), groktypename ((yyvsp[-1].ttype))); }
#line 3352 "c-p12907.c"
    break;

  case 96: /* primary: CHOOSE_EXPR '(' expr_no_commas ',' expr_no_commas ',' expr_no_commas ')'  */
#line 640 "c-parse.y"
                {
                  tree c;

                  c = fold ((yyvsp[-5].ttype));
                  STRIP_NOPS (c);
                  if (TREE_CODE (c) != INTEGER_CST)
                    error ("first argument to __builtin_choose_expr not a constant");
                  (yyval.ttype) = integer_zerop (c) ? (yyvsp[-1].ttype) : (yyvsp[-3].ttype);
		}
#line 3366 "c-p12907.c"
    break;

  case 97: /* primary: TYPES_COMPATIBLE_P '(' typename ',' typename ')'  */
#line 650 "c-parse.y"
                {
		  tree e1, e2;

		  e1 = TYPE_MAIN_VARIANT (groktypename ((yyvsp[-3].ttype)));
		  e2 = TYPE_MAIN_VARIANT (groktypename ((yyvsp[-1].ttype)));

		  (yyval.ttype) = comptypes (e1, e2)
		    ? build_int_2 (1, 0) : build_int_2 (0, 0);
		}
#line 3380 "c-p12907.c"
    break;

  case 98: /* primary: primary '[' expr ']'  */
#line 660 "c-parse.y"
                { (yyval.ttype) = build_array_ref ((yyvsp[-3].ttype), (yyvsp[-1].ttype)); }
#line 3386 "c-p12907.c"
    break;

  case 99: /* primary: primary '.' identifier  */
#line 662 "c-parse.y"
                {
		      (yyval.ttype) = build_component_ref ((yyvsp[-2].ttype), (yyvsp[0].ttype));
		}
#line 3394 "c-p12907.c"
    break;

  case 100: /* primary: primary POINTSAT identifier  */
#line 666 "c-parse.y"
                {
                  tree expr = build_indirect_ref ((yyvsp[-2].ttype), "->");

			(yyval.ttype) = build_component_ref (expr, (yyvsp[0].ttype));
		}
#line 3404 "c-p12907.c"
    break;

  case 101: /* primary: primary PLUSPLUS  */
#line 672 "c-parse.y"
                { (yyval.ttype) = build_unary_op (POSTINCREMENT_EXPR, (yyvsp[-1].ttype), 0); }
#line 3410 "c-p12907.c"
    break;

  case 102: /* primary: primary MINUSMINUS  */
#line 674 "c-parse.y"
                { (yyval.ttype) = build_unary_op (POSTDECREMENT_EXPR, (yyvsp[-1].ttype), 0); }
#line 3416 "c-p12907.c"
    break;

  case 104: /* string: string STRING  */
#line 681 "c-parse.y"
                {
                  static int last_lineno = 0;
                  static const char *last_input_filename = 0;
                  (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[0].ttype));
		  if (warn_traditional && !in_system_header
		      && (lineno != last_lineno || !last_input_filename ||
			  strcmp (last_input_filename, input_filename)))
		    {
		      warning ("traditional C rejects string concatenation");
		      last_lineno = lineno;
		      last_input_filename = input_filename;
		    }
		}
#line 3434 "c-p12907.c"
    break;

  case 107: /* old_style_parm_decls: datadecls ELLIPSIS  */
#line 702 "c-parse.y"
                { c_mark_varargs ();
		  if (pedantic)
		    pedwarn ("ISO C does not permit use of `varargs.h'"); }
#line 3442 "c-p12907.c"
    break;

  case 108: /* lineno_datadecl: save_filename save_lineno datadecl  */
#line 712 "c-parse.y"
                { }
#line 3448 "c-p12907.c"
    break;

  case 113: /* datadecl: declspecs_ts_nosa setspecs initdecls ';'  */
#line 728 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 3454 "c-p12907.c"
    break;

  case 114: /* datadecl: declspecs_nots_nosa setspecs notype_initdecls ';'  */
#line 730 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 3460 "c-p12907.c"
    break;

  case 115: /* datadecl: declspecs_ts_nosa ';'  */
#line 732 "c-parse.y"
                { shadow_tag_warned ((yyvsp[-1].ttype), 1);
		  pedwarn ("empty declaration"); }
#line 3467 "c-p12907.c"
    break;

  case 116: /* datadecl: declspecs_nots_nosa ';'  */
#line 735 "c-parse.y"
                { pedwarn ("empty declaration"); }
#line 3473 "c-p12907.c"
    break;

  case 117: /* lineno_decl: save_filename save_lineno decl  */
#line 744 "c-parse.y"
                { }
#line 3479 "c-p12907.c"
    break;

  case 118: /* setspecs: %empty  */
#line 752 "c-parse.y"
                { pending_xref_error ();
		  PUSH_DECLSPEC_STACK;
		  split_specs_attrs ((yyvsp[0].ttype),
				     &current_declspecs, &prefix_attributes);
		  all_prefix_attributes = prefix_attributes; }
#line 3489 "c-p12907.c"
    break;

  case 119: /* maybe_resetattrs: maybe_attribute  */
#line 763 "c-parse.y"
                { all_prefix_attributes = chainon ((yyvsp[0].ttype), prefix_attributes); }
#line 3495 "c-p12907.c"
    break;

  case 120: /* decl: declspecs_ts setspecs initdecls ';'  */
#line 768 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 3501 "c-p12907.c"
    break;

  case 121: /* decl: declspecs_nots setspecs notype_initdecls ';'  */
#line 770 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 3507 "c-p12907.c"
    break;

  case 122: /* decl: declspecs_ts setspecs nested_function  */
#line 772 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 3513 "c-p12907.c"
    break;

  case 123: /* decl: declspecs_nots setspecs notype_nested_function  */
#line 774 "c-parse.y"
                { POP_DECLSPEC_STACK; }
#line 3519 "c-p12907.c"
    break;

  case 124: /* decl: declspecs ';'  */
#line 776 "c-parse.y"
                { shadow_tag ((yyvsp[-1].ttype)); }
#line 3525 "c-p12907.c"
    break;

  case 125: /* decl: extension decl  */
#line 778 "c-parse.y"
                { RESTORE_WARN_FLAGS ((yyvsp[-1].ttype)); }
#line 3531 "c-p12907.c"
    break;

  case 126: /* declspecs_nosc_nots_nosa_noea: TYPE_QUAL  */
#line 835 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), NULL_TREE);
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3538 "c-p12907.c"
    break;

  case 127: /* declspecs_nosc_nots_nosa_noea: declspecs_nosc_nots_nosa_noea TYPE_QUAL  */
#line 838 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3545 "c-p12907.c"
    break;

  case 128: /* declspecs_nosc_nots_nosa_noea: declspecs_nosc_nots_nosa_ea TYPE_QUAL  */
#line 841 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3552 "c-p12907.c"
    break;

  case 129: /* declspecs_nosc_nots_nosa_ea: declspecs_nosc_nots_nosa_noea attributes  */
#line 847 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3559 "c-p12907.c"
    break;

  case 130: /* declspecs_nosc_nots_sa_noea: declspecs_nosc_nots_sa_noea TYPE_QUAL  */
#line 853 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3566 "c-p12907.c"
    break;

  case 131: /* declspecs_nosc_nots_sa_noea: declspecs_nosc_nots_sa_ea TYPE_QUAL  */
#line 856 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3573 "c-p12907.c"
    break;

  case 132: /* declspecs_nosc_nots_sa_ea: attributes  */
#line 862 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, NULL_TREE);
		  TREE_STATIC ((yyval.ttype)) = 0; }
#line 3580 "c-p12907.c"
    break;

  case 133: /* declspecs_nosc_nots_sa_ea: declspecs_nosc_nots_sa_noea attributes  */
#line 865 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3587 "c-p12907.c"
    break;

  case 134: /* declspecs_nosc_ts_nosa_noea: typespec_nonattr  */
#line 871 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), NULL_TREE);
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3594 "c-p12907.c"
    break;

  case 135: /* declspecs_nosc_ts_nosa_noea: declspecs_nosc_ts_nosa_noea TYPE_QUAL  */
#line 874 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3601 "c-p12907.c"
    break;

  case 136: /* declspecs_nosc_ts_nosa_noea: declspecs_nosc_ts_nosa_ea TYPE_QUAL  */
#line 877 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3608 "c-p12907.c"
    break;

  case 137: /* declspecs_nosc_ts_nosa_noea: declspecs_nosc_ts_nosa_noea typespec_reserved_nonattr  */
#line 880 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3615 "c-p12907.c"
    break;

  case 138: /* declspecs_nosc_ts_nosa_noea: declspecs_nosc_ts_nosa_ea typespec_reserved_nonattr  */
#line 883 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3622 "c-p12907.c"
    break;

  case 139: /* declspecs_nosc_ts_nosa_noea: declspecs_nosc_nots_nosa_noea typespec_nonattr  */
#line 886 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3629 "c-p12907.c"
    break;

  case 140: /* declspecs_nosc_ts_nosa_noea: declspecs_nosc_nots_nosa_ea typespec_nonattr  */
#line 889 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3636 "c-p12907.c"
    break;

  case 141: /* declspecs_nosc_ts_nosa_ea: typespec_attr  */
#line 895 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), NULL_TREE);
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3643 "c-p12907.c"
    break;

  case 142: /* declspecs_nosc_ts_nosa_ea: declspecs_nosc_ts_nosa_noea attributes  */
#line 898 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3650 "c-p12907.c"
    break;

  case 143: /* declspecs_nosc_ts_nosa_ea: declspecs_nosc_ts_nosa_noea typespec_reserved_attr  */
#line 901 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3657 "c-p12907.c"
    break;

  case 144: /* declspecs_nosc_ts_nosa_ea: declspecs_nosc_ts_nosa_ea typespec_reserved_attr  */
#line 904 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3664 "c-p12907.c"
    break;

  case 145: /* declspecs_nosc_ts_nosa_ea: declspecs_nosc_nots_nosa_noea typespec_attr  */
#line 907 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3671 "c-p12907.c"
    break;

  case 146: /* declspecs_nosc_ts_nosa_ea: declspecs_nosc_nots_nosa_ea typespec_attr  */
#line 910 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3678 "c-p12907.c"
    break;

  case 147: /* declspecs_nosc_ts_sa_noea: declspecs_nosc_ts_sa_noea TYPE_QUAL  */
#line 916 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3685 "c-p12907.c"
    break;

  case 148: /* declspecs_nosc_ts_sa_noea: declspecs_nosc_ts_sa_ea TYPE_QUAL  */
#line 919 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3692 "c-p12907.c"
    break;

  case 149: /* declspecs_nosc_ts_sa_noea: declspecs_nosc_ts_sa_noea typespec_reserved_nonattr  */
#line 922 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3699 "c-p12907.c"
    break;

  case 150: /* declspecs_nosc_ts_sa_noea: declspecs_nosc_ts_sa_ea typespec_reserved_nonattr  */
#line 925 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3706 "c-p12907.c"
    break;

  case 151: /* declspecs_nosc_ts_sa_noea: declspecs_nosc_nots_sa_noea typespec_nonattr  */
#line 928 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3713 "c-p12907.c"
    break;

  case 152: /* declspecs_nosc_ts_sa_noea: declspecs_nosc_nots_sa_ea typespec_nonattr  */
#line 931 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3720 "c-p12907.c"
    break;

  case 153: /* declspecs_nosc_ts_sa_ea: declspecs_nosc_ts_sa_noea attributes  */
#line 937 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3727 "c-p12907.c"
    break;

  case 154: /* declspecs_nosc_ts_sa_ea: declspecs_nosc_ts_sa_noea typespec_reserved_attr  */
#line 940 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3734 "c-p12907.c"
    break;

  case 155: /* declspecs_nosc_ts_sa_ea: declspecs_nosc_ts_sa_ea typespec_reserved_attr  */
#line 943 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3741 "c-p12907.c"
    break;

  case 156: /* declspecs_nosc_ts_sa_ea: declspecs_nosc_nots_sa_noea typespec_attr  */
#line 946 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3748 "c-p12907.c"
    break;

  case 157: /* declspecs_nosc_ts_sa_ea: declspecs_nosc_nots_sa_ea typespec_attr  */
#line 949 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3755 "c-p12907.c"
    break;

  case 158: /* declspecs_sc_nots_nosa_noea: scspec  */
#line 955 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), NULL_TREE);
		  TREE_STATIC ((yyval.ttype)) = 0; }
#line 3762 "c-p12907.c"
    break;

  case 159: /* declspecs_sc_nots_nosa_noea: declspecs_sc_nots_nosa_noea TYPE_QUAL  */
#line 958 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3769 "c-p12907.c"
    break;

  case 160: /* declspecs_sc_nots_nosa_noea: declspecs_sc_nots_nosa_ea TYPE_QUAL  */
#line 961 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3776 "c-p12907.c"
    break;

  case 161: /* declspecs_sc_nots_nosa_noea: declspecs_nosc_nots_nosa_noea scspec  */
#line 964 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3786 "c-p12907.c"
    break;

  case 162: /* declspecs_sc_nots_nosa_noea: declspecs_nosc_nots_nosa_ea scspec  */
#line 970 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3796 "c-p12907.c"
    break;

  case 163: /* declspecs_sc_nots_nosa_noea: declspecs_sc_nots_nosa_noea scspec  */
#line 976 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3806 "c-p12907.c"
    break;

  case 164: /* declspecs_sc_nots_nosa_noea: declspecs_sc_nots_nosa_ea scspec  */
#line 982 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3816 "c-p12907.c"
    break;

  case 165: /* declspecs_sc_nots_nosa_ea: declspecs_sc_nots_nosa_noea attributes  */
#line 991 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3823 "c-p12907.c"
    break;

  case 166: /* declspecs_sc_nots_sa_noea: declspecs_sc_nots_sa_noea TYPE_QUAL  */
#line 997 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3830 "c-p12907.c"
    break;

  case 167: /* declspecs_sc_nots_sa_noea: declspecs_sc_nots_sa_ea TYPE_QUAL  */
#line 1000 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3837 "c-p12907.c"
    break;

  case 168: /* declspecs_sc_nots_sa_noea: declspecs_nosc_nots_sa_noea scspec  */
#line 1003 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3847 "c-p12907.c"
    break;

  case 169: /* declspecs_sc_nots_sa_noea: declspecs_nosc_nots_sa_ea scspec  */
#line 1009 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3857 "c-p12907.c"
    break;

  case 170: /* declspecs_sc_nots_sa_noea: declspecs_sc_nots_sa_noea scspec  */
#line 1015 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3867 "c-p12907.c"
    break;

  case 171: /* declspecs_sc_nots_sa_noea: declspecs_sc_nots_sa_ea scspec  */
#line 1021 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3877 "c-p12907.c"
    break;

  case 172: /* declspecs_sc_nots_sa_ea: declspecs_sc_nots_sa_noea attributes  */
#line 1030 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3884 "c-p12907.c"
    break;

  case 173: /* declspecs_sc_ts_nosa_noea: declspecs_sc_ts_nosa_noea TYPE_QUAL  */
#line 1036 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3891 "c-p12907.c"
    break;

  case 174: /* declspecs_sc_ts_nosa_noea: declspecs_sc_ts_nosa_ea TYPE_QUAL  */
#line 1039 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3898 "c-p12907.c"
    break;

  case 175: /* declspecs_sc_ts_nosa_noea: declspecs_sc_ts_nosa_noea typespec_reserved_nonattr  */
#line 1042 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3905 "c-p12907.c"
    break;

  case 176: /* declspecs_sc_ts_nosa_noea: declspecs_sc_ts_nosa_ea typespec_reserved_nonattr  */
#line 1045 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3912 "c-p12907.c"
    break;

  case 177: /* declspecs_sc_ts_nosa_noea: declspecs_sc_nots_nosa_noea typespec_nonattr  */
#line 1048 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3919 "c-p12907.c"
    break;

  case 178: /* declspecs_sc_ts_nosa_noea: declspecs_sc_nots_nosa_ea typespec_nonattr  */
#line 1051 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3926 "c-p12907.c"
    break;

  case 179: /* declspecs_sc_ts_nosa_noea: declspecs_nosc_ts_nosa_noea scspec  */
#line 1054 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3936 "c-p12907.c"
    break;

  case 180: /* declspecs_sc_ts_nosa_noea: declspecs_nosc_ts_nosa_ea scspec  */
#line 1060 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3946 "c-p12907.c"
    break;

  case 181: /* declspecs_sc_ts_nosa_noea: declspecs_sc_ts_nosa_noea scspec  */
#line 1066 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3956 "c-p12907.c"
    break;

  case 182: /* declspecs_sc_ts_nosa_noea: declspecs_sc_ts_nosa_ea scspec  */
#line 1072 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3966 "c-p12907.c"
    break;

  case 183: /* declspecs_sc_ts_nosa_ea: declspecs_sc_ts_nosa_noea attributes  */
#line 1081 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 3973 "c-p12907.c"
    break;

  case 184: /* declspecs_sc_ts_nosa_ea: declspecs_sc_ts_nosa_noea typespec_reserved_attr  */
#line 1084 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3980 "c-p12907.c"
    break;

  case 185: /* declspecs_sc_ts_nosa_ea: declspecs_sc_ts_nosa_ea typespec_reserved_attr  */
#line 1087 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3987 "c-p12907.c"
    break;

  case 186: /* declspecs_sc_ts_nosa_ea: declspecs_sc_nots_nosa_noea typespec_attr  */
#line 1090 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 3994 "c-p12907.c"
    break;

  case 187: /* declspecs_sc_ts_nosa_ea: declspecs_sc_nots_nosa_ea typespec_attr  */
#line 1093 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4001 "c-p12907.c"
    break;

  case 188: /* declspecs_sc_ts_sa_noea: declspecs_sc_ts_sa_noea TYPE_QUAL  */
#line 1099 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4008 "c-p12907.c"
    break;

  case 189: /* declspecs_sc_ts_sa_noea: declspecs_sc_ts_sa_ea TYPE_QUAL  */
#line 1102 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4015 "c-p12907.c"
    break;

  case 190: /* declspecs_sc_ts_sa_noea: declspecs_sc_ts_sa_noea typespec_reserved_nonattr  */
#line 1105 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4022 "c-p12907.c"
    break;

  case 191: /* declspecs_sc_ts_sa_noea: declspecs_sc_ts_sa_ea typespec_reserved_nonattr  */
#line 1108 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4029 "c-p12907.c"
    break;

  case 192: /* declspecs_sc_ts_sa_noea: declspecs_sc_nots_sa_noea typespec_nonattr  */
#line 1111 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4036 "c-p12907.c"
    break;

  case 193: /* declspecs_sc_ts_sa_noea: declspecs_sc_nots_sa_ea typespec_nonattr  */
#line 1114 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4043 "c-p12907.c"
    break;

  case 194: /* declspecs_sc_ts_sa_noea: declspecs_nosc_ts_sa_noea scspec  */
#line 1117 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 4053 "c-p12907.c"
    break;

  case 195: /* declspecs_sc_ts_sa_noea: declspecs_nosc_ts_sa_ea scspec  */
#line 1123 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 4063 "c-p12907.c"
    break;

  case 196: /* declspecs_sc_ts_sa_noea: declspecs_sc_ts_sa_noea scspec  */
#line 1129 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 4073 "c-p12907.c"
    break;

  case 197: /* declspecs_sc_ts_sa_noea: declspecs_sc_ts_sa_ea scspec  */
#line 1135 "c-parse.y"
                { if (extra_warnings && TREE_STATIC ((yyvsp[-1].ttype)))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER ((yyvsp[0].ttype)));
		  (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 4083 "c-p12907.c"
    break;

  case 198: /* declspecs_sc_ts_sa_ea: declspecs_sc_ts_sa_noea attributes  */
#line 1144 "c-parse.y"
                { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = TREE_STATIC ((yyvsp[-1].ttype)); }
#line 4090 "c-p12907.c"
    break;

  case 199: /* declspecs_sc_ts_sa_ea: declspecs_sc_ts_sa_noea typespec_reserved_attr  */
#line 1147 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4097 "c-p12907.c"
    break;

  case 200: /* declspecs_sc_ts_sa_ea: declspecs_sc_ts_sa_ea typespec_reserved_attr  */
#line 1150 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4104 "c-p12907.c"
    break;

  case 201: /* declspecs_sc_ts_sa_ea: declspecs_sc_nots_sa_noea typespec_attr  */
#line 1153 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4111 "c-p12907.c"
    break;

  case 202: /* declspecs_sc_ts_sa_ea: declspecs_sc_nots_sa_ea typespec_attr  */
#line 1156 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, (yyvsp[0].ttype), (yyvsp[-1].ttype));
		  TREE_STATIC ((yyval.ttype)) = 1; }
#line 4118 "c-p12907.c"
    break;

  case 259: /* maybe_type_quals_attrs: %empty  */
#line 1244 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4124 "c-p12907.c"
    break;

  case 260: /* maybe_type_quals_attrs: declspecs_nosc_nots  */
#line 1246 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4130 "c-p12907.c"
    break;

  case 264: /* typespec_reserved_nonattr: TYPESPEC  */
#line 1281 "c-parse.y"
                { OBJC_NEED_RAW_IDENTIFIER (1);	}
#line 4136 "c-p12907.c"
    break;

  case 267: /* typespec_nonreserved_nonattr: TYPENAME  */
#line 1291 "c-parse.y"
                { /* For a typedef name, record the meaning, not the name.
		     In case of `foo foo, bar;'.  */
		  (yyval.ttype) = lookup_name ((yyvsp[0].ttype)); }
#line 4144 "c-p12907.c"
    break;

  case 268: /* typespec_nonreserved_nonattr: TYPEOF '(' expr ')'  */
#line 1295 "c-parse.y"
                { (yyval.ttype) = TREE_TYPE ((yyvsp[-1].ttype)); }
#line 4150 "c-p12907.c"
    break;

  case 269: /* typespec_nonreserved_nonattr: TYPEOF '(' typename ')'  */
#line 1297 "c-parse.y"
                { (yyval.ttype) = groktypename ((yyvsp[-1].ttype)); }
#line 4156 "c-p12907.c"
    break;

  case 274: /* maybeasm: %empty  */
#line 1314 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4162 "c-p12907.c"
    break;

  case 275: /* maybeasm: ASM_KEYWORD '(' string ')'  */
#line 1316 "c-parse.y"
                { if (TREE_CHAIN ((yyvsp[-1].ttype))) (yyvsp[-1].ttype) = combine_strings ((yyvsp[-1].ttype));
		  (yyval.ttype) = (yyvsp[-1].ttype);
		}
#line 4170 "c-p12907.c"
    break;

  case 276: /* @15: %empty  */
#line 1323 "c-parse.y"
                { (yyval.ttype) = start_decl ((yyvsp[-3].ttype), current_declspecs, 1,
					  chainon ((yyvsp[-1].ttype), all_prefix_attributes));
		  start_init ((yyval.ttype), (yyvsp[-2].ttype), global_bindings_p ()); }
#line 4178 "c-p12907.c"
    break;

  case 277: /* initdcl: declarator maybeasm maybe_attribute '=' @15 init  */
#line 1328 "c-parse.y"
                { finish_init ();
		  finish_decl ((yyvsp[-1].ttype), (yyvsp[0].ttype), (yyvsp[-4].ttype)); }
#line 4185 "c-p12907.c"
    break;

  case 278: /* initdcl: declarator maybeasm maybe_attribute  */
#line 1331 "c-parse.y"
                { tree d = start_decl ((yyvsp[-2].ttype), current_declspecs, 0,
				       chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  finish_decl (d, NULL_TREE, (yyvsp[-1].ttype)); 
                }
#line 4194 "c-p12907.c"
    break;

  case 279: /* @16: %empty  */
#line 1339 "c-parse.y"
                { (yyval.ttype) = start_decl ((yyvsp[-3].ttype), current_declspecs, 1,
					  chainon ((yyvsp[-1].ttype), all_prefix_attributes));
		  start_init ((yyval.ttype), (yyvsp[-2].ttype), global_bindings_p ()); }
#line 4202 "c-p12907.c"
    break;

  case 280: /* notype_initdcl: notype_declarator maybeasm maybe_attribute '=' @16 init  */
#line 1344 "c-parse.y"
                { finish_init ();
		  finish_decl ((yyvsp[-1].ttype), (yyvsp[0].ttype), (yyvsp[-4].ttype)); }
#line 4209 "c-p12907.c"
    break;

  case 281: /* notype_initdcl: notype_declarator maybeasm maybe_attribute  */
#line 1347 "c-parse.y"
                { tree d = start_decl ((yyvsp[-2].ttype), current_declspecs, 0,
				       chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  finish_decl (d, NULL_TREE, (yyvsp[-1].ttype)); }
#line 4217 "c-p12907.c"
    break;

  case 282: /* maybe_attribute: %empty  */
#line 1355 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4223 "c-p12907.c"
    break;

  case 283: /* maybe_attribute: attributes  */
#line 1357 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4229 "c-p12907.c"
    break;

  case 284: /* attributes: attribute  */
#line 1362 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4235 "c-p12907.c"
    break;

  case 285: /* attributes: attributes attribute  */
#line 1364 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4241 "c-p12907.c"
    break;

  case 286: /* attribute: ATTRIBUTE '(' '(' attribute_list ')' ')'  */
#line 1369 "c-parse.y"
                { (yyval.ttype) = (yyvsp[-2].ttype); }
#line 4247 "c-p12907.c"
    break;

  case 287: /* attribute_list: attrib  */
#line 1374 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4253 "c-p12907.c"
    break;

  case 288: /* attribute_list: attribute_list ',' attrib  */
#line 1376 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 4259 "c-p12907.c"
    break;

  case 289: /* attrib: %empty  */
#line 1381 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4265 "c-p12907.c"
    break;

  case 290: /* attrib: any_word  */
#line 1383 "c-parse.y"
                { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
#line 4271 "c-p12907.c"
    break;

  case 291: /* attrib: any_word '(' IDENTIFIER ')'  */
#line 1385 "c-parse.y"
                { (yyval.ttype) = build_tree_list ((yyvsp[-3].ttype), build_tree_list (NULL_TREE, (yyvsp[-1].ttype))); }
#line 4277 "c-p12907.c"
    break;

  case 292: /* attrib: any_word '(' IDENTIFIER ',' nonnull_exprlist ')'  */
#line 1387 "c-parse.y"
                { (yyval.ttype) = build_tree_list ((yyvsp[-5].ttype), tree_cons (NULL_TREE, (yyvsp[-3].ttype), (yyvsp[-1].ttype))); }
#line 4283 "c-p12907.c"
    break;

  case 293: /* attrib: any_word '(' exprlist ')'  */
#line 1389 "c-parse.y"
                { (yyval.ttype) = build_tree_list ((yyvsp[-3].ttype), (yyvsp[-1].ttype)); }
#line 4289 "c-p12907.c"
    break;

  case 301: /* $@17: %empty  */
#line 1412 "c-parse.y"
                { really_start_incremental_init (NULL_TREE); }
#line 4295 "c-p12907.c"
    break;

  case 302: /* init: '{' $@17 initlist_maybe_comma '}'  */
#line 1414 "c-parse.y"
                { (yyval.ttype) = pop_init_level (0); }
#line 4301 "c-p12907.c"
    break;

  case 303: /* init: error  */
#line 1416 "c-parse.y"
                { (yyval.ttype) = error_mark_node; }
#line 4307 "c-p12907.c"
    break;

  case 304: /* initlist_maybe_comma: %empty  */
#line 1422 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids empty initializer braces"); }
#line 4314 "c-p12907.c"
    break;

  case 308: /* initelt: designator_list '=' initval  */
#line 1436 "c-parse.y"
                { if (pedantic && ! flag_isoc99)
		    pedwarn ("ISO C89 forbids specifying subobject to initialize"); }
#line 4321 "c-p12907.c"
    break;

  case 309: /* initelt: designator initval  */
#line 1439 "c-parse.y"
                { if (pedantic)
		    pedwarn ("obsolete use of designated initializer without `='"); }
#line 4328 "c-p12907.c"
    break;

  case 310: /* $@18: %empty  */
#line 1442 "c-parse.y"
                { set_init_label ((yyvsp[-1].ttype));
		  if (pedantic)
		    pedwarn ("obsolete use of designated initializer with `:'"); }
#line 4336 "c-p12907.c"
    break;

  case 313: /* $@19: %empty  */
#line 1451 "c-parse.y"
                { push_init_level (0); }
#line 4342 "c-p12907.c"
    break;

  case 314: /* initval: '{' $@19 initlist_maybe_comma '}'  */
#line 1453 "c-parse.y"
                { process_init_element (pop_init_level (0)); }
#line 4348 "c-p12907.c"
    break;

  case 315: /* initval: expr_no_commas  */
#line 1455 "c-parse.y"
                { process_init_element ((yyvsp[0].ttype)); }
#line 4354 "c-p12907.c"
    break;

  case 319: /* designator: '.' identifier  */
#line 1466 "c-parse.y"
                { set_init_label ((yyvsp[0].ttype)); }
#line 4360 "c-p12907.c"
    break;

  case 320: /* designator: '[' expr_no_commas ELLIPSIS expr_no_commas ']'  */
#line 1471 "c-parse.y"
                { set_init_index ((yyvsp[-3].ttype), (yyvsp[-1].ttype));
		  if (pedantic)
		    pedwarn ("ISO C forbids specifying range of elements to initialize"); }
#line 4368 "c-p12907.c"
    break;

  case 321: /* designator: '[' expr_no_commas ']'  */
#line 1475 "c-parse.y"
                { set_init_index ((yyvsp[-1].ttype), NULL_TREE); }
#line 4374 "c-p12907.c"
    break;

  case 322: /* $@20: %empty  */
#line 1480 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids nested functions");

		  push_function_context ();
		  if (! start_function (current_declspecs, (yyvsp[0].ttype),
					all_prefix_attributes))
		    {
		      pop_function_context ();
		      YYERROR1;
		    }
		}
#line 4390 "c-p12907.c"
    break;

  case 323: /* $@21: %empty  */
#line 1492 "c-parse.y"
                { store_parm_decls (); }
#line 4396 "c-p12907.c"
    break;

  case 324: /* nested_function: declarator $@20 old_style_parm_decls $@21 save_filename save_lineno compstmt  */
#line 1500 "c-parse.y"
                { tree decl = current_function_decl;
		  DECL_SOURCE_FILE (decl) = (yyvsp[-2].filename);
		  DECL_SOURCE_LINE (decl) = (yyvsp[-1].lineno);
		  finish_function (1, 1);
		  pop_function_context (); 
		  add_decl_stmt (decl); }
#line 4407 "c-p12907.c"
    break;

  case 325: /* $@22: %empty  */
#line 1510 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids nested functions");

		  push_function_context ();
		  if (! start_function (current_declspecs, (yyvsp[0].ttype),
					all_prefix_attributes))
		    {
		      pop_function_context ();
		      YYERROR1;
		    }
		}
#line 4423 "c-p12907.c"
    break;

  case 326: /* $@23: %empty  */
#line 1522 "c-parse.y"
                { store_parm_decls (); }
#line 4429 "c-p12907.c"
    break;

  case 327: /* notype_nested_function: notype_declarator $@22 old_style_parm_decls $@23 save_filename save_lineno compstmt  */
#line 1530 "c-parse.y"
                { tree decl = current_function_decl;
		  DECL_SOURCE_FILE (decl) = (yyvsp[-2].filename);
		  DECL_SOURCE_LINE (decl) = (yyvsp[-1].lineno);
		  finish_function (1, 1);
		  pop_function_context (); 
		  add_decl_stmt (decl); }
#line 4440 "c-p12907.c"
    break;

  case 330: /* after_type_declarator: '(' maybe_attribute after_type_declarator ')'  */
#line 1550 "c-parse.y"
                { (yyval.ttype) = (yyvsp[-2].ttype) ? tree_cons ((yyvsp[-2].ttype), (yyvsp[-1].ttype), NULL_TREE) : (yyvsp[-1].ttype); }
#line 4446 "c-p12907.c"
    break;

  case 331: /* after_type_declarator: after_type_declarator '(' parmlist_or_identifiers  */
#line 1552 "c-parse.y"
                { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL_TREE); }
#line 4452 "c-p12907.c"
    break;

  case 332: /* after_type_declarator: after_type_declarator array_declarator  */
#line 1557 "c-parse.y"
                { (yyval.ttype) = set_array_declarator_type ((yyvsp[0].ttype), (yyvsp[-1].ttype), 0); }
#line 4458 "c-p12907.c"
    break;

  case 333: /* after_type_declarator: '*' maybe_type_quals_attrs after_type_declarator  */
#line 1559 "c-parse.y"
                { (yyval.ttype) = make_pointer_declarator ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4464 "c-p12907.c"
    break;

  case 337: /* parm_declarator_starttypename: parm_declarator_starttypename '(' parmlist_or_identifiers  */
#line 1574 "c-parse.y"
                { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL_TREE); }
#line 4470 "c-p12907.c"
    break;

  case 338: /* parm_declarator_starttypename: parm_declarator_starttypename array_declarator  */
#line 1579 "c-parse.y"
                { (yyval.ttype) = set_array_declarator_type ((yyvsp[0].ttype), (yyvsp[-1].ttype), 0); }
#line 4476 "c-p12907.c"
    break;

  case 340: /* parm_declarator_nostarttypename: parm_declarator_nostarttypename '(' parmlist_or_identifiers  */
#line 1585 "c-parse.y"
                { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL_TREE); }
#line 4482 "c-p12907.c"
    break;

  case 341: /* parm_declarator_nostarttypename: parm_declarator_nostarttypename array_declarator  */
#line 1590 "c-parse.y"
                { (yyval.ttype) = set_array_declarator_type ((yyvsp[0].ttype), (yyvsp[-1].ttype), 0); }
#line 4488 "c-p12907.c"
    break;

  case 342: /* parm_declarator_nostarttypename: '*' maybe_type_quals_attrs parm_declarator_starttypename  */
#line 1592 "c-parse.y"
                { (yyval.ttype) = make_pointer_declarator ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4494 "c-p12907.c"
    break;

  case 343: /* parm_declarator_nostarttypename: '*' maybe_type_quals_attrs parm_declarator_nostarttypename  */
#line 1594 "c-parse.y"
                { (yyval.ttype) = make_pointer_declarator ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4500 "c-p12907.c"
    break;

  case 344: /* parm_declarator_nostarttypename: '(' maybe_attribute parm_declarator_nostarttypename ')'  */
#line 1596 "c-parse.y"
                { (yyval.ttype) = (yyvsp[-2].ttype) ? tree_cons ((yyvsp[-2].ttype), (yyvsp[-1].ttype), NULL_TREE) : (yyvsp[-1].ttype); }
#line 4506 "c-p12907.c"
    break;

  case 345: /* notype_declarator: notype_declarator '(' parmlist_or_identifiers  */
#line 1604 "c-parse.y"
                { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL_TREE); }
#line 4512 "c-p12907.c"
    break;

  case 346: /* notype_declarator: '(' maybe_attribute notype_declarator ')'  */
#line 1609 "c-parse.y"
                { (yyval.ttype) = (yyvsp[-2].ttype) ? tree_cons ((yyvsp[-2].ttype), (yyvsp[-1].ttype), NULL_TREE) : (yyvsp[-1].ttype); }
#line 4518 "c-p12907.c"
    break;

  case 347: /* notype_declarator: '*' maybe_type_quals_attrs notype_declarator  */
#line 1611 "c-parse.y"
                { (yyval.ttype) = make_pointer_declarator ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4524 "c-p12907.c"
    break;

  case 348: /* notype_declarator: notype_declarator array_declarator  */
#line 1613 "c-parse.y"
                { (yyval.ttype) = set_array_declarator_type ((yyvsp[0].ttype), (yyvsp[-1].ttype), 0); }
#line 4530 "c-p12907.c"
    break;

  case 350: /* struct_head: STRUCT  */
#line 1619 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4536 "c-p12907.c"
    break;

  case 351: /* struct_head: STRUCT attributes  */
#line 1621 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4542 "c-p12907.c"
    break;

  case 352: /* union_head: UNION  */
#line 1626 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4548 "c-p12907.c"
    break;

  case 353: /* union_head: UNION attributes  */
#line 1628 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4554 "c-p12907.c"
    break;

  case 354: /* enum_head: ENUM  */
#line 1633 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4560 "c-p12907.c"
    break;

  case 355: /* enum_head: ENUM attributes  */
#line 1635 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4566 "c-p12907.c"
    break;

  case 356: /* @24: %empty  */
#line 1646 "c-parse.y"
                { (yyval.ttype) = start_struct (RECORD_TYPE, (yyvsp[-1].ttype));
		  /* Start scope of tag before parsing components.  */
		}
#line 4574 "c-p12907.c"
    break;

  case 357: /* structsp_attr: struct_head identifier '{' @24 component_decl_list '}' maybe_attribute  */
#line 1650 "c-parse.y"
                { (yyval.ttype) = finish_struct ((yyvsp[-3].ttype), (yyvsp[-2].ttype), chainon ((yyvsp[-6].ttype), (yyvsp[0].ttype))); }
#line 4580 "c-p12907.c"
    break;

  case 358: /* structsp_attr: struct_head '{' component_decl_list '}' maybe_attribute  */
#line 1652 "c-parse.y"
                { (yyval.ttype) = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
				      (yyvsp[-2].ttype), chainon ((yyvsp[-4].ttype), (yyvsp[0].ttype)));
		}
#line 4588 "c-p12907.c"
    break;

  case 359: /* @25: %empty  */
#line 1656 "c-parse.y"
                { (yyval.ttype) = start_struct (UNION_TYPE, (yyvsp[-1].ttype)); }
#line 4594 "c-p12907.c"
    break;

  case 360: /* structsp_attr: union_head identifier '{' @25 component_decl_list '}' maybe_attribute  */
#line 1658 "c-parse.y"
                { (yyval.ttype) = finish_struct ((yyvsp[-3].ttype), (yyvsp[-2].ttype), chainon ((yyvsp[-6].ttype), (yyvsp[0].ttype))); }
#line 4600 "c-p12907.c"
    break;

  case 361: /* structsp_attr: union_head '{' component_decl_list '}' maybe_attribute  */
#line 1660 "c-parse.y"
                { (yyval.ttype) = finish_struct (start_struct (UNION_TYPE, NULL_TREE),
				      (yyvsp[-2].ttype), chainon ((yyvsp[-4].ttype), (yyvsp[0].ttype)));
		}
#line 4608 "c-p12907.c"
    break;

  case 362: /* @26: %empty  */
#line 1664 "c-parse.y"
                { (yyval.ttype) = start_enum ((yyvsp[-1].ttype)); }
#line 4614 "c-p12907.c"
    break;

  case 363: /* structsp_attr: enum_head identifier '{' @26 enumlist maybecomma_warn '}' maybe_attribute  */
#line 1666 "c-parse.y"
                { (yyval.ttype) = finish_enum ((yyvsp[-4].ttype), nreverse ((yyvsp[-3].ttype)),
				    chainon ((yyvsp[-7].ttype), (yyvsp[0].ttype))); }
#line 4621 "c-p12907.c"
    break;

  case 364: /* @27: %empty  */
#line 1669 "c-parse.y"
                { (yyval.ttype) = start_enum (NULL_TREE); }
#line 4627 "c-p12907.c"
    break;

  case 365: /* structsp_attr: enum_head '{' @27 enumlist maybecomma_warn '}' maybe_attribute  */
#line 1671 "c-parse.y"
                { (yyval.ttype) = finish_enum ((yyvsp[-4].ttype), nreverse ((yyvsp[-3].ttype)),
				    chainon ((yyvsp[-6].ttype), (yyvsp[0].ttype))); }
#line 4634 "c-p12907.c"
    break;

  case 366: /* structsp_nonattr: struct_head identifier  */
#line 1677 "c-parse.y"
                { (yyval.ttype) = xref_tag (RECORD_TYPE, (yyvsp[0].ttype)); }
#line 4640 "c-p12907.c"
    break;

  case 367: /* structsp_nonattr: union_head identifier  */
#line 1679 "c-parse.y"
                { (yyval.ttype) = xref_tag (UNION_TYPE, (yyvsp[0].ttype)); }
#line 4646 "c-p12907.c"
    break;

  case 368: /* structsp_nonattr: enum_head identifier  */
#line 1681 "c-parse.y"
                { (yyval.ttype) = xref_tag (ENUMERAL_TYPE, (yyvsp[0].ttype));
		  /* In ISO C, enumerated types can be referred to
		     only if already defined.  */
		  if (pedantic && !COMPLETE_TYPE_P ((yyval.ttype)))
		    pedwarn ("ISO C forbids forward references to `enum' types"); }
#line 4656 "c-p12907.c"
    break;

  case 372: /* maybecomma_warn: ','  */
#line 1696 "c-parse.y"
                { if (pedantic && ! flag_isoc99)
		    pedwarn ("comma at end of enumerator list"); }
#line 4663 "c-p12907.c"
    break;

  case 373: /* component_decl_list: component_decl_list2  */
#line 1702 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 4669 "c-p12907.c"
    break;

  case 374: /* component_decl_list: component_decl_list2 component_decl  */
#line 1704 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-1].ttype), (yyvsp[0].ttype));
		  pedwarn ("no semicolon at end of struct or union"); }
#line 4676 "c-p12907.c"
    break;

  case 375: /* component_decl_list2: %empty  */
#line 1709 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4682 "c-p12907.c"
    break;

  case 376: /* component_decl_list2: component_decl_list2 component_decl ';'  */
#line 1711 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-2].ttype), (yyvsp[-1].ttype)); }
#line 4688 "c-p12907.c"
    break;

  case 377: /* component_decl_list2: component_decl_list2 ';'  */
#line 1713 "c-parse.y"
                { if (pedantic)
		    pedwarn ("extra semicolon in struct or union specified"); }
#line 4695 "c-p12907.c"
    break;

  case 378: /* component_decl: declspecs_nosc_ts setspecs components  */
#line 1719 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; }
#line 4702 "c-p12907.c"
    break;

  case 379: /* component_decl: declspecs_nosc_ts setspecs save_filename save_lineno  */
#line 1722 "c-parse.y"
                {
		  /* Support for unnamed structs or unions as members of 
		     structs or unions (which is [a] useful and [b] supports 
		     MS P-SDK).  */
		  if (pedantic)
		    pedwarn ("ISO C doesn't support unnamed structs/unions");

		  (yyval.ttype) = grokfield((yyvsp[-1].filename), (yyvsp[0].lineno), NULL, current_declspecs, NULL_TREE);
		  POP_DECLSPEC_STACK; }
#line 4716 "c-p12907.c"
    break;

  case 380: /* component_decl: declspecs_nosc_nots setspecs components_notype  */
#line 1732 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; }
#line 4723 "c-p12907.c"
    break;

  case 381: /* component_decl: declspecs_nosc_nots  */
#line 1735 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids member declarations with no members");
		  shadow_tag((yyvsp[0].ttype));
		  (yyval.ttype) = NULL_TREE; }
#line 4732 "c-p12907.c"
    break;

  case 382: /* component_decl: error  */
#line 1740 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4738 "c-p12907.c"
    break;

  case 383: /* component_decl: extension component_decl  */
#line 1742 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  RESTORE_WARN_FLAGS ((yyvsp[-1].ttype)); }
#line 4745 "c-p12907.c"
    break;

  case 385: /* components: components ',' maybe_resetattrs component_declarator  */
#line 1749 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-3].ttype), (yyvsp[0].ttype)); }
#line 4751 "c-p12907.c"
    break;

  case 387: /* components_notype: components_notype ',' maybe_resetattrs component_notype_declarator  */
#line 1755 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-3].ttype), (yyvsp[0].ttype)); }
#line 4757 "c-p12907.c"
    break;

  case 388: /* component_declarator: save_filename save_lineno declarator maybe_attribute  */
#line 1760 "c-parse.y"
                { (yyval.ttype) = grokfield ((yyvsp[-3].filename), (yyvsp[-2].lineno), (yyvsp[-1].ttype), current_declspecs, NULL_TREE);
		  decl_attributes (&(yyval.ttype), chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); }
#line 4764 "c-p12907.c"
    break;

  case 389: /* component_declarator: save_filename save_lineno declarator ':' expr_no_commas maybe_attribute  */
#line 1764 "c-parse.y"
                { (yyval.ttype) = grokfield ((yyvsp[-5].filename), (yyvsp[-4].lineno), (yyvsp[-3].ttype), current_declspecs, (yyvsp[-1].ttype));
		  decl_attributes (&(yyval.ttype), chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); }
#line 4771 "c-p12907.c"
    break;

  case 390: /* component_declarator: save_filename save_lineno ':' expr_no_commas maybe_attribute  */
#line 1767 "c-parse.y"
                { (yyval.ttype) = grokfield ((yyvsp[-4].filename), (yyvsp[-3].lineno), NULL_TREE, current_declspecs, (yyvsp[-1].ttype));
		  decl_attributes (&(yyval.ttype), chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); }
#line 4778 "c-p12907.c"
    break;

  case 391: /* component_notype_declarator: save_filename save_lineno notype_declarator maybe_attribute  */
#line 1773 "c-parse.y"
                { (yyval.ttype) = grokfield ((yyvsp[-3].filename), (yyvsp[-2].lineno), (yyvsp[-1].ttype), current_declspecs, NULL_TREE);
		  decl_attributes (&(yyval.ttype), chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); }
#line 4785 "c-p12907.c"
    break;

  case 392: /* component_notype_declarator: save_filename save_lineno notype_declarator ':' expr_no_commas maybe_attribute  */
#line 1777 "c-parse.y"
                { (yyval.ttype) = grokfield ((yyvsp[-5].filename), (yyvsp[-4].lineno), (yyvsp[-3].ttype), current_declspecs, (yyvsp[-1].ttype));
		  decl_attributes (&(yyval.ttype), chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); }
#line 4792 "c-p12907.c"
    break;

  case 393: /* component_notype_declarator: save_filename save_lineno ':' expr_no_commas maybe_attribute  */
#line 1780 "c-parse.y"
                { (yyval.ttype) = grokfield ((yyvsp[-4].filename), (yyvsp[-3].lineno), NULL_TREE, current_declspecs, (yyvsp[-1].ttype));
		  decl_attributes (&(yyval.ttype), chainon ((yyvsp[0].ttype), all_prefix_attributes), 0); }
#line 4799 "c-p12907.c"
    break;

  case 395: /* enumlist: enumlist ',' enumerator  */
#line 1792 "c-parse.y"
                { if ((yyvsp[-2].ttype) == error_mark_node)
		    (yyval.ttype) = (yyvsp[-2].ttype);
		  else
		    (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype)); }
#line 4808 "c-p12907.c"
    break;

  case 396: /* enumlist: error  */
#line 1797 "c-parse.y"
                { (yyval.ttype) = error_mark_node; }
#line 4814 "c-p12907.c"
    break;

  case 397: /* enumerator: identifier  */
#line 1803 "c-parse.y"
                { (yyval.ttype) = build_enumerator ((yyvsp[0].ttype), NULL_TREE); }
#line 4820 "c-p12907.c"
    break;

  case 398: /* enumerator: identifier '=' expr_no_commas  */
#line 1805 "c-parse.y"
                { (yyval.ttype) = build_enumerator ((yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 4826 "c-p12907.c"
    break;

  case 399: /* @28: %empty  */
#line 1810 "c-parse.y"
                { pending_xref_error ();
		  (yyval.ttype) = (yyvsp[0].ttype); }
#line 4833 "c-p12907.c"
    break;

  case 400: /* typename: declspecs_nosc @28 absdcl  */
#line 1813 "c-parse.y"
                { (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4839 "c-p12907.c"
    break;

  case 401: /* absdcl: %empty  */
#line 1818 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 4845 "c-p12907.c"
    break;

  case 403: /* absdcl_maybe_attribute: %empty  */
#line 1824 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 NULL_TREE),
					all_prefix_attributes); }
#line 4853 "c-p12907.c"
    break;

  case 404: /* absdcl_maybe_attribute: absdcl1  */
#line 1828 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[0].ttype)),
					all_prefix_attributes); }
#line 4861 "c-p12907.c"
    break;

  case 405: /* absdcl_maybe_attribute: absdcl1_noea attributes  */
#line 1832 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[-1].ttype)),
					chainon ((yyvsp[0].ttype), all_prefix_attributes)); }
#line 4869 "c-p12907.c"
    break;

  case 409: /* absdcl1_noea: '*' maybe_type_quals_attrs absdcl1_noea  */
#line 1845 "c-parse.y"
                { (yyval.ttype) = make_pointer_declarator ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4875 "c-p12907.c"
    break;

  case 410: /* absdcl1_ea: '*' maybe_type_quals_attrs  */
#line 1850 "c-parse.y"
                { (yyval.ttype) = make_pointer_declarator ((yyvsp[0].ttype), NULL_TREE); }
#line 4881 "c-p12907.c"
    break;

  case 411: /* absdcl1_ea: '*' maybe_type_quals_attrs absdcl1_ea  */
#line 1852 "c-parse.y"
                { (yyval.ttype) = make_pointer_declarator ((yyvsp[-1].ttype), (yyvsp[0].ttype)); }
#line 4887 "c-p12907.c"
    break;

  case 412: /* direct_absdcl1: '(' maybe_attribute absdcl1 ')'  */
#line 1857 "c-parse.y"
                { (yyval.ttype) = (yyvsp[-2].ttype) ? tree_cons ((yyvsp[-2].ttype), (yyvsp[-1].ttype), NULL_TREE) : (yyvsp[-1].ttype); }
#line 4893 "c-p12907.c"
    break;

  case 413: /* direct_absdcl1: direct_absdcl1 '(' parmlist  */
#line 1859 "c-parse.y"
                { (yyval.ttype) = build_nt (CALL_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL_TREE); }
#line 4899 "c-p12907.c"
    break;

  case 414: /* direct_absdcl1: direct_absdcl1 array_declarator  */
#line 1861 "c-parse.y"
                { (yyval.ttype) = set_array_declarator_type ((yyvsp[0].ttype), (yyvsp[-1].ttype), 1); }
#line 4905 "c-p12907.c"
    break;

  case 415: /* direct_absdcl1: '(' parmlist  */
#line 1863 "c-parse.y"
                { (yyval.ttype) = build_nt (CALL_EXPR, NULL_TREE, (yyvsp[0].ttype), NULL_TREE); }
#line 4911 "c-p12907.c"
    break;

  case 416: /* direct_absdcl1: array_declarator  */
#line 1865 "c-parse.y"
                { (yyval.ttype) = set_array_declarator_type ((yyvsp[0].ttype), NULL_TREE, 1); }
#line 4917 "c-p12907.c"
    break;

  case 417: /* array_declarator: '[' maybe_type_quals_attrs expr ']'  */
#line 1872 "c-parse.y"
                { (yyval.ttype) = build_array_declarator ((yyvsp[-1].ttype), (yyvsp[-2].ttype), 0, 0); }
#line 4923 "c-p12907.c"
    break;

  case 418: /* array_declarator: '[' maybe_type_quals_attrs ']'  */
#line 1874 "c-parse.y"
                { (yyval.ttype) = build_array_declarator (NULL_TREE, (yyvsp[-1].ttype), 0, 0); }
#line 4929 "c-p12907.c"
    break;

  case 419: /* array_declarator: '[' maybe_type_quals_attrs '*' ']'  */
#line 1876 "c-parse.y"
                { (yyval.ttype) = build_array_declarator (NULL_TREE, (yyvsp[-2].ttype), 0, 1); }
#line 4935 "c-p12907.c"
    break;

  case 420: /* array_declarator: '[' STATIC maybe_type_quals_attrs expr ']'  */
#line 1878 "c-parse.y"
                { (yyval.ttype) = build_array_declarator ((yyvsp[-1].ttype), (yyvsp[-2].ttype), 1, 0); }
#line 4941 "c-p12907.c"
    break;

  case 421: /* array_declarator: '[' declspecs_nosc_nots STATIC expr ']'  */
#line 1881 "c-parse.y"
                { (yyval.ttype) = build_array_declarator ((yyvsp[-1].ttype), (yyvsp[-3].ttype), 1, 0); }
#line 4947 "c-p12907.c"
    break;

  case 424: /* stmts_and_decls: lineno_stmt_decl_or_labels_ending_label  */
#line 1894 "c-parse.y"
                {
		  pedwarn ("deprecated use of label at end of compound statement");
		}
#line 4955 "c-p12907.c"
    break;

  case 432: /* lineno_stmt_decl_or_labels_ending_decl: lineno_stmt_decl_or_labels_ending_stmt lineno_decl  */
#line 1911 "c-parse.y"
                { if (pedantic && !flag_isoc99)
		    pedwarn ("ISO C89 forbids mixed declarations and code"); }
#line 4962 "c-p12907.c"
    break;

  case 447: /* pushlevel: %empty  */
#line 1941 "c-parse.y"
                { pushlevel (0);
		  clear_last_expr ();
		  add_scope_stmt (/*begin_p=*/1, /*partial_p=*/0);
		}
#line 4971 "c-p12907.c"
    break;

  case 448: /* poplevel: %empty  */
#line 1948 "c-parse.y"
                { (yyval.ttype) = add_scope_stmt (/*begin_p=*/0, /*partial_p=*/0); }
#line 4977 "c-p12907.c"
    break;

  case 449: /* c99_block_start: %empty  */
#line 1953 "c-parse.y"
                { if (flag_isoc99)
		    {
		      (yyval.ttype) = c_begin_compound_stmt ();
		      pushlevel (0);
		      clear_last_expr ();
		      add_scope_stmt (/*begin_p=*/1, /*partial_p=*/0);
		    }
		  else
		    (yyval.ttype) = NULL_TREE;
		}
#line 4992 "c-p12907.c"
    break;

  case 450: /* c99_block_end: %empty  */
#line 1969 "c-parse.y"
                { if (flag_isoc99)
		    {
		      tree scope_stmt = add_scope_stmt (/*begin_p=*/0, /*partial_p=*/0);
		      (yyval.ttype) = poplevel (kept_level_p (), 0, 0); 
		      SCOPE_STMT_BLOCK (TREE_PURPOSE (scope_stmt)) 
			= SCOPE_STMT_BLOCK (TREE_VALUE (scope_stmt))
			= (yyval.ttype);
		    }
		  else
		    (yyval.ttype) = NULL_TREE; }
#line 5007 "c-p12907.c"
    break;

  case 452: /* maybe_label_decls: label_decls  */
#line 1986 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids label declarations"); }
#line 5014 "c-p12907.c"
    break;

  case 455: /* label_decl: LABEL identifiers_or_typenames ';'  */
#line 1997 "c-parse.y"
                { tree link;
		  for (link = (yyvsp[-1].ttype); link; link = TREE_CHAIN (link))
		    {
		      tree label = shadow_label (TREE_VALUE (link));
		      C_DECLARED_LABEL_FLAG (label) = 1;
		      add_decl_stmt (label);
		    }
		}
#line 5027 "c-p12907.c"
    break;

  case 456: /* compstmt_or_error: compstmt  */
#line 2011 "c-parse.y"
                {}
#line 5033 "c-p12907.c"
    break;

  case 458: /* compstmt_start: '{'  */
#line 2015 "c-parse.y"
                    { compstmt_count++;
                      (yyval.ttype) = c_begin_compound_stmt (); }
#line 5040 "c-p12907.c"
    break;

  case 459: /* compstmt_nostart: '}'  */
#line 2020 "c-parse.y"
                { (yyval.ttype) = convert (void_type_node, integer_zero_node); }
#line 5046 "c-p12907.c"
    break;

  case 460: /* compstmt_nostart: pushlevel maybe_label_decls compstmt_contents_nonempty '}' poplevel  */
#line 2022 "c-parse.y"
                { (yyval.ttype) = poplevel (kept_level_p (), 1, 0); 
		  SCOPE_STMT_BLOCK (TREE_PURPOSE ((yyvsp[0].ttype))) 
		    = SCOPE_STMT_BLOCK (TREE_VALUE ((yyvsp[0].ttype)))
		    = (yyval.ttype); }
#line 5055 "c-p12907.c"
    break;

  case 463: /* compstmt_primary_start: '(' '{'  */
#line 2035 "c-parse.y"
                { if (current_function_decl == 0)
		    {
		      error ("braced-group within expression allowed only inside a function");
		      YYERROR;
		    }
		  /* We must force a BLOCK for this level
		     so that, if it is not expanded later,
		     there is a way to turn off the entire subtree of blocks
		     that are contained in it.  */
		  keep_next_level ();
		  push_label_level ();
		  compstmt_count++;
		  (yyval.ttype) = add_stmt (build_stmt (COMPOUND_STMT, last_tree));
		}
#line 5074 "c-p12907.c"
    break;

  case 464: /* compstmt: compstmt_start compstmt_nostart  */
#line 2052 "c-parse.y"
                { RECHAIN_STMTS ((yyvsp[-1].ttype), COMPOUND_BODY ((yyvsp[-1].ttype))); 
		  last_expr_type = NULL_TREE;
                  (yyval.ttype) = (yyvsp[-1].ttype); }
#line 5082 "c-p12907.c"
    break;

  case 465: /* simple_if: if_prefix c99_block_lineno_labeled_stmt  */
#line 2060 "c-parse.y"
                { c_finish_then (); }
#line 5088 "c-p12907.c"
    break;

  case 467: /* @29: %empty  */
#line 2077 "c-parse.y"
                { (yyval.ttype) = c_begin_if_stmt (); }
#line 5094 "c-p12907.c"
    break;

  case 468: /* if_prefix: IF @29 '(' expr ')'  */
#line 2079 "c-parse.y"
                { c_expand_start_cond (c_common_truthvalue_conversion ((yyvsp[-1].ttype)), 
				       compstmt_count,(yyvsp[-3].ttype));
		  (yyval.itype) = stmt_count;
		  if_stmt_file = (yyvsp[(-2) - (5)].filename);
		  if_stmt_line = (yyvsp[(-1) - (5)].lineno); }
#line 5104 "c-p12907.c"
    break;

  case 469: /* @30: %empty  */
#line 2091 "c-parse.y"
                { stmt_count++;
		  compstmt_count++;
		  (yyval.ttype) 
		    = add_stmt (build_stmt (DO_STMT, NULL_TREE,
					    NULL_TREE));
		  /* In the event that a parse error prevents
		     parsing the complete do-statement, set the
		     condition now.  Otherwise, we can get crashes at
		     RTL-generation time.  */
		  DO_COND ((yyval.ttype)) = error_mark_node; }
#line 5119 "c-p12907.c"
    break;

  case 470: /* do_stmt_start: DO @30 c99_block_lineno_labeled_stmt WHILE  */
#line 2102 "c-parse.y"
                { (yyval.ttype) = (yyvsp[-2].ttype);
		  RECHAIN_STMTS ((yyval.ttype), DO_BODY ((yyval.ttype))); }
#line 5126 "c-p12907.c"
    break;

  case 471: /* save_filename: %empty  */
#line 2110 "c-parse.y"
                { if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  (yyval.filename) = input_filename; }
#line 5134 "c-p12907.c"
    break;

  case 472: /* save_lineno: %empty  */
#line 2116 "c-parse.y"
                { if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  (yyval.lineno) = lineno; }
#line 5142 "c-p12907.c"
    break;

  case 475: /* c99_block_lineno_labeled_stmt: c99_block_start lineno_labeled_stmt c99_block_end  */
#line 2129 "c-parse.y"
                { if (flag_isoc99)
		    RECHAIN_STMTS ((yyvsp[-2].ttype), COMPOUND_BODY ((yyvsp[-2].ttype))); }
#line 5149 "c-p12907.c"
    break;

  case 476: /* lineno_stmt: save_filename save_lineno stmt  */
#line 2135 "c-parse.y"
                { if ((yyvsp[0].ttype))
		    {
		      STMT_LINENO ((yyvsp[0].ttype)) = (yyvsp[-1].lineno);
		      /* ??? We currently have no way of recording
			 the filename for a statement.  This probably
			 matters little in practice at the moment,
			 but I suspect that problems will occur when
			 doing inlining at the tree level.  */
		    }
		}
#line 5164 "c-p12907.c"
    break;

  case 477: /* lineno_label: save_filename save_lineno label  */
#line 2149 "c-parse.y"
                { if ((yyvsp[0].ttype))
		    {
		      STMT_LINENO ((yyvsp[0].ttype)) = (yyvsp[-1].lineno);
		    }
		}
#line 5174 "c-p12907.c"
    break;

  case 478: /* $@31: %empty  */
#line 2158 "c-parse.y"
                { c_expand_start_else ();
		  (yyvsp[-1].itype) = stmt_count; }
#line 5181 "c-p12907.c"
    break;

  case 479: /* select_or_iter_stmt: simple_if ELSE $@31 c99_block_lineno_labeled_stmt  */
#line 2161 "c-parse.y"
                { c_finish_else ();
		  c_expand_end_cond ();
		  if (extra_warnings && stmt_count == (yyvsp[-3].itype))
		    warning ("empty body in an else-statement"); }
#line 5190 "c-p12907.c"
    break;

  case 480: /* select_or_iter_stmt: simple_if  */
#line 2166 "c-parse.y"
                { c_expand_end_cond ();
		  /* This warning is here instead of in simple_if, because we
		     do not want a warning if an empty if is followed by an
		     else statement.  Increment stmt_count so we don't
		     give a second error if this is a nested `if'.  */
		  if (extra_warnings && stmt_count++ == (yyvsp[0].itype))
		    warning_with_file_and_line (if_stmt_file, if_stmt_line,
						"empty body in an if-statement"); }
#line 5203 "c-p12907.c"
    break;

  case 481: /* select_or_iter_stmt: simple_if ELSE error  */
#line 2178 "c-parse.y"
                { c_expand_end_cond (); }
#line 5209 "c-p12907.c"
    break;

  case 482: /* @32: %empty  */
#line 2188 "c-parse.y"
                { stmt_count++; 
		  (yyval.ttype) = c_begin_while_stmt (); }
#line 5216 "c-p12907.c"
    break;

  case 483: /* @33: %empty  */
#line 2191 "c-parse.y"
                { (yyvsp[-1].ttype) = c_common_truthvalue_conversion ((yyvsp[-1].ttype));
		  c_finish_while_stmt_cond
		    (c_common_truthvalue_conversion ((yyvsp[-1].ttype)), (yyvsp[-3].ttype));
		  (yyval.ttype) = add_stmt ((yyvsp[-3].ttype)); }
#line 5225 "c-p12907.c"
    break;

  case 484: /* select_or_iter_stmt: WHILE @32 '(' expr ')' @33 c99_block_lineno_labeled_stmt  */
#line 2196 "c-parse.y"
                { RECHAIN_STMTS ((yyvsp[-1].ttype), WHILE_BODY ((yyvsp[-1].ttype))); }
#line 5231 "c-p12907.c"
    break;

  case 485: /* select_or_iter_stmt: do_stmt_start '(' expr ')' ';'  */
#line 2199 "c-parse.y"
                { DO_COND ((yyvsp[-4].ttype)) = c_common_truthvalue_conversion ((yyvsp[-2].ttype)); }
#line 5237 "c-p12907.c"
    break;

  case 486: /* select_or_iter_stmt: do_stmt_start error  */
#line 2201 "c-parse.y"
                { }
#line 5243 "c-p12907.c"
    break;

  case 487: /* @34: %empty  */
#line 2203 "c-parse.y"
                { (yyval.ttype) = build_stmt (FOR_STMT, NULL_TREE, NULL_TREE,
					  NULL_TREE, NULL_TREE);
		  add_stmt ((yyval.ttype)); }
#line 5251 "c-p12907.c"
    break;

  case 488: /* $@35: %empty  */
#line 2207 "c-parse.y"
                { stmt_count++;
		  RECHAIN_STMTS ((yyvsp[-2].ttype), FOR_INIT_STMT ((yyvsp[-2].ttype))); }
#line 5258 "c-p12907.c"
    break;

  case 489: /* $@36: %empty  */
#line 2210 "c-parse.y"
                { if ((yyvsp[-1].ttype)) 
		    FOR_COND ((yyvsp[-5].ttype))
		      = c_common_truthvalue_conversion ((yyvsp[-1].ttype)); }
#line 5266 "c-p12907.c"
    break;

  case 490: /* $@37: %empty  */
#line 2214 "c-parse.y"
                { FOR_EXPR ((yyvsp[-8].ttype)) = (yyvsp[-1].ttype); }
#line 5272 "c-p12907.c"
    break;

  case 491: /* select_or_iter_stmt: FOR @34 '(' for_init_stmt $@35 xexpr ';' $@36 xexpr ')' $@37 c99_block_lineno_labeled_stmt  */
#line 2216 "c-parse.y"
                { RECHAIN_STMTS ((yyvsp[-10].ttype), FOR_BODY ((yyvsp[-10].ttype))); }
#line 5278 "c-p12907.c"
    break;

  case 492: /* @38: %empty  */
#line 2218 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = c_start_case ((yyvsp[-1].ttype)); }
#line 5285 "c-p12907.c"
    break;

  case 493: /* select_or_iter_stmt: SWITCH '(' expr ')' @38 c99_block_lineno_labeled_stmt  */
#line 2221 "c-parse.y"
                { c_finish_case (); }
#line 5291 "c-p12907.c"
    break;

  case 494: /* for_init_stmt: xexpr ';'  */
#line 2226 "c-parse.y"
                { add_stmt (build_stmt (EXPR_STMT, (yyvsp[-1].ttype))); }
#line 5297 "c-p12907.c"
    break;

  case 495: /* for_init_stmt: decl  */
#line 2228 "c-parse.y"
                { check_for_loop_decls (); }
#line 5303 "c-p12907.c"
    break;

  case 496: /* stmt: compstmt  */
#line 2234 "c-parse.y"
                { stmt_count++; (yyval.ttype) = (yyvsp[0].ttype); }
#line 5309 "c-p12907.c"
    break;

  case 497: /* stmt: expr ';'  */
#line 2236 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = c_expand_expr_stmt ((yyvsp[-1].ttype)); }
#line 5316 "c-p12907.c"
    break;

  case 498: /* stmt: c99_block_start select_or_iter_stmt c99_block_end  */
#line 2239 "c-parse.y"
                { if (flag_isoc99)
		    RECHAIN_STMTS ((yyvsp[-2].ttype), COMPOUND_BODY ((yyvsp[-2].ttype)));
		  (yyval.ttype) = NULL_TREE; }
#line 5324 "c-p12907.c"
    break;

  case 499: /* stmt: BREAK ';'  */
#line 2243 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = add_stmt (build_break_stmt ()); }
#line 5331 "c-p12907.c"
    break;

  case 500: /* stmt: CONTINUE ';'  */
#line 2246 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = add_stmt (build_continue_stmt ()); }
#line 5338 "c-p12907.c"
    break;

  case 501: /* stmt: RETURN ';'  */
#line 2249 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = c_expand_return (NULL_TREE); }
#line 5345 "c-p12907.c"
    break;

  case 502: /* stmt: RETURN expr ';'  */
#line 2252 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = c_expand_return ((yyvsp[-1].ttype)); }
#line 5352 "c-p12907.c"
    break;

  case 503: /* stmt: ASM_KEYWORD maybe_type_qual '(' expr ')' ';'  */
#line 2255 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = simple_asm_stmt ((yyvsp[-2].ttype)); }
#line 5359 "c-p12907.c"
    break;

  case 504: /* stmt: ASM_KEYWORD maybe_type_qual '(' expr ':' asm_operands ')' ';'  */
#line 2259 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = build_asm_stmt ((yyvsp[-6].ttype), (yyvsp[-4].ttype), (yyvsp[-2].ttype), NULL_TREE, NULL_TREE); }
#line 5366 "c-p12907.c"
    break;

  case 505: /* stmt: ASM_KEYWORD maybe_type_qual '(' expr ':' asm_operands ':' asm_operands ')' ';'  */
#line 2264 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = build_asm_stmt ((yyvsp[-8].ttype), (yyvsp[-6].ttype), (yyvsp[-4].ttype), (yyvsp[-2].ttype), NULL_TREE); }
#line 5373 "c-p12907.c"
    break;

  case 506: /* stmt: ASM_KEYWORD maybe_type_qual '(' expr ':' asm_operands ':' asm_operands ':' asm_clobbers ')' ';'  */
#line 2269 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = build_asm_stmt ((yyvsp[-10].ttype), (yyvsp[-8].ttype), (yyvsp[-6].ttype), (yyvsp[-4].ttype), (yyvsp[-2].ttype)); }
#line 5380 "c-p12907.c"
    break;

  case 507: /* stmt: GOTO identifier ';'  */
#line 2272 "c-parse.y"
                { tree decl;
		  stmt_count++;
		  decl = lookup_label ((yyvsp[-1].ttype));
		  if (decl != 0)
		    {
		      TREE_USED (decl) = 1;
		      (yyval.ttype) = add_stmt (build_stmt (GOTO_STMT, decl));
		    }
		  else
		    (yyval.ttype) = NULL_TREE;
		}
#line 5396 "c-p12907.c"
    break;

  case 508: /* stmt: GOTO '*' expr ';'  */
#line 2284 "c-parse.y"
                { if (pedantic)
		    pedwarn ("ISO C forbids `goto *expr;'");
		  stmt_count++;
		  (yyvsp[-1].ttype) = convert (ptr_type_node, (yyvsp[-1].ttype));
		  (yyval.ttype) = add_stmt (build_stmt (GOTO_STMT, (yyvsp[-1].ttype))); }
#line 5406 "c-p12907.c"
    break;

  case 509: /* stmt: ';'  */
#line 2290 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 5412 "c-p12907.c"
    break;

  case 510: /* label: CASE expr_no_commas ':'  */
#line 2298 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = do_case ((yyvsp[-1].ttype), NULL_TREE); }
#line 5419 "c-p12907.c"
    break;

  case 511: /* label: CASE expr_no_commas ELLIPSIS expr_no_commas ':'  */
#line 2301 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = do_case ((yyvsp[-3].ttype), (yyvsp[-1].ttype)); }
#line 5426 "c-p12907.c"
    break;

  case 512: /* label: DEFAULT ':'  */
#line 2304 "c-parse.y"
                { stmt_count++;
		  (yyval.ttype) = do_case (NULL_TREE, NULL_TREE); }
#line 5433 "c-p12907.c"
    break;

  case 513: /* label: identifier save_filename save_lineno ':' maybe_attribute  */
#line 2307 "c-parse.y"
                { tree label = define_label ((yyvsp[-3].filename), (yyvsp[-2].lineno), (yyvsp[-4].ttype));
		  stmt_count++;
		  if (label)
		    {
		      decl_attributes (&label, (yyvsp[0].ttype), 0);
		      (yyval.ttype) = add_stmt (build_stmt (LABEL_STMT, label));
		    }
		  else
		    (yyval.ttype) = NULL_TREE;
		}
#line 5448 "c-p12907.c"
    break;

  case 514: /* maybe_type_qual: %empty  */
#line 2323 "c-parse.y"
                { emit_line_note (input_filename, lineno);
		  (yyval.ttype) = NULL_TREE; }
#line 5455 "c-p12907.c"
    break;

  case 515: /* maybe_type_qual: TYPE_QUAL  */
#line 2326 "c-parse.y"
                { emit_line_note (input_filename, lineno); }
#line 5461 "c-p12907.c"
    break;

  case 516: /* xexpr: %empty  */
#line 2331 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 5467 "c-p12907.c"
    break;

  case 518: /* asm_operands: %empty  */
#line 2338 "c-parse.y"
                { (yyval.ttype) = NULL_TREE; }
#line 5473 "c-p12907.c"
    break;

  case 521: /* nonnull_asm_operands: nonnull_asm_operands ',' asm_operand  */
#line 2345 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-2].ttype), (yyvsp[0].ttype)); }
#line 5479 "c-p12907.c"
    break;

  case 522: /* asm_operand: STRING '(' expr ')'  */
#line 2350 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (NULL_TREE, (yyvsp[-3].ttype)), (yyvsp[-1].ttype)); }
#line 5485 "c-p12907.c"
    break;

  case 523: /* asm_operand: '[' identifier ']' STRING '(' expr ')'  */
#line 2352 "c-parse.y"
                { (yyvsp[-5].ttype) = build_string (IDENTIFIER_LENGTH ((yyvsp[-5].ttype)),
				     IDENTIFIER_POINTER ((yyvsp[-5].ttype)));
		  (yyval.ttype) = build_tree_list (build_tree_list ((yyvsp[-5].ttype), (yyvsp[-3].ttype)), (yyvsp[-1].ttype)); }
#line 5493 "c-p12907.c"
    break;

  case 524: /* asm_clobbers: string  */
#line 2359 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, combine_strings ((yyvsp[0].ttype)), NULL_TREE); }
#line 5499 "c-p12907.c"
    break;

  case 525: /* asm_clobbers: asm_clobbers ',' string  */
#line 2361 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, combine_strings ((yyvsp[0].ttype)), (yyvsp[-2].ttype)); }
#line 5505 "c-p12907.c"
    break;

  case 526: /* $@39: %empty  */
#line 2371 "c-parse.y"
                { pushlevel (0);
		  clear_parm_order ();
		  declare_parm_level (0); }
#line 5513 "c-p12907.c"
    break;

  case 527: /* parmlist: maybe_attribute $@39 parmlist_1  */
#line 2375 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  parmlist_tags_warning ();
		  poplevel (0, 0, 0); }
#line 5521 "c-p12907.c"
    break;

  case 529: /* $@40: %empty  */
#line 2383 "c-parse.y"
                { tree parm;
		  if (pedantic)
		    pedwarn ("ISO C forbids forward parameter declarations");
		  /* Mark the forward decls as such.  */
		  for (parm = getdecls (); parm; parm = TREE_CHAIN (parm))
		    TREE_ASM_WRITTEN (parm) = 1;
		  clear_parm_order (); }
#line 5533 "c-p12907.c"
    break;

  case 530: /* $@41: %empty  */
#line 2391 "c-parse.y"
                { /* Dummy action so attributes are in known place
		     on parser stack.  */ }
#line 5540 "c-p12907.c"
    break;

  case 531: /* parmlist_1: parms ';' $@40 maybe_attribute $@41 parmlist_1  */
#line 2394 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype); }
#line 5546 "c-p12907.c"
    break;

  case 532: /* parmlist_1: error ')'  */
#line 2396 "c-parse.y"
                { (yyval.ttype) = tree_cons (NULL_TREE, NULL_TREE, NULL_TREE); }
#line 5552 "c-p12907.c"
    break;

  case 533: /* parmlist_2: %empty  */
#line 2402 "c-parse.y"
                { (yyval.ttype) = get_parm_info (0); }
#line 5558 "c-p12907.c"
    break;

  case 534: /* parmlist_2: ELLIPSIS  */
#line 2404 "c-parse.y"
                { (yyval.ttype) = get_parm_info (0);
		  /* Gcc used to allow this as an extension.  However, it does
		     not work for all targets, and thus has been disabled.
		     Also, since func (...) and func () are indistinguishable,
		     it caused problems with the code in expand_builtin which
		     tries to verify that BUILT_IN_NEXT_ARG is being used
		     correctly.  */
		  error ("ISO C requires a named argument before `...'");
		}
#line 5572 "c-p12907.c"
    break;

  case 535: /* parmlist_2: parms  */
#line 2414 "c-parse.y"
                { (yyval.ttype) = get_parm_info (1); }
#line 5578 "c-p12907.c"
    break;

  case 536: /* parmlist_2: parms ',' ELLIPSIS  */
#line 2416 "c-parse.y"
                { (yyval.ttype) = get_parm_info (0); }
#line 5584 "c-p12907.c"
    break;

  case 537: /* parms: firstparm  */
#line 2421 "c-parse.y"
                { push_parm_decl ((yyvsp[0].ttype)); }
#line 5590 "c-p12907.c"
    break;

  case 538: /* parms: parms ',' parm  */
#line 2423 "c-parse.y"
                { push_parm_decl ((yyvsp[0].ttype)); }
#line 5596 "c-p12907.c"
    break;

  case 539: /* parm: declspecs_ts setspecs parm_declarator maybe_attribute  */
#line 2430 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[-1].ttype)),
					chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  POP_DECLSPEC_STACK; }
#line 5605 "c-p12907.c"
    break;

  case 540: /* parm: declspecs_ts setspecs notype_declarator maybe_attribute  */
#line 2435 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[-1].ttype)),
					chainon ((yyvsp[0].ttype), all_prefix_attributes)); 
		  POP_DECLSPEC_STACK; }
#line 5614 "c-p12907.c"
    break;

  case 541: /* parm: declspecs_ts setspecs absdcl_maybe_attribute  */
#line 2440 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; }
#line 5621 "c-p12907.c"
    break;

  case 542: /* parm: declspecs_nots setspecs notype_declarator maybe_attribute  */
#line 2443 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[-1].ttype)),
					chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  POP_DECLSPEC_STACK; }
#line 5630 "c-p12907.c"
    break;

  case 543: /* parm: declspecs_nots setspecs absdcl_maybe_attribute  */
#line 2449 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; }
#line 5637 "c-p12907.c"
    break;

  case 544: /* firstparm: declspecs_ts_nosa setspecs_fp parm_declarator maybe_attribute  */
#line 2457 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[-1].ttype)),
					chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  POP_DECLSPEC_STACK; }
#line 5646 "c-p12907.c"
    break;

  case 545: /* firstparm: declspecs_ts_nosa setspecs_fp notype_declarator maybe_attribute  */
#line 2462 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[-1].ttype)),
					chainon ((yyvsp[0].ttype), all_prefix_attributes)); 
		  POP_DECLSPEC_STACK; }
#line 5655 "c-p12907.c"
    break;

  case 546: /* firstparm: declspecs_ts_nosa setspecs_fp absdcl_maybe_attribute  */
#line 2467 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; }
#line 5662 "c-p12907.c"
    break;

  case 547: /* firstparm: declspecs_nots_nosa setspecs_fp notype_declarator maybe_attribute  */
#line 2470 "c-parse.y"
                { (yyval.ttype) = build_tree_list (build_tree_list (current_declspecs,
							 (yyvsp[-1].ttype)),
					chainon ((yyvsp[0].ttype), all_prefix_attributes));
		  POP_DECLSPEC_STACK; }
#line 5671 "c-p12907.c"
    break;

  case 548: /* firstparm: declspecs_nots_nosa setspecs_fp absdcl_maybe_attribute  */
#line 2476 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  POP_DECLSPEC_STACK; }
#line 5678 "c-p12907.c"
    break;

  case 549: /* setspecs_fp: setspecs  */
#line 2482 "c-parse.y"
                { prefix_attributes = chainon (prefix_attributes, (yyvsp[(-2) - (1)].ttype));
		  all_prefix_attributes = prefix_attributes; }
#line 5685 "c-p12907.c"
    break;

  case 550: /* $@42: %empty  */
#line 2491 "c-parse.y"
                { pushlevel (0);
		  clear_parm_order ();
		  declare_parm_level (1); }
#line 5693 "c-p12907.c"
    break;

  case 551: /* parmlist_or_identifiers: maybe_attribute $@42 parmlist_or_identifiers_1  */
#line 2495 "c-parse.y"
                { (yyval.ttype) = (yyvsp[0].ttype);
		  parmlist_tags_warning ();
		  poplevel (0, 0, 0); }
#line 5701 "c-p12907.c"
    break;

  case 553: /* parmlist_or_identifiers_1: identifiers ')'  */
#line 2503 "c-parse.y"
                { tree t;
		  for (t = (yyvsp[-1].ttype); t; t = TREE_CHAIN (t))
		    if (TREE_VALUE (t) == NULL_TREE)
		      error ("`...' in old-style identifier list");
		  (yyval.ttype) = tree_cons (NULL_TREE, NULL_TREE, (yyvsp[-1].ttype));

		  /* Make sure we have a parmlist after attributes.  */
		  if ((yyvsp[(-1) - (2)].ttype) != 0
		      && (TREE_CODE ((yyval.ttype)) != TREE_LIST
			  || TREE_PURPOSE ((yyval.ttype)) == 0
			  || TREE_CODE (TREE_PURPOSE ((yyval.ttype))) != PARM_DECL))
		    YYERROR1;
		}
#line 5719 "c-p12907.c"
    break;

  case 554: /* identifiers: IDENTIFIER  */
#line 2521 "c-parse.y"
                { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].ttype)); }
#line 5725 "c-p12907.c"
    break;

  case 555: /* identifiers: identifiers ',' IDENTIFIER  */
#line 2523 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE, (yyvsp[0].ttype))); }
#line 5731 "c-p12907.c"
    break;

  case 556: /* identifiers_or_typenames: identifier  */
#line 2529 "c-parse.y"
                { (yyval.ttype) = build_tree_list (NULL_TREE, (yyvsp[0].ttype)); }
#line 5737 "c-p12907.c"
    break;

  case 557: /* identifiers_or_typenames: identifiers_or_typenames ',' identifier  */
#line 2531 "c-parse.y"
                { (yyval.ttype) = chainon ((yyvsp[-2].ttype), build_tree_list (NULL_TREE, (yyvsp[0].ttype))); }
#line 5743 "c-p12907.c"
    break;

  case 558: /* extension: EXTENSION  */
#line 2536 "c-parse.y"
                { (yyval.ttype) = SAVE_WARN_FLAGS();
		  pedantic = 0;
		  warn_pointer_arith = 0;
		  warn_traditional = 0; }
#line 5752 "c-p12907.c"
    break;


#line 5756 "c-p12907.c"

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 2542 "c-parse.y"


/* yylex() is a thin wrapper around c_lex(), all it does is translate
   cpplib.h's token codes into yacc's token codes.  */

static enum cpp_ttype last_token;

/* The reserved keyword table.  */
struct resword
{
  const char *word;
  ENUM_BITFIELD(rid) rid : 16;
  unsigned int disable   : 16;
};

/* Disable mask.  Keywords are disabled if (reswords[i].disable & mask) is
   _true_.  */
#define D_C89	0x01	/* not in C89 */
#define D_EXT	0x02	/* GCC extension */
#define D_EXT89	0x04	/* GCC extension incorporated in C99 */
#define D_OBJC	0x08	/* Objective C only */

static const struct resword reswords[] =
{
  { "_Bool",		RID_BOOL,	0 },
  { "_Complex",		RID_COMPLEX,	0 },
  { "__FUNCTION__",	RID_FUNCTION_NAME, 0 },
  { "__PRETTY_FUNCTION__", RID_PRETTY_FUNCTION_NAME, 0 },
  { "__alignof",	RID_ALIGNOF,	0 },
  { "__alignof__",	RID_ALIGNOF,	0 },
  { "__asm",		RID_ASM,	0 },
  { "__asm__",		RID_ASM,	0 },
  { "__attribute",	RID_ATTRIBUTE,	0 },
  { "__attribute__",	RID_ATTRIBUTE,	0 },
  { "__bounded",	RID_BOUNDED,	0 },
  { "__bounded__",	RID_BOUNDED,	0 },
  { "__builtin_choose_expr", RID_CHOOSE_EXPR, 0 },
  { "__builtin_types_compatible_p", RID_TYPES_COMPATIBLE_P, 0 },
  { "__builtin_va_arg",	RID_VA_ARG,	0 },
  { "__complex",	RID_COMPLEX,	0 },
  { "__complex__",	RID_COMPLEX,	0 },
  { "__const",		RID_CONST,	0 },
  { "__const__",	RID_CONST,	0 },
  { "__extension__",	RID_EXTENSION,	0 },
  { "__func__",		RID_C99_FUNCTION_NAME, 0 },
  { "__imag",		RID_IMAGPART,	0 },
  { "__imag__",		RID_IMAGPART,	0 },
  { "__inline",		RID_INLINE,	0 },
  { "__inline__",	RID_INLINE,	0 },
  { "__label__",	RID_LABEL,	0 },
  { "__ptrbase",	RID_PTRBASE,	0 },
  { "__ptrbase__",	RID_PTRBASE,	0 },
  { "__ptrextent",	RID_PTREXTENT,	0 },
  { "__ptrextent__",	RID_PTREXTENT,	0 },
  { "__ptrvalue",	RID_PTRVALUE,	0 },
  { "__ptrvalue__",	RID_PTRVALUE,	0 },
  { "__real",		RID_REALPART,	0 },
  { "__real__",		RID_REALPART,	0 },
  { "__restrict",	RID_RESTRICT,	0 },
  { "__restrict__",	RID_RESTRICT,	0 },
  { "__signed",		RID_SIGNED,	0 },
  { "__signed__",	RID_SIGNED,	0 },
  { "__typeof",		RID_TYPEOF,	0 },
  { "__typeof__",	RID_TYPEOF,	0 },
  { "__unbounded",	RID_UNBOUNDED,	0 },
  { "__unbounded__",	RID_UNBOUNDED,	0 },
  { "__volatile",	RID_VOLATILE,	0 },
  { "__volatile__",	RID_VOLATILE,	0 },
  { "asm",		RID_ASM,	D_EXT },
  { "auto",		RID_AUTO,	0 },
  { "break",		RID_BREAK,	0 },
  { "case",		RID_CASE,	0 },
  { "char",		RID_CHAR,	0 },
  { "const",		RID_CONST,	0 },
  { "continue",		RID_CONTINUE,	0 },
  { "default",		RID_DEFAULT,	0 },
  { "do",		RID_DO,		0 },
  { "double",		RID_DOUBLE,	0 },
  { "else",		RID_ELSE,	0 },
  { "enum",		RID_ENUM,	0 },
  { "extern",		RID_EXTERN,	0 },
  { "float",		RID_FLOAT,	0 },
  { "for",		RID_FOR,	0 },
  { "goto",		RID_GOTO,	0 },
  { "if",		RID_IF,		0 },
  { "inline",		RID_INLINE,	D_EXT89 },
  { "int",		RID_INT,	0 },
  { "long",		RID_LONG,	0 },
  { "register",		RID_REGISTER,	0 },
  { "restrict",		RID_RESTRICT,	D_C89 },
  { "return",		RID_RETURN,	0 },
  { "short",		RID_SHORT,	0 },
  { "signed",		RID_SIGNED,	0 },
  { "sizeof",		RID_SIZEOF,	0 },
  { "static",		RID_STATIC,	0 },
  { "struct",		RID_STRUCT,	0 },
  { "switch",		RID_SWITCH,	0 },
  { "typedef",		RID_TYPEDEF,	0 },
  { "typeof",		RID_TYPEOF,	D_EXT },
  { "union",		RID_UNION,	0 },
  { "unsigned",		RID_UNSIGNED,	0 },
  { "void",		RID_VOID,	0 },
  { "volatile",		RID_VOLATILE,	0 },
  { "while",		RID_WHILE,	0 },
};
#define N_reswords (sizeof reswords / sizeof (struct resword))

/* Table mapping from RID_* constants to yacc token numbers.
   Unfortunately we have to have entries for all the keywords in all
   three languages.  */
static const short rid_to_yy[RID_MAX] =
{
  /* RID_STATIC */	STATIC,
  /* RID_UNSIGNED */	TYPESPEC,
  /* RID_LONG */	TYPESPEC,
  /* RID_CONST */	TYPE_QUAL,
  /* RID_EXTERN */	SCSPEC,
  /* RID_REGISTER */	SCSPEC,
  /* RID_TYPEDEF */	SCSPEC,
  /* RID_SHORT */	TYPESPEC,
  /* RID_INLINE */	SCSPEC,
  /* RID_VOLATILE */	TYPE_QUAL,
  /* RID_SIGNED */	TYPESPEC,
  /* RID_AUTO */	SCSPEC,
  /* RID_RESTRICT */	TYPE_QUAL,

  /* C extensions */
  /* RID_BOUNDED */	TYPE_QUAL,
  /* RID_UNBOUNDED */	TYPE_QUAL,
  /* RID_COMPLEX */	TYPESPEC,

  /* C++ */
  /* RID_FRIEND */	0,
  /* RID_VIRTUAL */	0,
  /* RID_EXPLICIT */	0,
  /* RID_EXPORT */	0,
  /* RID_MUTABLE */	0,

  /* ObjC */
  /* RID_IN */		TYPE_QUAL,
  /* RID_OUT */		TYPE_QUAL,
  /* RID_INOUT */	TYPE_QUAL,
  /* RID_BYCOPY */	TYPE_QUAL,
  /* RID_BYREF */	TYPE_QUAL,
  /* RID_ONEWAY */	TYPE_QUAL,
  
  /* C */
  /* RID_INT */		TYPESPEC,
  /* RID_CHAR */	TYPESPEC,
  /* RID_FLOAT */	TYPESPEC,
  /* RID_DOUBLE */	TYPESPEC,
  /* RID_VOID */	TYPESPEC,
  /* RID_ENUM */	ENUM,
  /* RID_STRUCT */	STRUCT,
  /* RID_UNION */	UNION,
  /* RID_IF */		IF,
  /* RID_ELSE */	ELSE,
  /* RID_WHILE */	WHILE,
  /* RID_DO */		DO,
  /* RID_FOR */		FOR,
  /* RID_SWITCH */	SWITCH,
  /* RID_CASE */	CASE,
  /* RID_DEFAULT */	DEFAULT,
  /* RID_BREAK */	BREAK,
  /* RID_CONTINUE */	CONTINUE,
  /* RID_RETURN */	RETURN,
  /* RID_GOTO */	GOTO,
  /* RID_SIZEOF */	SIZEOF,

  /* C extensions */
  /* RID_ASM */		ASM_KEYWORD,
  /* RID_TYPEOF */	TYPEOF,
  /* RID_ALIGNOF */	ALIGNOF,
  /* RID_ATTRIBUTE */	ATTRIBUTE,
  /* RID_VA_ARG */	VA_ARG,
  /* RID_EXTENSION */	EXTENSION,
  /* RID_IMAGPART */	IMAGPART,
  /* RID_REALPART */	REALPART,
  /* RID_LABEL */	LABEL,
  /* RID_PTRBASE */	PTR_BASE,
  /* RID_PTREXTENT */	PTR_EXTENT,
  /* RID_PTRVALUE */	PTR_VALUE,

  /* RID_CHOOSE_EXPR */			CHOOSE_EXPR,
  /* RID_TYPES_COMPATIBLE_P */		TYPES_COMPATIBLE_P,

  /* RID_FUNCTION_NAME */		STRING_FUNC_NAME,
  /* RID_PRETTY_FUNCTION_NAME */	STRING_FUNC_NAME,
  /* RID_C99_FUNCTION_NAME */		VAR_FUNC_NAME,

  /* C++ */
  /* RID_BOOL */	TYPESPEC,
  /* RID_WCHAR */	0,
  /* RID_CLASS */	0,
  /* RID_PUBLIC */	0,
  /* RID_PRIVATE */	0,
  /* RID_PROTECTED */	0,
  /* RID_TEMPLATE */	0,
  /* RID_NULL */	0,
  /* RID_CATCH */	0,
  /* RID_DELETE */	0,
  /* RID_FALSE */	0,
  /* RID_NAMESPACE */	0,
  /* RID_NEW */		0,
  /* RID_OPERATOR */	0,
  /* RID_THIS */	0,
  /* RID_THROW */	0,
  /* RID_TRUE */	0,
  /* RID_TRY */		0,
  /* RID_TYPENAME */	0,
  /* RID_TYPEID */	0,
  /* RID_USING */	0,

  /* casts */
  /* RID_CONSTCAST */	0,
  /* RID_DYNCAST */	0,
  /* RID_REINTCAST */	0,
  /* RID_STATCAST */	0,

  /* alternate spellings */
  /* RID_AND */		0,
  /* RID_AND_EQ */	0,
  /* RID_NOT */		0,
  /* RID_NOT_EQ */	0,
  /* RID_OR */		0,
  /* RID_OR_EQ */	0,
  /* RID_XOR */		0,
  /* RID_XOR_EQ */	0,
  /* RID_BITAND */	0,
  /* RID_BITOR */	0,
  /* RID_COMPL */	0,
  
  /* Objective C */
  /* RID_ID */			OBJECTNAME,
  /* RID_AT_ENCODE */		ENCODE,
  /* RID_AT_END */		END,
  /* RID_AT_CLASS */		CLASS,
  /* RID_AT_ALIAS */		ALIAS,
  /* RID_AT_DEFS */		DEFS,
  /* RID_AT_PRIVATE */		PRIVATE,
  /* RID_AT_PROTECTED */	PROTECTED,
  /* RID_AT_PUBLIC */		PUBLIC,
  /* RID_AT_PROTOCOL */		PROTOCOL,
  /* RID_AT_SELECTOR */		SELECTOR,
  /* RID_AT_INTERFACE */	INTERFACE,
  /* RID_AT_IMPLEMENTATION */	IMPLEMENTATION
};

static void
init_reswords ()
{
  unsigned int i;
  tree id;
  int mask = (flag_isoc99 ? 0 : D_C89)
	      | (flag_no_asm ? (flag_isoc99 ? D_EXT : D_EXT|D_EXT89) : 0);

  if (c_language != clk_objective_c)
     mask |= D_OBJC;

  /* It is not necessary to register ridpointers as a GC root, because
     all the trees it points to are permanently interned in the
     get_identifier hash anyway.  */
  ridpointers = (tree *) xcalloc ((int) RID_MAX, sizeof (tree));
  for (i = 0; i < N_reswords; i++)
    {
      /* If a keyword is disabled, do not enter it into the table
	 and so create a canonical spelling that isn't a keyword.  */
      if (reswords[i].disable & mask)
	continue;

      id = get_identifier (reswords[i].word);
      C_RID_CODE (id) = reswords[i].rid;
      C_IS_RESERVED_WORD (id) = 1;
      ridpointers [(int) reswords[i].rid] = id;
    }
}

#define NAME(type) cpp_type2name (type)

static void
yyerror (msgid)
     const char *msgid;
{
  const char *string = _(msgid);

  if (last_token == CPP_EOF)
    error ("%s at end of input", string);
  else if (last_token == CPP_CHAR || last_token == CPP_WCHAR)
    {
      unsigned int val = TREE_INT_CST_LOW (yylval.ttype);
      const char *const ell = (last_token == CPP_CHAR) ? "" : "L";
      if (val <= UCHAR_MAX && ISGRAPH (val))
	error ("%s before %s'%c'", string, ell, val);
      else
	error ("%s before %s'\\x%x'", string, ell, val);
    }
  else if (last_token == CPP_STRING
	   || last_token == CPP_WSTRING)
    error ("%s before string constant", string);
  else if (last_token == CPP_NUMBER)
    error ("%s before numeric constant", string);
  else if (last_token == CPP_NAME)
    error ("%s before \"%s\"", string, IDENTIFIER_POINTER (yylval.ttype));
  else
    error ("%s before '%s' token", string, NAME(last_token));
}

static int
yylexname ()
{
  tree decl;
  
  
  if (C_IS_RESERVED_WORD (yylval.ttype))
    {
      enum rid rid_code = C_RID_CODE (yylval.ttype);

      {
	int yycode = rid_to_yy[(int) rid_code];
	if (yycode == STRING_FUNC_NAME)
	  {
	    /* __FUNCTION__ and __PRETTY_FUNCTION__ get converted
	       to string constants.  */
	    const char *name = fname_string (rid_code);
	  
	    yylval.ttype = build_string (strlen (name) + 1, name);
	    C_ARTIFICIAL_STRING_P (yylval.ttype) = 1;
	    last_token = CPP_STRING;  /* so yyerror won't choke */
	    return STRING;
	  }
      
	/* Return the canonical spelling for this keyword.  */
	yylval.ttype = ridpointers[(int) rid_code];
	return yycode;
      }
    }

  decl = lookup_name (yylval.ttype);
  if (decl)
    {
      if (TREE_CODE (decl) == TYPE_DECL)
	return TYPENAME;
    }

  return IDENTIFIER;
}


static inline int
_yylex ()
{
 get_next:
  last_token = c_lex (&yylval.ttype);
  switch (last_token)
    {
    case CPP_EQ:					return '=';
    case CPP_NOT:					return '!';
    case CPP_GREATER:	yylval.code = GT_EXPR;		return ARITHCOMPARE;
    case CPP_LESS:	yylval.code = LT_EXPR;		return ARITHCOMPARE;
    case CPP_PLUS:	yylval.code = PLUS_EXPR;	return '+';
    case CPP_MINUS:	yylval.code = MINUS_EXPR;	return '-';
    case CPP_MULT:	yylval.code = MULT_EXPR;	return '*';
    case CPP_DIV:	yylval.code = TRUNC_DIV_EXPR;	return '/';
    case CPP_MOD:	yylval.code = TRUNC_MOD_EXPR;	return '%';
    case CPP_AND:	yylval.code = BIT_AND_EXPR;	return '&';
    case CPP_OR:	yylval.code = BIT_IOR_EXPR;	return '|';
    case CPP_XOR:	yylval.code = BIT_XOR_EXPR;	return '^';
    case CPP_RSHIFT:	yylval.code = RSHIFT_EXPR;	return RSHIFT;
    case CPP_LSHIFT:	yylval.code = LSHIFT_EXPR;	return LSHIFT;

    case CPP_COMPL:					return '~';
    case CPP_AND_AND:					return ANDAND;
    case CPP_OR_OR:					return OROR;
    case CPP_QUERY:					return '?';
    case CPP_OPEN_PAREN:				return '(';
    case CPP_EQ_EQ:	yylval.code = EQ_EXPR;		return EQCOMPARE;
    case CPP_NOT_EQ:	yylval.code = NE_EXPR;		return EQCOMPARE;
    case CPP_GREATER_EQ:yylval.code = GE_EXPR;		return ARITHCOMPARE;
    case CPP_LESS_EQ:	yylval.code = LE_EXPR;		return ARITHCOMPARE;

    case CPP_PLUS_EQ:	yylval.code = PLUS_EXPR;	return ASSIGN;
    case CPP_MINUS_EQ:	yylval.code = MINUS_EXPR;	return ASSIGN;
    case CPP_MULT_EQ:	yylval.code = MULT_EXPR;	return ASSIGN;
    case CPP_DIV_EQ:	yylval.code = TRUNC_DIV_EXPR;	return ASSIGN;
    case CPP_MOD_EQ:	yylval.code = TRUNC_MOD_EXPR;	return ASSIGN;
    case CPP_AND_EQ:	yylval.code = BIT_AND_EXPR;	return ASSIGN;
    case CPP_OR_EQ:	yylval.code = BIT_IOR_EXPR;	return ASSIGN;
    case CPP_XOR_EQ:	yylval.code = BIT_XOR_EXPR;	return ASSIGN;
    case CPP_RSHIFT_EQ:	yylval.code = RSHIFT_EXPR;	return ASSIGN;
    case CPP_LSHIFT_EQ:	yylval.code = LSHIFT_EXPR;	return ASSIGN;

    case CPP_OPEN_SQUARE:				return '[';
    case CPP_CLOSE_SQUARE:				return ']';
    case CPP_OPEN_BRACE:				return '{';
    case CPP_CLOSE_BRACE:				return '}';
    case CPP_ELLIPSIS:					return ELLIPSIS;

    case CPP_PLUS_PLUS:					return PLUSPLUS;
    case CPP_MINUS_MINUS:				return MINUSMINUS;
    case CPP_DEREF:					return POINTSAT;
    case CPP_DOT:					return '.';

      /* The following tokens may affect the interpretation of any
	 identifiers following, if doing Objective-C.  */
    case CPP_COLON:		OBJC_NEED_RAW_IDENTIFIER (0);	return ':';
    case CPP_COMMA:		OBJC_NEED_RAW_IDENTIFIER (0);	return ',';
    case CPP_CLOSE_PAREN:	OBJC_NEED_RAW_IDENTIFIER (0);	return ')';
    case CPP_SEMICOLON:		OBJC_NEED_RAW_IDENTIFIER (0);	return ';';

    case CPP_EOF:
      return 0;

    case CPP_NAME:
      return yylexname ();

    case CPP_NUMBER:
    case CPP_CHAR:
    case CPP_WCHAR:
      return CONSTANT;

    case CPP_STRING:
    case CPP_WSTRING:
      return STRING;
      
      /* This token is Objective-C specific.  It gives the next token
	 special significance.  */
    case CPP_ATSIGN:

      /* These tokens are C++ specific (and will not be generated
         in C mode, but let's be cautious).  */
    case CPP_SCOPE:
    case CPP_DEREF_STAR:
    case CPP_DOT_STAR:
    case CPP_MIN_EQ:
    case CPP_MAX_EQ:
    case CPP_MIN:
    case CPP_MAX:
      /* These tokens should not survive translation phase 4.  */
    case CPP_HASH:
    case CPP_PASTE:
      error ("syntax error at '%s' token", NAME(last_token));
      goto get_next;

    default:
      abort ();
    }
  /* NOTREACHED */
}

static int
yylex()
{
  int r;
  timevar_push (TV_LEX);
  r = _yylex();
  timevar_pop (TV_LEX);
  return r;
}

/* Sets the value of the 'yydebug' variable to VALUE.
   This is a function so we don't have to have YYDEBUG defined
   in order to build the compiler.  */

void
c_set_yydebug (value)
     int value;
{
#if YYDEBUG != 0
  yydebug = value;
#else
  warning ("YYDEBUG not defined");
#endif
}

/* Function used when yydebug is set, to print a token in more detail.  */

static void
yyprint (file, yychar, yyl)
     FILE *file;
     int yychar;
     YYSTYPE yyl;
{
  tree t = yyl.ttype;

  fprintf (file, " [%s]", NAME(last_token));
  
  switch (yychar)
    {
    case IDENTIFIER:
    case TYPENAME:
    case OBJECTNAME:
    case TYPESPEC:
    case TYPE_QUAL:
    case SCSPEC:
    case STATIC:
      if (IDENTIFIER_POINTER (t))
	fprintf (file, " `%s'", IDENTIFIER_POINTER (t));
      break;

    case CONSTANT:
      fprintf (file, " %s", GET_MODE_NAME (TYPE_MODE (TREE_TYPE (t))));
      if (TREE_CODE (t) == INTEGER_CST)
	fprintf (file,
#if HOST_BITS_PER_WIDE_INT == 64
#if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_INT
		 " 0x%x%016x",
#else
#if HOST_BITS_PER_WIDE_INT == HOST_BITS_PER_LONG
		 " 0x%lx%016lx",
#else
		 " 0x%llx%016llx",
#endif
#endif
#else
#if HOST_BITS_PER_WIDE_INT != HOST_BITS_PER_INT
		 " 0x%lx%08lx",
#else
		 " 0x%x%08x",
#endif
#endif
		 TREE_INT_CST_HIGH (t), TREE_INT_CST_LOW (t));
      break;
    }
}

/* This is not the ideal place to put these, but we have to get them out
   of c-lex.c because cp/lex.c has its own versions.  */

/* Return something to represent absolute declarators containing a *.
   TARGET is the absolute declarator that the * contains.
   TYPE_QUALS_ATTRS is a list of modifiers such as const or volatile
   to apply to the pointer type, represented as identifiers, possible mixed
   with attributes.

   We return an INDIRECT_REF whose "contents" are TARGET (inside a TREE_LIST,
   if attributes are present) and whose type is the modifier list.  */

tree
make_pointer_declarator (type_quals_attrs, target)
     tree type_quals_attrs, target;
{
  tree quals, attrs;
  tree itarget = target;
  split_specs_attrs (type_quals_attrs, &quals, &attrs);
  if (attrs != NULL_TREE)
    itarget = tree_cons (attrs, target, NULL_TREE);
  return build1 (INDIRECT_REF, quals, itarget);
}
