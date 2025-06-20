import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import brightness_image, save_image, open_image

def test_brightness_image():
    img = open_image("test/python/input/prueba.png")
    bright = brightness_image(img, 1.5)
    os.makedirs("test/python/output", exist_ok=True)
    save_image(bright, "test/python/output/brightness_image_result.png")
    print("test_brightness_image: OK")

if __name__ == "__main__":
    test_brightness_image()
