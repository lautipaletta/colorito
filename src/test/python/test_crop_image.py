import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import crop_image, save_image, open_image

def test_crop_image():
    img = open_image("test/python/input/prueba.jpg")
    os.makedirs("test/python/output", exist_ok=True)
    # Horizontal: dividir en 3 partes y guardar cada una
    for n in range(3):
        cropped = crop_image(img, 3, n)
        save_image(cropped, f"test/python/output/crop_image_horizontal_{n}.jpg")
    print("test_crop_image: OK")

if __name__ == "__main__":
    test_crop_image()
