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
Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression);
Factor * ExpressionSemanticAction(Expression * expression);

/**
 * Expresiones de operaciones de imagen.
 */
Expression * OpenImageExpressionSemanticAction(const char * filename);
Expression * SaveImageExpressionSemanticAction(Expression * image, const char * filename);
Expression * CropImageExpressionSemanticAction(Expression * image, int divisions_qty, int output_division);
Expression * ResizeImageExpressionSemanticAction(Expression * image, const char * dimension);
Expression * RotateImageExpressionSemanticAction(Expression * image, int degrees);
Expression * BrightnessImageExpressionSemanticAction(Expression * image, int amount);
Expression * ContrastImageExpressionSemanticAction(Expression * image, int amount);
Expression * BlurImageExpressionSemanticAction(Expression * image, int amount);
Expression * PixelateImageExpressionSemanticAction(Expression * image, int amount);
Expression * OpacityImageExpressionSemanticAction(Expression * image, int percentage);
Expression * FlipImageExpressionSemanticAction(Expression * image, Orientation orientation);
Expression * GrayscaleImageExpressionSemanticAction(Expression * image);
Expression * InvertImageExpressionSemanticAction(Expression * image);
Expression * SharpenImageExpressionSemanticAction(Expression * image);
Expression * BlendImageExpressionSemanticAction(Expression * image1, Expression * image2, int amount);
Expression * MergeImageExpressionSemanticAction(Expression * image1, Expression * image2, Orientation orientation);
Expression * RecolorImageExpressionSemanticAction(Expression * image, const char * sourceColor1, const char * sourceColor2, const char * targetColor);

#endif
