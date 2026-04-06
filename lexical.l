%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "syn.tab.h"

#define TS_DEFINE
#include "ts.h"

extern YYSTYPE yylval;

int ligne   = 1;
int colonne = 1;

static char *dupliquer_chaine(const char *s)
{
    char *p;
    size_t n;

    n = strlen(s) + 1;
    p = (char *)malloc(n);
    if (p != NULL)
        memcpy(p, s, n);
    return p;
}

static int verifier_borne_int(long v)
{
    return (v >= -32768 && v <= 32767);
}

static long extraire_entier_signe(const char *txt)
{
    char buf[64];
    int i, j;

    j = 0;
    for (i = 0; txt[i] != '\0' && j < 63; i++) {
        if (txt[i] != '(' && txt[i] != ')' && txt[i] != ' ' && txt[i] != '\t')
            buf[j++] = txt[i];
    }
    buf[j] = '\0';
    return strtol(buf, NULL, 10);
}

static double extraire_reel_signe(const char *txt)
{
    char buf[128];
    int i, j;

    j = 0;
    for (i = 0; txt[i] != '\0' && j < 127; i++) {
        if (txt[i] != '(' && txt[i] != ')' && txt[i] != ' ' && txt[i] != '\t')
            buf[j++] = txt[i];
    }
    buf[j] = '\0';
    return strtod(buf, NULL);
}
%}

%option noyywrap
%x COMMENTAIRE

DIGIT       [0-9]
LETTER      [A-Za-z]
IDF         {LETTER}({LETTER}|{DIGIT}|(_({LETTER}|{DIGIT})))*
INT         {DIGIT}+
REAL        {DIGIT}+"."{DIGIT}+
SINT        \([ \t]*[+-][ \t]*{DIGIT}+[ \t]*\)
SREAL       \([ \t]*[+-][ \t]*{DIGIT}+"."{DIGIT}+[ \t]*\)

%%

"BeginProject"  { colonne += yyleng; return mc_beginproject; }
"EndProject"    { colonne += yyleng; return mc_endproject; }
"Setup"         { colonne += yyleng; return mc_setup; }
"Run"           { colonne += yyleng; return mc_run; }

"define"        { colonne += yyleng; return mc_define; }
"const"         { colonne += yyleng; return mc_const; }

"integer"       { yylval.str = dupliquer_chaine(yytext); colonne += yyleng; return mc_integer; }
"float"         { yylval.str = dupliquer_chaine(yytext); colonne += yyleng; return mc_float; }

"if"            { colonne += yyleng; return mc_if; }
"then"          { colonne += yyleng; return mc_then; }
"else"          { colonne += yyleng; return mc_else; }
"endIf"         { colonne += yyleng; return mc_endif; }

"loop"          { colonne += yyleng; return mc_loop; }
"while"         { colonne += yyleng; return mc_while; }
"endloop"       { colonne += yyleng; return mc_endloop; }

"for"           { colonne += yyleng; return mc_for; }
"in"            { colonne += yyleng; return mc_in; }
"to"            { colonne += yyleng; return mc_to; }
"endfor"        { colonne += yyleng; return mc_endfor; }

"AND"           { colonne += yyleng; return mc_and; }
"OR"            { colonne += yyleng; return mc_or; }
"NON"           { colonne += yyleng; return mc_non; }

"out"           { colonne += yyleng; return mc_out; }

"<-"            { colonne += yyleng; return affect; }
"<="            { colonne += yyleng; return infeg; }
">="            { colonne += yyleng; return supeg; }
"=="            { colonne += yyleng; return egal; }
"!="            { colonne += yyleng; return diff; }

"="             { colonne += yyleng; return '='; }
"+"             { colonne += yyleng; return '+'; }
"-"             { colonne += yyleng; return '-'; }
"*"             { colonne += yyleng; return '*'; }
"/"             { colonne += yyleng; return '/'; }
">"             { colonne += yyleng; return '>'; }
"<"             { colonne += yyleng; return '<'; }

";"             { colonne += yyleng; return ';'; }
":"             { colonne += yyleng; return ':'; }
"["             { colonne += yyleng; return '['; }
"]"             { colonne += yyleng; return ']'; }
"("             { colonne += yyleng; return '('; }
")"             { colonne += yyleng; return ')'; }
","             { colonne += yyleng; return ','; }
"{"             { colonne += yyleng; return '{'; }
"}"             { colonne += yyleng; return '}'; }
"|"             { colonne += yyleng; return tok_pipe; }

\"([^\"\n]*)\"  {
    yylval.str = dupliquer_chaine(yytext);
    colonne += yyleng;
    return tok_chaine;
}

{SREAL} {
    yylval.reel = (float) extraire_reel_signe(yytext);
    colonne += yyleng;
    return cst_reel;
}

{SINT} {
    long v;
    v = extraire_entier_signe(yytext);

    if (!verifier_borne_int(v)) {
        fprintf(stderr,
            "Erreur lexicale : entier hors bornes, ligne %d, colonne %d, entite %s\n",
            ligne, colonne, yytext);
        colonne += yyleng;
        return err;
    }

    yylval.entier = (int)v;
    colonne += yyleng;
    return cst_int;
}

{REAL} {
    yylval.reel = (float) atof(yytext);
    colonne += yyleng;
    return cst_reel;
}

{INT} {
    long v;
    v = strtol(yytext, NULL, 10);

    if (!verifier_borne_int(v)) {
        fprintf(stderr,
            "Erreur lexicale : entier hors bornes, ligne %d, colonne %d, entite %s\n",
            ligne, colonne, yytext);
        colonne += yyleng;
        return err;
    }

    yylval.entier = (int)v;
    colonne += yyleng;
    return cst_int;
}

{IDF} {
    if (yyleng > 14) {
        fprintf(stderr,
            "Erreur lexicale : identificateur trop long, ligne %d, colonne %d, entite %s\n",
            ligne, colonne, yytext);
        colonne += yyleng;
        return err;
    }

    yylval.str = dupliquer_chaine(yytext);
    inserer(yytext, "idf");
    colonne += yyleng;
    return idf;
}

"%%"[^\n]* {
    colonne += yyleng;
}

"//*" {
    BEGIN(COMMENTAIRE);
    colonne += yyleng;
}

<COMMENTAIRE>"*//" {
    BEGIN(INITIAL);
    colonne += yyleng;
}

<COMMENTAIRE>\n {
    ligne++;
    colonne = 1;
}

<COMMENTAIRE>. {
    colonne += yyleng;
}

[ \t\r]+ {
    colonne += yyleng;
}

\n {
    ligne++;
    colonne = 1;
}

. {
    fprintf(stderr,
        "Erreur lexicale : caractere inconnu, ligne %d, colonne %d, entite %s\n",
        ligne, colonne, yytext);
    colonne += yyleng;
    return err;
}

%%