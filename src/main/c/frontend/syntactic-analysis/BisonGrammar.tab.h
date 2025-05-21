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

#ifndef YY_YY_BISONGRAMMAR_TAB_H_INCLUDED
# define YY_YY_BISONGRAMMAR_TAB_H_INCLUDED
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
    STRING = 258,                  /* STRING  */
    DIMENSION = 259,               /* DIMENSION  */
    COLOR = 260,                   /* COLOR  */
    INTEGER = 261,                 /* INTEGER  */
    PERCENTAGE = 262,              /* PERCENTAGE  */
    CLOSE_PARENTHESIS = 263,       /* CLOSE_PARENTHESIS  */
    OPEN_PARENTHESIS = 264,        /* OPEN_PARENTHESIS  */
    OPEN = 265,                    /* OPEN  */
    SAVE = 266,                    /* SAVE  */
    CROP = 267,                    /* CROP  */
    RESIZE = 268,                  /* RESIZE  */
    ROTATE = 269,                  /* ROTATE  */
    FLIP = 270,                    /* FLIP  */
    MERGE = 271,                   /* MERGE  */
    BRIGHTNESS = 272,              /* BRIGHTNESS  */
    CONTRAST = 273,                /* CONTRAST  */
    OPACITY = 274,                 /* OPACITY  */
    INVERT = 275,                  /* INVERT  */
    GRAYSCALE = 276,               /* GRAYSCALE  */
    RECOLOR = 277,                 /* RECOLOR  */
    BLUR = 278,                    /* BLUR  */
    SHARPEN = 279,                 /* SHARPEN  */
    PIXELATE = 280,                /* PIXELATE  */
    BLEND = 281,                   /* BLEND  */
    EQUALS = 282,                  /* EQUALS  */
    IDENTIFIER = 283,              /* IDENTIFIER  */
    HORIZONTALLY = 284,            /* HORIZONTALLY  */
    VERTICALLY = 285,              /* VERTICALLY  */
    IN = 286,                      /* IN  */
    GET = 287,                     /* GET  */
    TO = 288,                      /* TO  */
    BY = 289,                      /* BY  */
    WITH = 290,                    /* WITH  */
    USING = 291,                   /* USING  */
    ON = 292,                      /* ON  */
    SEMICOLON = 293,               /* SEMICOLON  */
    UNKNOWN = 294                  /* UNKNOWN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#line 8 "BisonGrammar.y"
union SemanticValue
{
#line 10 "BisonGrammar.y"

	/** Terminals. */

	int integer;
	Token token;
	char * string;
	Orientation orientation;

	/** Non-terminals. */

	Expression * expression;
	Factor * factor;
	Program * program;
	Line * line;

#line 120 "BisonGrammar.tab.h"

};
#line 8 "BisonGrammar.y"
typedef union SemanticValue YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BISONGRAMMAR_TAB_H_INCLUDED  */
