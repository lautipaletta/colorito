# Colorito Runtime

Este runtime permite ejecutar programas de procesamiento de imágenes generados automáticamente por nuestra gramática, usando Python y Pillow.

## Requisitos
- Python 3.8+
- Pillow

Instalar las dependencias:
```
pip install -r requirements.txt
```

## Uso
1. Generar un archivo Python con el código generado (por ejemplo, `programa_generado.py`). Para ver como generar el código consultar la sección **start** en:
   * [Microsoft Windows](doc/readme/Windows.md)
   * [Ubuntu](doc/readme/Ubuntu.md)
2. Ejecutar el runtime:
```
python runtime.py programa_generado.py
```

Los logs de ejecución y errores se mostrarán en consola.
