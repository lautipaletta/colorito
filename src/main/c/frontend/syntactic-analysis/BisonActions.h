#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

/**
 * Constantes y factores.
 */
/*Constant * IntegerConstantSemanticAction(const int value);
Constant * StringConstantSemanticAction(const char * value);
Constant * DimensionConstantSemanticAction(const char * value);
Constant * PercentageConstantSemanticAction(const int value);
Constant * OrientationConstantSemanticAction(const Token token);
Constant * ColorConstantSemanticAction(const char * value);*/

/**
 * Expresiones generales.
 */
Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Line * line);
Factor * ExpressionSemanticAction(Expression * expression);
Line * ExpressionLineSemanticAction(Expression * expression, Line * next);

/**
 * Expresiones de operaciones de imagen.
 */
Expression * OpenImageExpressionSemanticAction(Variable * filename);
Expression * SaveImageExpressionSemanticAction(Factor * image, Variable * filename);
Expression * CropImageExpressionSemanticAction(Factor * image, Variable * divisions_qty, Variable * output_division);
Expression * ResizeImageExpressionSemanticAction(Factor * image, Variable * dimension);
Expression * ExpressionWithIntegerSemanticAction(Factor * image, Variable * integer, ExpressionType expressionType);
Expression * FlipImageExpressionSemanticAction(Factor * image, Orientation orientation);
Expression * ExpressionWithOnlyFactorSemanticAction(Factor * image, ExpressionType expressionType);
Expression * BlendImageExpressionSemanticAction(Factor * image1, Factor * image2, Variable * amount);
Expression * MergeImageExpressionSemanticAction(Factor * image1, Factor * image2, Orientation orientation);
Expression * RecolorImageExpressionSemanticAction(Factor * image, Variable * sourceColor1, Variable * sourceColor2, Variable * targetColor);

/**
 * Declaración de variables.
 */
Line * VariableDeclarationLineSemanticAction(const char * identifier, Variable * variable, Line * next);
Factor * VariableFactorSemanticAction(const char * identifier);
Variable * StringVariableSemanticAction(const char * value);
Variable * IntegerVariableSemanticAction(int value);
Variable * ExpressionVariableSemanticAction(Expression * expression);
Variable * ColorVariableSemanticAction(const char * value);
Variable * PercentageVariableSemanticAction(int value);
Variable * DimensionVariableSemanticAction(const char * value);
Variable * IdentifierVariableSemanticAction(const char * identifier);

#endif
