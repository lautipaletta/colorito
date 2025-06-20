import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import invert_image, save_image, open_image

def test_invert_image():
    img = open_image("test/python/input/prueba.jpg")
    inverted = invert_image(img)
    os.makedirs("test/python/output", exist_ok=True)
    save_image(inverted, "test/python/output/invert_image_result.jpg")
    print("test_invert_image: OK")

if __name__ == "__main__":
    test_invert_image()
