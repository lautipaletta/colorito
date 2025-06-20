import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import rotate_image, save_image, open_image

def test_rotate_image():
    img = open_image("test/python/input/prueba.png")
    rotated = rotate_image(img, 90)
    os.makedirs("test/python/output", exist_ok=True)
    save_image(rotated, "test/python/output/rotate_image_result.png")
    print("test_rotate_image: OK")

if __name__ == "__main__":
    test_rotate_image()
