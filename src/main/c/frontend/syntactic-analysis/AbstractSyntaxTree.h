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
typedef enum Orientation Orientation;
typedef enum FactorType FactorType;
typedef enum VariableType VariableType;
typedef enum LineType LineType;

typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef struct Line Line;
typedef struct Variable Variable;

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
    BRIGHTNESS_IMAGE, // BRIGHTNESS expression BY integer SEMICOLON
    CONTRAST_IMAGE,   // CONTRAST expression BY integer SEMICOLON
    BLUR_IMAGE,     // BLUR expression BY integer SEMICOLON
    PIXELATE_IMAGE, // PIXELATE expression BY integer SEMICOLON
    OPACITY_IMAGE,    // OPACITY expression PERCENTAGE SEMICOLON
    
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

enum Orientation {
    HORIZONTAL,
    VERTICAL
};

enum FactorType {
    FACTOR_EXPRESSION,
    FACTOR_IDENTIFIER
};

struct Factor {
    FactorType type;
    union {
        Expression * expression;
        const char * identifier;
    } data;
};

struct Expression {
    ExpressionType type;
    union {
        // Para OPEN_IMAGE
        struct {
            Variable * filename;
        } open;
        
        // Para SAVE_IMAGE
        struct {
            Factor * image;
            Variable * filename;
        } save;
        
        // Para CROP_IMAGE
        struct {
            Factor * image;
            Variable * divisions_qty;
			Variable * output_division;
        } crop;
        
        // Para RESIZE_IMAGE
        struct {
            Factor * image;
            Variable * dimension;
        } resize;
        
        // Para operaciones con valor numérico, es tambien para percentage
        struct {
            Factor * image;
            Variable * value;
        } numeric_op;
        
        // Para operaciones con dirección
        struct {
            Factor * image;
            Orientation direction;
        } directional_op;
        
        // Para operaciones sin parámetros
        struct {
            Factor * image;
        } simple_op;
        
        // Para operaciones de dos imágenes
        struct {
            Factor * image1;
            Factor * image2;
            union {
                Variable * blend_factor;			// Para BLEND_IMAGES
                Orientation direction;	// Para MERGE_IMAGES
            } param;
        } dual_op;
        
        // Para RECOLOR_IMAGE
        struct {
            Factor * image;
            Variable * from_color1;
            Variable * from_color2;
            Variable * to_color;
        } recolor;
    } data;
};

enum VariableType {
    STRING_TYPE,
    INTEGER_TYPE,
    EXPRESSION_TYPE,
    COLOR_TYPE,
    PERCENTAGE_TYPE,
    IDENTIFIER_TYPE,
    DIMENSION_TYPE
};

struct Variable {
    VariableType type;
    union {
        const char * identifier;
        const char * string;
        int integer;
        Expression * expression;
        const char * color;
        int percentage;
	    const char * dimension;
    } data;
};

struct Program {
	Line * line;
};

enum LineType {
    LINE_EXPRESSION,
    LINE_VARIABLE_DECLARATION
};

struct Line {
    LineType type;
    union {
        Expression * expression;
        struct {
            const char * identifier;
            Variable * variable;
        } variable_declaration;
    } content;
    Line * next;
};


/**
 * Node recursive destructors.
 */
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);
void releaseLine(Line * line);
void releaseVariable(Variable * variable);

#endif
