import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import contrast_image, save_image, open_image

def test_contrast_image():
    img = open_image("test/python/input/prueba.jpg")
    contrast = contrast_image(img, 0.5)
    os.makedirs("test/python/output", exist_ok=True)
    save_image(contrast, "test/python/output/contrast_image_result.jpg")
    print("test_contrast_image: OK")

if __name__ == "__main__":
    test_contrast_image()
