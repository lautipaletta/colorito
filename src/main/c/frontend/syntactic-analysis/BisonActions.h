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
Expression * OpenImageExpressionSemanticAction(const char * filename);
Expression * SaveImageExpressionSemanticAction(Factor * image, const char * filename);
Expression * CropImageExpressionSemanticAction(Factor * image, int divisions_qty, int output_division);
Expression * ResizeImageExpressionSemanticAction(Factor * image, const char * dimension);
Expression * ExpressionWithIntegerSemanticAction(Factor * image, int integer, ExpressionType expressionType);
Expression * FlipImageExpressionSemanticAction(Factor * image, Orientation orientation);
Expression * ExpressionWithOnlyFactorSemanticAction(Factor * image, ExpressionType expressionType);
Expression * BlendImageExpressionSemanticAction(Factor * image1, Factor * image2, int amount);
Expression * MergeImageExpressionSemanticAction(Factor * image1, Factor * image2, Orientation orientation);
Expression * RecolorImageExpressionSemanticAction(Factor * image, const char * sourceColor1, const char * sourceColor2, const char * targetColor);

/**
 * Declaración de variables.
 */
Line * VariableDeclarationLineSemanticAction(const char * identifier, Expression * expression, Line * next);
Factor * VariableFactorSemanticAction(const char * identifier);

#endif
