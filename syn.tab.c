
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "syn.y"

#include "ts.h"
#include "quad.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int ligne;
extern int colonne;

int  yylex(void);
void yyerror(const char *msg);

char SauvType[20];
char attente[128][15];
int  nb_attente = 0;

static int type_compatible(const char *gauche, const char *droite)
{
    if (strcmp(gauche, droite) == 0) return 1;
    if (strcmp(gauche,"float")==0 && strcmp(droite,"integer")==0) return 1;
    return 0;
}

static void erreur_semantique(const char *msg, const char *entite)
{
    fprintf(stderr,
        "Erreur semantique : %s, ligne %d, colonne %d, entite %s\n",
        msg, ligne, colonne, entite ? entite : "");
}

static void ajouterAttente(const char *nom)
{
    if (nb_attente < 128) {
        strncpy(attente[nb_attente], nom, 14);
        attente[nb_attente][14] = '\0';
        nb_attente++;
    }
}

static void viderAttente(void) { nb_attente = 0; }

static void appliquerTypeVar(void)
{
    int i;
    for (i = 0; i < nb_attente; i++) {
        if (rechercheType(attente[i]))
            erreur_semantique("double declaration", attente[i]);
        else {
            insererType(attente[i], SauvType);
            mettreAjourCode(attente[i], "var");
        }
    }
}

static void sauvegarder_valeur_expr(const char *nom, ExprInfo *e)
{
    char buf[64];
    if (e == NULL || !e->isConst) return;
    if (e->isInt) sprintf(buf, "%d", e->ival);
    else          sprintf(buf, "%g", (double)e->fval);
    insererValeur(nom, buf);
}

static void verifierDeclaration(const char *nom)
{
    if (!rechercheType(nom))
        erreur_semantique("identificateur non declare", nom);
}

static void verifierVariableSimple(const char *nom)
{
    verifierDeclaration(nom);
    if (estTableau(nom))
        erreur_semantique("tableau utilise sans indice", nom);
}

static void verifierConstanteModifiable(const char *nom)
{
    verifierDeclaration(nom);
    if (estConstante(nom))
        erreur_semantique("modification d'une constante", nom);
}

static void verifierTypeAffectation(const char *nom, ExprInfo *e)
{
    const char *t;
    if (e == NULL) return;
    t = getType(nom);
    if (t == NULL || strlen(t) == 0) return;
    if (!type_compatible(t, e->type))
        erreur_semantique("incompatibilite de types dans affectation", nom);
}

static void verifierTableauIndice(const char *nom, ExprInfo *idx)
{
    verifierDeclaration(nom);
    if (!estTableau(nom)) {
        erreur_semantique("identificateur non tableau utilise avec indice", nom);
        return;
    }
    if (idx == NULL) return;
    if (strcmp(idx->type, "integer") != 0)
        erreur_semantique("indice de tableau non entier", nom);
    if (idx->isConst && idx->isInt)
        if (idx->ival < 0 || idx->ival >= getTaille(nom))
            erreur_semantique("index hors limites", nom);
}


/* Line 189 of yacc.c  */
#line 184 "syn.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     mc_beginproject = 258,
     mc_endproject = 259,
     mc_setup = 260,
     mc_run = 261,
     mc_define = 262,
     mc_const = 263,
     mc_if = 264,
     mc_then = 265,
     mc_else = 266,
     mc_endif = 267,
     mc_loop = 268,
     mc_while = 269,
     mc_endloop = 270,
     mc_for = 271,
     mc_to = 272,
     mc_endfor = 273,
     mc_and = 274,
     mc_or = 275,
     mc_non = 276,
     mc_out = 277,
     mc_in = 278,
     err = 279,
     affect = 280,
     infeg = 281,
     supeg = 282,
     egal = 283,
     diff = 284,
     tok_pipe = 285,
     idf = 286,
     mc_integer = 287,
     mc_float = 288,
     tok_chaine = 289,
     cst_int = 290,
     cst_reel = 291
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 111 "syn.y"

    int   entier;
    float reel;
    char *str;
    void *expr;



