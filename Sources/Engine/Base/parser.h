/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_ENGINE_BASE_PARSER_HPP_INCLUDED
# define YY_YY_ENGINE_BASE_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    c_float = 258,
    c_int = 259,
    c_string = 260,
    c_char = 261,
    identifier = 262,
    k_INDEX = 263,
    k_FLOAT = 264,
    k_CTString = 265,
    k_void = 266,
    k_const = 267,
    k_user = 268,
    k_persistent = 269,
    k_extern = 270,
    k_pre = 271,
    k_post = 272,
    k_help = 273,
    k_if = 274,
    k_else = 275,
    k_else_if = 276,
    SHL = 277,
    SHR = 278,
    EQ = 279,
    NEQ = 280,
    GEQ = 281,
    LEQ = 282,
    LOGAND = 283,
    LOGOR = 284,
    block_beg = 285,
    block_end = 286,
    TYPECAST = 287,
    SIGN = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 197 "Engine/Base/Parser.y"

  value val;                  // for constants and expressions
  arguments arg;               // for function input arguments
  ULONG ulFlags;              // for declaration qualifiers
  INDEX istType;              // for types
  CShellSymbol *pssSymbol;    // for symbols
  struct LValue lvLValue;
  INDEX (*pPreFunc)(INDEX);  // pre-set function for a variable
  void (*pPostFunc)(INDEX); // post-set function for a variable

#line 102 "Engine/Base/Parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void);

#endif /* !YY_YY_ENGINE_BASE_PARSER_HPP_INCLUDED  */
