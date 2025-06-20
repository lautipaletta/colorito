import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import flip_image, save_image, open_image

def test_flip_image():
    img = open_image("tests/input/prueba.png")
    flipped = flip_image(img, 'horizontal')
    os.makedirs("tests/output", exist_ok=True)
    save_image(flipped, "tests/output/flip_image_result.png")
    print("test_flip_image: OK")

if __name__ == "__main__":
    test_flip_image()