/* Line 214 of yacc.c  */
#line 265 "syn.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 277 "syn.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   325

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNRULES -- Number of states.  */
#define YYNSTATES  168

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      50,    51,    41,    39,    52,    40,     2,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    44,    43,
      37,    47,    38,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    48,     2,    49,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    45,     2,    46,     2,     2,     2,     2,
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
      35,    36
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,    17,    18,    21,    23,    25,    27,    35,
      41,    49,    56,    59,    63,    73,    75,    77,    78,    81,
      83,    85,    87,    89,    91,    93,    98,   106,   113,   116,
     117,   125,   131,   132,   139,   146,   153,   160,   161,   168,
     174,   176,   180,   182,   184,   186,   190,   194,   198,   202,
     206,   210,   214,   218,   222,   226,   230,   234,   239,   243,
     245,   250,   252,   254,   256
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      54,     0,    -1,     3,    31,    43,     5,    44,    55,     6,
      44,    45,    62,    46,     4,    43,    -1,    -1,    55,    56,
      -1,    57,    -1,    58,    -1,    60,    -1,     8,    31,    44,
      61,    47,    81,    43,    -1,     7,    31,    44,    61,    43,
      -1,     7,    31,    44,    61,    47,    81,    43,    -1,     7,
      31,    59,    44,    61,    43,    -1,    30,    31,    -1,    59,
      30,    31,    -1,     7,    31,    44,    48,    61,    43,    35,
      49,    43,    -1,    32,    -1,    33,    -1,    -1,    62,    63,
      -1,    64,    -1,    68,    -1,    71,    -1,    73,    -1,    74,
      -1,    76,    -1,    31,    25,    80,    43,    -1,    31,    48,
      80,    49,    25,    80,    43,    -1,     9,    50,    79,    51,
      10,    44,    -1,    12,    43,    -1,    -1,    11,    67,    45,
      62,    46,    12,    43,    -1,    65,    45,    62,    46,    66,
      -1,    -1,    13,    14,    70,    50,    79,    51,    -1,    69,
      45,    62,    46,    15,    43,    -1,    16,    31,    23,    80,
      17,    80,    -1,    72,    45,    62,    46,    18,    43,    -1,
      -1,    22,    50,    75,    77,    51,    43,    -1,    23,    50,
      31,    51,    43,    -1,    78,    -1,    77,    52,    78,    -1,
      80,    -1,    34,    -1,    80,    -1,    80,    39,    80,    -1,
      80,    40,    80,    -1,    80,    41,    80,    -1,    80,    42,
      80,    -1,    80,    37,    80,    -1,    80,    38,    80,    -1,
      80,    26,    80,    -1,    80,    27,    80,    -1,    80,    28,
      80,    -1,    80,    29,    80,    -1,    80,    19,    80,    -1,
      80,    20,    80,    -1,    21,    50,    80,    51,    -1,    50,
      80,    51,    -1,    31,    -1,    31,    48,    80,    49,    -1,
      35,    -1,    36,    -1,    35,    -1,    36,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   141,   141,   150,   152,   156,   157,   158,   162,   180,
     190,   205,   215,   217,   222,   241,   242,   245,   247,   251,
     252,   253,   254,   255,   256,   260,   269,   282,   292,   295,
     294,   300,   304,   303,   315,   320,   338,   344,   343,   350,
     360,   361,   365,   367,   371,   374,   383,   392,   401,   412,
     421,   430,   439,   448,   457,   466,   475,   484,   491,   492,
     500,   510,   516,   525,   531
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "mc_beginproject", "mc_endproject",
  "mc_setup", "mc_run", "mc_define", "mc_const", "mc_if", "mc_then",
  "mc_else", "mc_endif", "mc_loop", "mc_while", "mc_endloop", "mc_for",
  "mc_to", "mc_endfor", "mc_and", "mc_or", "mc_non", "mc_out", "mc_in",
  "err", "affect", "infeg", "supeg", "egal", "diff", "tok_pipe", "idf",
  "mc_integer", "mc_float", "tok_chaine", "cst_int", "cst_reel", "'<'",
  "'>'", "'+'", "'-'", "'*'", "'/'", "';'", "':'", "'{'", "'}'", "'='",
  "'['", "']'", "'('", "')'", "','", "$accept", "S", "DECLARATIONS", "DEC",
  "DECL_CONST", "DECL_VARIABLE", "SUITE_IDF", "DECL_TABLEAU", "TYPE",
  "INSTRUCTIONS", "INSTRUCTION", "instaff", "IF_PREFIX", "IF_SUITE", "$@1",
  "instif", "WHILE_PREFIX", "$@2", "instwhile", "FOR_PREFIX", "instfor",
  "instout", "$@3", "instin", "LIST_ARGS", "arg", "CONDITION", "EXPR",
  "VALEUR_LITTERALE", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,    60,    62,    43,
      45,    42,    47,    59,    58,   123,   125,    61,    91,    93,
      40,    41,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    53,    54,    55,    55,    56,    56,    56,    57,    58,
      58,    58,    59,    59,    60,    61,    61,    62,    62,    63,
      63,    63,    63,    63,    63,    64,    64,    65,    66,    67,
      66,    68,    70,    69,    71,    72,    73,    75,    74,    76,
      77,    77,    78,    78,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    81,    81
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,    13,     0,     2,     1,     1,     1,     7,     5,
       7,     6,     2,     3,     9,     1,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     4,     7,     6,     2,     0,
       7,     5,     0,     6,     6,     6,     6,     0,     6,     5,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     3,     1,
       4,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     3,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    17,
       0,     0,     0,     0,     0,    12,    15,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    19,     0,    20,     0,    21,     0,    22,    23,    24,
       0,     9,     0,    13,     0,     0,     0,    32,     0,    37,
       0,     0,     0,     0,    17,    17,    17,     0,    63,    64,
       0,    11,     0,     0,    59,    61,    62,     0,     0,    44,
       0,     0,     0,     0,     0,     0,     2,     0,     0,     0,
       0,    10,     8,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,    40,    42,     0,    25,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    55,    56,    51,    52,
      53,    54,    49,    50,    45,    46,    47,    48,     0,     0,
       0,     0,    39,     0,    29,     0,    31,     0,     0,    14,
      57,    60,    27,    33,    35,    38,    41,     0,     0,    28,
      34,    36,    26,    17,     0,     0,     0,    30
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     8,    12,    13,    14,    22,    15,    29,    24,
      40,    41,    42,   146,   158,    43,    44,    80,    45,    46,
      47,    48,    82,    49,   112,   113,    78,    79,    70
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -65
static const yytype_int16 yypact[] =
{
      28,     2,    54,    13,   -65,    56,    15,   -65,   126,    30,
      61,    67,   -65,   -65,   -65,   -65,    55,   -10,    59,   -65,
      75,   -27,     8,    37,    42,   -65,   -65,   -65,    37,   -25,
      78,    37,    66,    68,   106,    85,    77,    87,   -16,   145,
     -65,   -65,   105,   -65,   107,   -65,   114,   -65,   -65,   -65,
     108,   -65,    43,   -65,   115,    43,    41,   -65,   137,   -65,
     130,    41,    41,   119,   -65,   -65,   -65,   129,   -65,   -65,
     120,   -65,   132,   121,   124,   -65,   -65,    41,   125,   254,
     127,    41,    76,   133,   204,   -12,   -65,    44,    71,    73,
     136,   -65,   -65,    41,    41,   102,   168,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
     180,   -65,    84,   -65,   254,   143,   -65,   162,   113,   173,
     171,   147,   128,   154,   -65,   157,   277,   283,   -29,   -29,
     -29,   -29,   -29,   -29,   104,   104,   -65,   -65,   151,    41,
     155,    76,   -65,    41,   -65,   161,   -65,   167,   169,   -65,
     -65,   -65,   -65,   -65,   254,   -65,   -65,   229,   160,   -65,
     -65,   -65,   -65,   -65,    92,   199,   170,   -65
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,    40,   -64,
     -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,
     -65,   -65,   -65,   -65,   -65,    74,   116,   -58,   159
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      87,    88,    89,    84,    85,    26,    27,    97,    98,    61,
     105,   106,   107,   108,    99,   100,   101,   102,    51,    95,
      20,    28,    52,   110,   114,   103,   104,   105,   106,   107,
     108,     1,    62,     3,    21,   122,   123,   117,    30,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    33,    31,    33,     4,    34,     5,    34,    35,     7,
      35,     6,    73,    32,    36,    37,    36,    37,    50,    26,
      27,    54,    74,    38,    16,    38,    75,    76,    68,    69,
      33,   154,    33,   114,    34,   157,    34,    35,    39,    35,
     118,    77,    17,    36,    37,    36,    37,    73,    18,   164,
      19,    33,    38,    23,    38,    34,    25,    74,    35,    53,
     111,    75,    76,    55,    36,    37,    58,   119,    56,   120,
      57,    97,    98,    38,   144,   145,    77,    59,    99,   100,
     101,   102,     9,    10,    11,   140,   141,    60,   165,   103,
     104,   105,   106,   107,   108,   107,   108,    97,    98,    63,
      64,    67,    65,   124,    99,   100,   101,   102,    71,    66,
      81,    83,    86,    91,    90,   103,   104,   105,   106,   107,
     108,    93,    94,    97,    98,    92,    96,   109,   125,   150,
      99,   100,   101,   102,   115,   121,   142,   143,   147,   148,
     149,   103,   104,   105,   106,   107,   108,   139,   155,    97,
      98,   152,   153,   151,   159,   163,    99,   100,   101,   102,
     160,   166,   161,   167,    72,   156,     0,   103,   104,   105,
     106,   107,   108,    97,    98,   138,     0,     0,     0,     0,
      99,   100,   101,   102,     0,     0,     0,     0,     0,     0,
       0,   103,   104,   105,   106,   107,   108,   116,    97,    98,
       0,     0,     0,     0,     0,    99,   100,   101,   102,     0,
       0,     0,     0,     0,     0,     0,   103,   104,   105,   106,
     107,   108,   162,    97,    98,     0,     0,     0,     0,     0,
      99,   100,   101,   102,     0,     0,     0,     0,     0,     0,
       0,   103,   104,   105,   106,   107,   108,    98,     0,     0,
       0,     0,     0,    99,   100,   101,   102,     0,     0,    99,
     100,   101,   102,     0,   103,   104,   105,   106,   107,   108,
     103,   104,   105,   106,   107,   108
};

