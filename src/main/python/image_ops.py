import logging
import numpy as np
from PIL import Image, ImageOps, ImageEnhance, ImageFilter, ImageColor

logger = logging.getLogger(__name__)

def parse_color(c):
    if isinstance(c, tuple):
        return c
    try:
        return ImageColor.getrgb(c)
    except Exception:
        raise ValueError(f"Invalid color: {c}")

# --- Wrappers for image operations ---

def open_image(path: str) -> Image.Image:
    """
    Opens an image from the specified path.
    :param path: Path to the image file.
    :return: PIL.Image.Image object
    Example usage:
        img = open_image("image.png")
    Example of manual creation:
        from PIL import Image
        img = Image.new("RGB", (100, 100), color="white")
    """
    logger.info(f"Opening image: {path}")
    return Image.open(path)

def save_image(img: Image.Image, path: str) -> None:
    """
    Saves an image to the specified path.
    :param img: PIL.Image.Image object
    :param path: Destination path for the image file
    Example usage:
        save_image(img, "new_image.png")
    """
    logger.info(f"Saving image to: {path}")
    img.save(path)

def crop_image(img: Image.Image, factor: int, n: int) -> Image.Image:
    """
    Splits the image into 'factor' equal horizontal parts and returns the nth part (starting at 0).
    :param img: PIL.Image.Image object
    :param factor: Number of horizontal parts to split the image into
    :param n: Index of the part to return (0 <= n < factor)
    :return: Corresponding subimage (PIL.Image.Image)
    Example usage:
        cropped = crop_image(img, 3, 1)  # Returns the second horizontal third
    """
    if factor < 1:
        raise ValueError("Factor must be >= 1")
    if not (0 <= n < factor):
        raise ValueError(f"n must be between 0 and factor-1 (n={n}, factor={factor})")
    w, h = img.size
    logger.info(f"Dividing image into {factor} horizontal parts, returning part {n}")
    part_width = w // factor
    x0 = n * part_width
    x1 = (n + 1) * part_width if n < factor - 1 else w
    return img.crop((x0, 0, x1, h))

def resize_image(img: Image.Image, size: tuple) -> Image.Image:
    """
    Resizes the image to the given size.
    :param img: PIL.Image.Image object
    :param size: Tuple (width, height)
    :return: Resized image
    Example usage:
        resized = resize_image(img, (800, 600))
    """
    logger.info(f"Resizing image to: {size}")
    return img.resize(size)

def rotate_image(img: Image.Image, angle: float) -> Image.Image:
    """
    Rotates the image by the specified number of degrees.
    :param img: PIL.Image.Image object
    :param angle: Degrees to rotate (float or int)
    :return: Rotated image
    Example usage:
        rotated = rotate_image(img, 90)
    """
    logger.info(f"Rotating image {angle} degrees")
    return img.rotate(angle)

def brightness_image(img: Image.Image, value: float) -> Image.Image:
    """
    Adjusts the brightness of the image.
    :param img: PIL.Image.Image object
    :param value: Brightness factor (1.0 = original, >1 brighter, <1 darker)
    :return: Brightness-adjusted image
    Example usage:
        bright = brightness_image(img, 1.2)
    """
    logger.info(f"Adjusting brightness: {value}")
    enhancer = ImageEnhance.Brightness(img)
    return enhancer.enhance(value)

def contrast_image(img: Image.Image, value: float) -> Image.Image:
    """
    Adjusts the contrast of the image.
    :param img: PIL.Image.Image object
    :param value: Contrast factor (1.0 = original, >1 more contrast, <1 less contrast)
    :return: Contrast-adjusted image
    Example usage:
        contrast = contrast_image(img, 0.8)
    """
    logger.info(f"Adjusting contrast: {value}")
    enhancer = ImageEnhance.Contrast(img)
    return enhancer.enhance(value)

def blur_image(img: Image.Image, radius: float) -> Image.Image:
    """
    Applies a Gaussian blur to the image.
    :param img: PIL.Image.Image object
    :param radius: Blur radius
    :return: Blurred image
    Example usage:
        blurred = blur_image(img, 5)
    """
    logger.info(f"Applying blur with radius: {radius}")
    return img.filter(ImageFilter.GaussianBlur(radius))

