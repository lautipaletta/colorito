#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum DirectionType DirectionType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ExpressionType {
    // Operaciones de archivo
    OPEN_IMAGE,     // OPEN string SEMICOLON
    SAVE_IMAGE,     // SAVE expression string SEMICOLON
    
    // Operaciones de dos puntos
    CROP_IMAGE,     // CROP expression IN integer GET integer SEMICOLON
    
    // Operaciones con dimensión
    RESIZE_IMAGE,   // RESIZE expression TO dimension SEMICOLON
    
    // Operaciones con porcentaje/entero
    ROTATE_IMAGE,   // ROTATE expression BY integer SEMICOLON
    BRIGHTNESS_ADJ, // BRIGHTNESS expression BY integer SEMICOLON
    CONTRAST_ADJ,   // CONTRAST expression BY integer SEMICOLON
    BLUR_IMAGE,     // BLUR expression BY integer SEMICOLON
    PIXELATE_IMAGE, // PIXELATE expression BY integer SEMICOLON
    OPACITY_ADJ,    // OPACITY expression PERCENTAGE SEMICOLON
    
    // Operaciones con dirección
    FLIP_IMAGE,     // FLIP expression HORIZONTALLY SEMICOLON
    
    // Operaciones sin parámetros
    GRAYSCALE_IMAGE,// GRAYSCALE expression SEMICOLON
    INVERT_IMAGE,   // INVERT expression SEMICOLON
    SHARPEN_IMAGE,  // SHARPEN expression SEMICOLON
    
    // Operaciones de dos imágenes
    BLEND_IMAGES,   // BLEND expression WITH expression USING integer SEMICOLON
    MERGE_IMAGES,   // MERGE expression WITH expression HORIZONTALLY SEMICOLON
    
    // Operaciones de color
    RECOLOR_IMAGE   // RECOLOR expression COLOR COLOR TO COLOR SEMICOLON
};

enum FactorType {
    CONSTANT,
    EXPRESSION
};

enum DirectionType {
    HORIZONTAL,
    VERTICAL
};

struct Constant {
    int value;
};

struct Factor {
    union {
        Constant * constant;
        Expression * expression;
    };
    FactorType type;
};

struct Expression {
    ExpressionType type;
    union {
        // Para OPEN_IMAGE
        struct {
            char* filename;
        } open;
        
        // Para SAVE_IMAGE
        struct {
            Expression* image;
            char* filename;
        } save;
        
        // Para CROP_IMAGE
        struct {
            Expression* image;
            int divisions_qty;
			int output_division;
        } crop;
        
        // Para RESIZE_IMAGE
        struct {
            Expression* image;
            char* dimension;
        } resize;
        
        // Para operaciones con valor numérico
        struct {
            Expression* image;
            int value;
        } numeric_op;
        
        // Para operaciones con porcentaje
        // struct {
        //     Expression* image;
        //     int percentage;
        // } percentage_op;
        
        // Para operaciones con dirección
        struct {
            Expression* image;
            DirectionType direction;
        } directional_op;
        
        // Para operaciones sin parámetros
        struct {
            Expression* image;
        } simple_op;
        
        // Para operaciones de dos imágenes
        struct {
            Expression* image1;
            Expression* image2;
            union {
                int blend_factor;			// Para BLEND_IMAGES
                DirectionType direction;	// Para MERGE_IMAGES
            } param;
        } dual_op;
        
        // Para RECOLOR_IMAGE
        struct {
            Expression* image;
            char* from_color1;
            char* from_color2;
            char* to_color;
        } recolor;
    } data;
};

struct Program {
	Expression * expression;
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);

#endif
