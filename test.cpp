#include "imagesaver.h"

int main(int argc, char *argv[])
{
    {
        imageRGBA image1 = loadImageRGBA("test.png");
        for (int i = 50; i < 100; i++)
            for (int j = 50; j < 100; j++)
            {
                image1.setPixel(i, j, pixelRGBA{125, 0, 100, 255});
            }
        saveImageRGBA("test1.png", image1);
    }
    return 1;
}