def pixelate_image(img: Image.Image, factor: int) -> Image.Image:
    """
    Pixelates the image by reducing and rescaling.
    :param img: PIL.Image.Image object
    :param factor: Reduction factor (>1)
    :return: Pixelated image
    Example usage:
        pixelated = pixelate_image(img, 10)
    """
    logger.info(f"Pixelating image with factor: {factor}")
    small = img.resize((img.width//factor, img.height//factor), resample=Image.NEAREST)
    return small.resize(img.size, Image.NEAREST)

def opacity_image(img: Image.Image, percentage: float) -> Image.Image:
    """
    Changes the opacity of an image.
    :param img: PIL.Image.Image object
    :param percentage: Value between 0.0 (transparent) and 1.0 (opaque)
    :return: Image with modified alpha channel
    Example usage:
        img_rgba = opacity_image(img, 0.5)
    """
    logger.info(f"Applying opacity: {percentage}")
    img_rgba = img.convert("RGBA")
    alpha = int(255 * percentage)
    img_rgba.putalpha(alpha)
    return img_rgba

def flip_image(img: Image.Image, orientation: str) -> Image.Image:
    """
    Flips the image horizontally or vertically.
    :param img: PIL.Image.Image object
    :param orientation: 'horizontal' or 'vertical'
    :return: Flipped image
    Example usage:
        flipped = flip_image(img, 'horizontal')
    """
    logger.info(f"Flipping image, orientation: {orientation}")
    if orientation == 'horizontal':
        return ImageOps.mirror(img)
    elif orientation == 'vertical':
        return ImageOps.flip(img)
    else:
        raise ValueError(f"Unknown orientation: {orientation}")

def grayscale_image(img: Image.Image) -> Image.Image:
    """
    Converts the image to grayscale.
    :param img: PIL.Image.Image object
    :return: Image in 'L' mode (grayscale)
    Example usage:
        gray = grayscale_image(img)
    """
    logger.info("Converting to grayscale")
    return img.convert("L")

def invert_image(img: Image.Image) -> Image.Image:
    """
    Inverts the colors of the image.
    :param img: PIL.Image.Image object
    :return: Inverted image
    Example usage:
        inverted = invert_image(img)
    """
    logger.info("Inverting image")
    return ImageOps.invert(img.convert("RGB"))

def sharpen_image(img: Image.Image) -> Image.Image:
    """
    Applies a sharpening filter to the image.
    :param img: PIL.Image.Image object
    :return: Sharpened image
    Example usage:
        sharp = sharpen_image(img)
    """
    logger.info("Applying sharpen filter")
    return img.filter(ImageFilter.SHARPEN)

def blend_image(img1: Image.Image, img2: Image.Image, percentage: float) -> Image.Image:
    """
    Blends two images using an alpha value.
    :param img1: First image (PIL.Image.Image)
    :param img2: Second image (PIL.Image.Image), must be the same size as img1
    :param percentage: Value between 0.0 and 1.0 (alpha for img2)
    :return: Blended image
    Example usage:
        blended = blend_image(img1, img2, 0.5)
    """
    logger.info(f"Blending images with alpha={percentage}")
    return Image.blend(img1, img2, percentage)

def merge_image(img1: Image.Image, img2: Image.Image, orientation: str) -> Image.Image:
    """
    Merges two images either horizontally or vertically.
    :param img1: First image (PIL.Image.Image)
    :param img2: Second image (PIL.Image.Image)
    :param orientation: 'horizontal' or 'vertical'
    :return: Combined image
    Example usage:
        merged = merge_image(img1, img2, 'horizontal')
    """
    logger.info(f"Merging images, orientation: {orientation}")
    if orientation == 'horizontal':
        new_img = Image.new("RGB", (img1.width + img2.width, max(img1.height, img2.height)))
        new_img.paste(img1, (0, 0))
        new_img.paste(img2, (img1.width, 0))
    elif orientation == 'vertical':
        new_img = Image.new("RGB", (max(img1.width, img2.width), img1.height + img2.height))
        new_img.paste(img1, (0, 0))
        new_img.paste(img2, (0, img1.height))
    else:
        raise ValueError(f"Unknown orientation: {orientation}")
    return new_img

def recolor_image(img: Image.Image, color1: str, color2: str, color3: str) -> Image.Image:
    """
    Recolors the image by changing all pixels whose color is between color1 and color2 (inclusive, by RGB channel) to color3.
    :param img: PIL.Image.Image object
    :param color1: Minimum color (hex, name or tuple RGB)
    :param color2: Maximum color (hex, name or tuple RGB)
    :param color3: New color (hex, name or tuple RGB)
    :return: Recolored image
    Example usage:
        recolored = recolor_image(img, '#ff0000', '#00ff00', '#0000ff')
    """
    logger.info(f"Recoloring: {color1}, {color2} -> {color3}")
    
    c1 = parse_color(color1)
    c2 = parse_color(color2)
    c3 = parse_color(color3)
    arr = np.array(img.convert("RGB"))
    # Create mask: True where each channel is between c1 and c2
    mask = np.ones(arr.shape[:2], dtype=bool)
    for ch in range(3):
        cmin, cmax = sorted([c1[ch], c2[ch]])
        mask &= (arr[..., ch] >= cmin) & (arr[..., ch] <= cmax)
    arr[mask] = c3
    return Image.fromarray(arr)

# Aliases for compatibility with grammar
def OPEN(path):
    return open_image(path)
def SAVE(img, path):
    return save_image(img, path)
def CROP(img, factor, n):
    return crop_image(img, factor, n)
def RESIZE(img, size):
    return resize_image(img, size)
def ROTATE(img, angle):
    return rotate_image(img, angle)
def BRIGHTNESS(img, value):
    return brightness_image(img, value)
def CONTRAST(img, value):
    return contrast_image(img, value)
def BLUR(img, radius):
    return blur_image(img, radius)
def PIXELATE(img, factor):
    return pixelate_image(img, factor)
def OPACITY(img, percentage):
    return opacity_image(img, percentage)
def FLIP(img, orientation):
    return flip_image(img, orientation)
def GRAYSCALE(img):
    return grayscale_image(img)
def INVERT(img):
    return invert_image(img)
def SHARPEN(img):
    return sharpen_image(img)
def BLEND(img1, img2, percentage):
    return blend_image(img1, img2, percentage)
def MERGE(img1, img2, orientation):
    return merge_image(img1, img2, orientation)
def RECOLOR(img, color1, color2, color3):
    return recolor_image(img, color1, color2, color3)
