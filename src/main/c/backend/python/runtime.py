import sys
import logging
import traceback

# Setup logging
logging.basicConfig(
    level=logging.INFO,
    format='[%(asctime)s] %(levelname)s: %(message)s',
    handlers=[logging.StreamHandler(sys.stdout)]
)

logger = logging.getLogger(__name__)


def run_generated_code(code_path):
    """
    Ejecuta código Python generado desde un archivo.
    """
    try:
        with open(code_path, 'r', encoding='utf-8') as f:
            code = f.read()
        logger.info(f"Ejecutando código generado desde: {code_path}")
        # Definir entorno seguro para ejecución
        exec_globals = {
            '__builtins__': __builtins__,
            # Puedes agregar aquí funciones utilitarias si necesitas
        }
        exec(code, exec_globals)
        logger.info("Ejecución finalizada correctamente.")
    except Exception as e:
        logger.error(f"Error durante la ejecución: {e}")
        traceback.print_exc()


def main():
    if len(sys.argv) != 2:
        print("Uso: python runtime.py <archivo_codigo_generado.py>")
        sys.exit(1)
    code_path = sys.argv[1]
    run_generated_code(code_path)


if __name__ == "__main__":
    main()