static const yytype_int16 yycheck[] =
{
      64,    65,    66,    61,    62,    32,    33,    19,    20,    25,
      39,    40,    41,    42,    26,    27,    28,    29,    43,    77,
      30,    48,    47,    81,    82,    37,    38,    39,    40,    41,
      42,     3,    48,    31,    44,    93,    94,    49,    30,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,     9,    44,     9,     0,    13,    43,    13,    16,    44,
      16,     5,    21,    23,    22,    23,    22,    23,    28,    32,
      33,    31,    31,    31,    44,    31,    35,    36,    35,    36,
       9,   139,     9,   141,    13,   143,    13,    16,    46,    16,
      46,    50,    31,    22,    23,    22,    23,    21,    31,   163,
      45,     9,    31,    44,    31,    13,    31,    31,    16,    31,
      34,    35,    36,    47,    22,    23,    31,    46,    50,    46,
      14,    19,    20,    31,    11,    12,    50,    50,    26,    27,
      28,    29,     6,     7,     8,    51,    52,    50,    46,    37,
      38,    39,    40,    41,    42,    41,    42,    19,    20,     4,
      45,    43,    45,    51,    26,    27,    28,    29,    43,    45,
      23,    31,    43,    43,    35,    37,    38,    39,    40,    41,
      42,    50,    48,    19,    20,    43,    51,    50,    10,    51,
      26,    27,    28,    29,    51,    49,    43,    25,    15,    18,
      43,    37,    38,    39,    40,    41,    42,    17,    43,    19,
      20,    44,    51,    49,    43,    45,    26,    27,    28,    29,
      43,    12,    43,    43,    55,   141,    -1,    37,    38,    39,
      40,    41,    42,    19,    20,   109,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    19,    20,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    43,    19,    20,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    20,    -1,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    -1,    -1,    26,
      27,    28,    29,    -1,    37,    38,    39,    40,    41,    42,
      37,    38,    39,    40,    41,    42
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    54,    31,     0,    43,     5,    44,    55,     6,
       7,     8,    56,    57,    58,    60,    44,    31,    31,    45,
      30,    44,    59,    44,    62,    31,    32,    33,    48,    61,
      30,    44,    61,     9,    13,    16,    22,    23,    31,    46,
      63,    64,    65,    68,    69,    71,    72,    73,    74,    76,
      61,    43,    47,    31,    61,    47,    50,    14,    31,    50,
      50,    25,    48,     4,    45,    45,    45,    43,    35,    36,
      81,    43,    81,    21,    31,    35,    36,    50,    79,    80,
      70,    23,    75,    31,    80,    80,    43,    62,    62,    62,
      35,    43,    43,    50,    48,    80,    51,    19,    20,    26,
      27,    28,    29,    37,    38,    39,    40,    41,    42,    50,
      80,    34,    77,    78,    80,    51,    43,    49,    46,    46,
      46,    49,    80,    80,    51,    10,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    79,    17,
      51,    52,    43,    25,    11,    12,    66,    15,    18,    43,
      51,    49,    44,    51,    80,    43,    78,    80,    67,    43,
      43,    43,    43,    45,    62,    46,    12,    43
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 142 "syn.y"
    {
          ajouterQuad("HALT", "", "", "");
          free((yyvsp[(2) - (13)].str));
          printf("Analyse syntaxique et semantique reussie.\n");
          YYACCEPT;
      ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 163 "syn.y"
    {
          if (rechercheType((yyvsp[(2) - (7)].str))) {
              erreur_semantique("double declaration", (yyvsp[(2) - (7)].str));
          } else {
              if (!type_compatible(SauvType, ((ExprInfo *)(yyvsp[(6) - (7)].expr))->type))
                  erreur_semantique("incompatibilite de types dans declaration constante", (yyvsp[(2) - (7)].str));
              mettreAjourCode((yyvsp[(2) - (7)].str), "const");
              insererType((yyvsp[(2) - (7)].str), SauvType);
              marquerConstante((yyvsp[(2) - (7)].str), 1);
              sauvegarder_valeur_expr((yyvsp[(2) - (7)].str), (ExprInfo *)(yyvsp[(6) - (7)].expr));
          }
          free((yyvsp[(2) - (7)].str));
          liberer_expr((ExprInfo *)(yyvsp[(6) - (7)].expr));
      ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 181 "syn.y"
    {
          if (rechercheType((yyvsp[(2) - (5)].str)))
              erreur_semantique("double declaration", (yyvsp[(2) - (5)].str));
          else {
              insererType((yyvsp[(2) - (5)].str), SauvType);
              mettreAjourCode((yyvsp[(2) - (5)].str), "var");
          }
          free((yyvsp[(2) - (5)].str));
      ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 191 "syn.y"
    {
          if (rechercheType((yyvsp[(2) - (7)].str))) {
              erreur_semantique("double declaration", (yyvsp[(2) - (7)].str));
          } else {
              if (!type_compatible(SauvType, ((ExprInfo *)(yyvsp[(6) - (7)].expr))->type))
                  erreur_semantique("incompatibilite de types dans initialisation", (yyvsp[(2) - (7)].str));
              insererType((yyvsp[(2) - (7)].str), SauvType);
              mettreAjourCode((yyvsp[(2) - (7)].str), "var");
              sauvegarder_valeur_expr((yyvsp[(2) - (7)].str), (ExprInfo *)(yyvsp[(6) - (7)].expr));
              ajouterQuad(":=", ((ExprInfo *)(yyvsp[(6) - (7)].expr))->place, "", (yyvsp[(2) - (7)].str));
          }
          free((yyvsp[(2) - (7)].str));
          liberer_expr((ExprInfo *)(yyvsp[(6) - (7)].expr));
      ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 206 "syn.y"
    {
          ajouterAttente((yyvsp[(2) - (6)].str));
          appliquerTypeVar();
          viderAttente();
          free((yyvsp[(2) - (6)].str));
      ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 216 "syn.y"
    { ajouterAttente((yyvsp[(2) - (2)].str)); free((yyvsp[(2) - (2)].str)); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 218 "syn.y"
    { ajouterAttente((yyvsp[(3) - (3)].str)); free((yyvsp[(3) - (3)].str)); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 223 "syn.y"
    {
          if (rechercheType((yyvsp[(2) - (9)].str))) {
              erreur_semantique("double declaration", (yyvsp[(2) - (9)].str));
          } else {
              if ((yyvsp[(7) - (9)].entier) <= 0)
                  erreur_semantique("taille tableau positive requise", (yyvsp[(2) - (9)].str));
              else {
                  insererType((yyvsp[(2) - (9)].str), SauvType);
                  insererTaille((yyvsp[(2) - (9)].str), (yyvsp[(7) - (9)].entier));
                  mettreAjourCode((yyvsp[(2) - (9)].str), "tab");
                  marquerTableau((yyvsp[(2) - (9)].str), 1);
              }
          }
          free((yyvsp[(2) - (9)].str));
      ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 241 "syn.y"
    { strcpy(SauvType, "integer"); free((yyvsp[(1) - (1)].str)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 242 "syn.y"
    { strcpy(SauvType, "float");   free((yyvsp[(1) - (1)].str)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 261 "syn.y"
    {
          verifierVariableSimple((yyvsp[(1) - (4)].str));
          verifierConstanteModifiable((yyvsp[(1) - (4)].str));
          verifierTypeAffectation((yyvsp[(1) - (4)].str), (ExprInfo *)(yyvsp[(3) - (4)].expr));
          ajouterQuad(":=", ((ExprInfo *)(yyvsp[(3) - (4)].expr))->place, "", (yyvsp[(1) - (4)].str));
          free((yyvsp[(1) - (4)].str));
          liberer_expr((ExprInfo *)(yyvsp[(3) - (4)].expr));
      ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 270 "syn.y"
    {
          verifierTableauIndice((yyvsp[(1) - (7)].str), (ExprInfo *)(yyvsp[(3) - (7)].expr));
          verifierConstanteModifiable((yyvsp[(1) - (7)].str));
          verifierTypeAffectation((yyvsp[(1) - (7)].str), (ExprInfo *)(yyvsp[(6) - (7)].expr));
          ajouterQuad("WRITETAB", ((ExprInfo *)(yyvsp[(6) - (7)].expr))->place, ((ExprInfo *)(yyvsp[(3) - (7)].expr))->place, (yyvsp[(1) - (7)].str));
          free((yyvsp[(1) - (7)].str));
          liberer_expr((ExprInfo *)(yyvsp[(3) - (7)].expr));
          liberer_expr((ExprInfo *)(yyvsp[(6) - (7)].expr));
      ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 283 "syn.y"
    {
          if (strcmp(((ExprInfo *)(yyvsp[(3) - (6)].expr))->type, "integer") != 0)
              erreur_semantique("condition if invalide", "if");
          debut_if((ExprInfo *)(yyvsp[(3) - (6)].expr));
          liberer_expr((ExprInfo *)(yyvsp[(3) - (6)].expr));
      ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 293 "syn.y"
    { fin_if_sans_else(); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 295 "syn.y"
    { partie_else(); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 297 "syn.y"
    { fin_if_avec_else(); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 304 "syn.y"
    { debut_while(); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 306 "syn.y"
    {
          if (strcmp(((ExprInfo *)(yyvsp[(5) - (6)].expr))->type, "integer") != 0)
              erreur_semantique("condition while invalide", "while");
          condition_while((ExprInfo *)(yyvsp[(5) - (6)].expr));
          liberer_expr((ExprInfo *)(yyvsp[(5) - (6)].expr));
      ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 316 "syn.y"
    { fin_while(); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 321 "syn.y"
    {
          verifierVariableSimple((yyvsp[(2) - (6)].str));
          verifierConstanteModifiable((yyvsp[(2) - (6)].str));
          if (strcmp(getType((yyvsp[(2) - (6)].str)), "integer") != 0)
              erreur_semantique("variable du for non entiere", (yyvsp[(2) - (6)].str));
          if (strcmp(((ExprInfo *)(yyvsp[(4) - (6)].expr))->type, "integer") != 0 ||
              strcmp(((ExprInfo *)(yyvsp[(6) - (6)].expr))->type, "integer") != 0)
              erreur_semantique("bornes du for non entieres", (yyvsp[(2) - (6)].str));
          verifierTypeAffectation((yyvsp[(2) - (6)].str), (ExprInfo *)(yyvsp[(4) - (6)].expr));
          debut_for((yyvsp[(2) - (6)].str), (ExprInfo *)(yyvsp[(4) - (6)].expr), (ExprInfo *)(yyvsp[(6) - (6)].expr));
          free((yyvsp[(2) - (6)].str));
          liberer_expr((ExprInfo *)(yyvsp[(4) - (6)].expr));
          liberer_expr((ExprInfo *)(yyvsp[(6) - (6)].expr));
      ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 339 "syn.y"
    { fin_for(); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 344 "syn.y"
    { ajouterQuad("BEGIN_OUT", "", "", ""); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 346 "syn.y"
    { ajouterQuad("END_OUT", "", "", ""); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 351 "syn.y"
    {
          verifierVariableSimple((yyvsp[(3) - (5)].str));
          verifierConstanteModifiable((yyvsp[(3) - (5)].str));
          ajouterQuad("IN", "", "", (yyvsp[(3) - (5)].str));
          free((yyvsp[(3) - (5)].str));
      ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 366 "syn.y"
    { ajouterQuad("OUT", ((ExprInfo *)(yyvsp[(1) - (1)].expr))->place, "", ""); liberer_expr((ExprInfo *)(yyvsp[(1) - (1)].expr)); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 368 "syn.y"
    { ajouterQuad("OUT", (yyvsp[(1) - (1)].str), "", ""); free((yyvsp[(1) - (1)].str)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 371 "syn.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 375 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "+");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "+");
          (yyval.expr) = (void *)expr_arith(a, b, '+');
      ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 384 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "-");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "-");
          (yyval.expr) = (void *)expr_arith(a, b, '-');
      ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 393 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "*");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "*");
          (yyval.expr) = (void *)expr_arith(a, b, '*');
      ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 402 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "/");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "/");
          if (b->isConst && ((b->isInt && b->ival==0)||((!b->isInt)&&b->fval==0.0f)))
              erreur_semantique("division par zero", "/");
          (yyval.expr) = (void *)expr_arith(a, b, '/');
      ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 413 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<");
          (yyval.expr) = (void *)expr_comp(a, b, "<", 1);
      ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 422 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">");
          (yyval.expr) = (void *)expr_comp(a, b, ">", 2);
      ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 431 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<=");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<=");
          (yyval.expr) = (void *)expr_comp(a, b, "<=", 3);
      ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 440 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">=");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">=");
          (yyval.expr) = (void *)expr_comp(a, b, ">=", 4);
      ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 449 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "==");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "==");
          (yyval.expr) = (void *)expr_comp(a, b, "==", 5);
      ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 458 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "!=");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "!=");
          (yyval.expr) = (void *)expr_comp(a, b, "!=", 6);
      ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 467 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type, "integer") != 0)
              erreur_semantique("operande AND non entier", "AND");
          if (strcmp(b->type, "integer") != 0)
              erreur_semantique("operande AND non entier", "AND");
          (yyval.expr) = (void *)expr_logique(a, b, "AND", 1);
      ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 476 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(1) - (3)].expr), *b = (ExprInfo *)(yyvsp[(3) - (3)].expr);
          if (strcmp(a->type, "integer") != 0)
              erreur_semantique("operande OR non entier", "OR");
          if (strcmp(b->type, "integer") != 0)
              erreur_semantique("operande OR non entier", "OR");
          (yyval.expr) = (void *)expr_logique(a, b, "OR", 2);
      ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 485 "syn.y"
    {
          ExprInfo *a = (ExprInfo *)(yyvsp[(3) - (4)].expr);
          if (strcmp(a->type, "integer") != 0)
              erreur_semantique("operande NON non entier", "NON");
          (yyval.expr) = (void *)expr_non(a);
      ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 491 "syn.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 493 "syn.y"
    {
          char place[64];
          verifierVariableSimple((yyvsp[(1) - (1)].str));
          strncpy(place, (yyvsp[(1) - (1)].str), 63); place[63] = '\0';
          (yyval.expr) = (void *)creer_expr(getType((yyvsp[(1) - (1)].str)), 0, 0, 0, 0.0f, place);
          free((yyvsp[(1) - (1)].str));
      ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 501 "syn.y"
    {
          char temp[32];
          verifierTableauIndice((yyvsp[(1) - (4)].str), (ExprInfo *)(yyvsp[(3) - (4)].expr));
          nouveauTemp(temp);
          ajouterQuad("READTAB", (yyvsp[(1) - (4)].str), ((ExprInfo *)(yyvsp[(3) - (4)].expr))->place, temp);
          (yyval.expr) = (void *)creer_expr(getType((yyvsp[(1) - (4)].str)), 0, 0, 0, 0.0f, temp);
          free((yyvsp[(1) - (4)].str));
          liberer_expr((ExprInfo *)(yyvsp[(3) - (4)].expr));
      ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 511 "syn.y"
    {
          char place[64];
          sprintf(place, "%d", (yyvsp[(1) - (1)].entier));
          (yyval.expr) = (void *)creer_expr("integer", 1, 1, (yyvsp[(1) - (1)].entier), (float)(yyvsp[(1) - (1)].entier), place);
      ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 517 "syn.y"
    {
          char place[64];
          sprintf(place, "%g", (double)(yyvsp[(1) - (1)].reel));
          (yyval.expr) = (void *)creer_expr("float", 1, 0, 0, (yyvsp[(1) - (1)].reel), place);
      ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 526 "syn.y"
    {
          char place[64];
          sprintf(place, "%d", (yyvsp[(1) - (1)].entier));
          (yyval.expr) = (void *)creer_expr("integer", 1, 1, (yyvsp[(1) - (1)].entier), (float)(yyvsp[(1) - (1)].entier), place);
      ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 532 "syn.y"
    {
          char place[64];
          sprintf(place, "%g", (double)(yyvsp[(1) - (1)].reel));
          (yyval.expr) = (void *)creer_expr("float", 1, 0, 0, (yyvsp[(1) - (1)].reel), place);
      ;}
    break;



/* Line 1455 of yacc.c  */
#line 2190 "syn.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 539 "syn.y"


void yyerror(const char *msg)
{
    fprintf(stderr, "Erreur syntaxique : %s | Ligne %d, Colonne %d\n",
            msg, ligne, colonne);
}

int main(void)
{
    int r = yyparse();
    if (r == 0) {
        
        afficher();
    }
    liberer();
    return 0;
}

