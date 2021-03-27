#ifndef IMAGESAVER_H
#define IMAGESAVER_H

#ifdef IMAGESAVER_EXPORTS
#define IMAGESAVERAPI __declspec(dllexport)
#else
#define IMAGESAVERAPI __declspec(dllimport)
#endif

struct colourRGBA
{
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
};

class IMAGESAVERAPI imageRGBA
{
private:
    int width;
    int height;
    colourRGBA *pixels;

public:
    imageRGBA();

    imageRGBA(int _width, int _height);

    imageRGBA(int _width, int _height, colourRGBA _defaultColor);

    imageRGBA(const imageRGBA &another);

    imageRGBA(imageRGBA &&another);

    imageRGBA &operator=(const imageRGBA &another);

    imageRGBA &operator=(imageRGBA &&another);

    ~imageRGBA();

    int getWidth() const;

    int getHeight() const;

    colourRGBA getPixel(int _width, int _height) const;

    void setPixel(int _width, int _height, colourRGBA _pixel);
};

bool IMAGESAVERAPI saveImageRGBA(const char *name, imageRGBA &image);

imageRGBA IMAGESAVERAPI loadImageRGBA(const char *name);

#endif