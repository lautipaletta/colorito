import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import open_image, save_image

def test_open_image():
    # Crear imagen de prueba si no existe
    img_path = "test/python/input/prueba.png"
    os.makedirs(os.path.dirname(img_path), exist_ok=True)
    if not os.path.exists(img_path):
        img = Image.new("RGB", (100, 100), color="red")
        img.save(img_path)
    # Probar open_image
    img = open_image(img_path)
    assert isinstance(img, Image.Image)
    # Guardar copia para verificar
    os.makedirs("test/python/output", exist_ok=True)
    save_image(img, "test/python/output/open_image_result.png")
    print("test_open_image: OK")

if __name__ == "__main__":
    test_open_image()
