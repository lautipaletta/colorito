import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import opacity_image, save_image, open_image

def test_opacity_image():
    img = open_image("tests/input/prueba.png")
    img_with_alpha = opacity_image(img, 0.5)
    os.makedirs("tests/output", exist_ok=True)
    save_image(img_with_alpha, "tests/output/opacity_image_result.png")
    print("test_opacity_image: OK")

if __name__ == "__main__":
    test_opacity_image()
