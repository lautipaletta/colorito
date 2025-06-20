import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import blur_image, save_image, open_image

def test_blur_image():
    img = open_image("test/python/input/prueba.png")
    blurred = blur_image(img, 8)
    os.makedirs("test/python/output", exist_ok=True)
    save_image(blurred, "test/python/output/blur_image_result.png")
    print("test_blur_image: OK")

if __name__ == "__main__":
    test_blur_image()
