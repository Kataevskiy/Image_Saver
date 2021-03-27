#include <iostream>
#include <png.h>
#include "imagesaver.h"

using namespace std;

imageRGBA::imageRGBA() : width(0),
                         height(0),
                         pixels(nullptr) {}

imageRGBA::imageRGBA(int _width, int _height) : width(_width), height(_height)
{
    pixels = new colourRGBA[height * width];
}

imageRGBA::imageRGBA(int _width, int _height, colourRGBA _defaultColor) : imageRGBA(_width, _height)
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            pixels[i * height + width] = _defaultColor;
}

imageRGBA::imageRGBA(const imageRGBA &another) : width(another.width),
                                                 height(another.height)
{
    pixels = new colourRGBA[another.height * another.width];
    for (int i = 0; i < another.height; i++)
        for (int j = 0; j < another.width; j++)
        {
            pixels[i * width + j] = another.pixels[i * width + j];
        }
}

imageRGBA::imageRGBA(imageRGBA &&another) : width(another.width),
                                            height(another.height)
{
    pixels = another.pixels;
    another.pixels = nullptr;
    another.width = 0;
    another.height = 0;
}

imageRGBA &imageRGBA::operator=(const imageRGBA &another)
{
    if (this != &another)
    {
        if (pixels != nullptr)
            delete[] pixels;

        width = another.width;
        height = another.height;
        pixels = new colourRGBA[height * width];
        for (int i = 0; i < another.height; i++)
            for (int j = 0; j < another.width; j++)
            {
                pixels[i * width + j] = another.pixels[i * width + j];
            }
    }
    return *this;
}

imageRGBA &imageRGBA::operator=(imageRGBA &&another)
{
    if (this != &another)
    {
        if (pixels != nullptr)
            delete[] pixels;

        width = another.width;
        height = another.height;
        pixels = another.pixels;
        another.width = 0;
        another.height = 0;
        another.pixels = nullptr;
    }

    return *this;
}

imageRGBA::~imageRGBA()
{
    if (pixels != nullptr)
        delete[] pixels;
}

int imageRGBA::getWidth() const
{
    return width;
}

int imageRGBA::getHeight() const
{
    return height;
}

colourRGBA imageRGBA::getPixel(int _width, int _height) const
{
    if (_width < width && _height < height)
        return pixels[_height * width + _width];
    else
        return colourRGBA{0, 0, 0, 0};
}

void imageRGBA::setPixel(int _width, int _height, colourRGBA _pixel)
{
    if (_width < width && _height < height)
        pixels[_height * width + _width] = _pixel;
}

bool saveImageRGBA(const char *name, imageRGBA &image)
{
    int width = image.getWidth();
    int height = image.getHeight();
    int bit_depth = 8;
    FILE *fp = fopen(name, "wb");
    png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop infoPtr = png_create_info_struct(pngPtr);
    png_init_io(pngPtr, fp);
    png_set_IHDR(pngPtr, infoPtr, width, height, bit_depth, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_bytepp rowPointers = (png_bytepp)png_malloc(pngPtr, sizeof(png_bytepp) * height);
    for (int i = 0; i < height; i++)
        rowPointers[i] = (png_bytep)png_malloc(pngPtr, width * 4);

    for (int hi = 0; hi < height; hi++)
        for (int wi = 0; wi < width; wi++)
        {
            rowPointers[hi][wi * 4 + 0] = image.getPixel(wi, hi).r;
            rowPointers[hi][wi * 4 + 1] = image.getPixel(wi, hi).g;
            rowPointers[hi][wi * 4 + 2] = image.getPixel(wi, hi).b;
            rowPointers[hi][wi * 4 + 3] = image.getPixel(wi, hi).a;
        }

    png_write_info(pngPtr, infoPtr);
    png_write_image(pngPtr, rowPointers);
    png_write_end(pngPtr, infoPtr);
    for (int i = 0; i < height; i++)
        png_free(pngPtr, rowPointers[i]);
    png_free(pngPtr, rowPointers);
    png_destroy_write_struct(&pngPtr, &infoPtr);
    fclose(fp);
    return true;
}

imageRGBA loadImageRGBA(const char *name)
{
    FILE *fp = fopen(name, "rb");
    if (!fp)
    {
        cout << "incorrect loading path " << name << '\n';
        imageRGBA image;
        return image;
    }
    png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop infoPtr = png_create_info_struct(pngPtr);
    setjmp(png_jmpbuf(pngPtr));
    png_init_io(pngPtr, fp);
    png_read_info(pngPtr, infoPtr);
    int width = png_get_image_width(pngPtr, infoPtr);
    int height = png_get_image_height(pngPtr, infoPtr);
    int colorType = png_get_color_type(pngPtr, infoPtr);
    int bitDepth = png_get_bit_depth(pngPtr, infoPtr);
    if (bitDepth == 16)
        png_set_strip_16(pngPtr);
    if (colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(pngPtr);
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(pngPtr);
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngPtr);
    if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(pngPtr, 255, PNG_FILLER_AFTER);
    if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(pngPtr);
    png_read_update_info(pngPtr, infoPtr);
    png_bytepp rowPointers = (png_bytepp)png_malloc(pngPtr, sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++)
        rowPointers[i] = (png_bytep)png_malloc(pngPtr, png_get_rowbytes(pngPtr, infoPtr));
    png_read_image(pngPtr, rowPointers);
    fclose(fp);

    imageRGBA image(width, height);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            colourRGBA pixel;
            pixel.r = rowPointers[i][j * 4 + 0];
            pixel.g = rowPointers[i][j * 4 + 1];
            pixel.b = rowPointers[i][j * 4 + 2];
            pixel.a = rowPointers[i][j * 4 + 3];
            image.setPixel(j, i, pixel);
        }

    for (int i = 0; i < height; i++)
        png_free(pngPtr, rowPointers[i]);
    png_free(pngPtr, rowPointers);
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    return image;
}