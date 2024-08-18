#include "../../include/gui/ImageLabel.h"

#pragma comment( lib, "MSIMG32.LIB")

void transparentimage(IMAGE *dstimg, int x, int y, IMAGE *srcimg)
{
    
}

PImageLabel::PImageLabel(const PString &ImagePath)
{
    _image  = new PImage(ImagePath);
    _device = PDevice::MakeFromImage(_image);
    
    _destructImage = true;

    Resize(_image->GetWidth(), _image->GetHeight());
}
PImageLabel::PImageLabel(PImage *Image)
{
    _image  = Image;
    _device = PDevice::MakeFromImage(_image);

    _destructImage = false;

    Resize(_image->GetWidth(), _image->GetHeight());
}
PImageLabel::~PImageLabel()
{
    delete _device;
    if (_destructImage)
    {
        delete _image;
    }
}
void PImageLabel::OnDraw()
{
    HDC targetDC = GetImageHDC(GetWorkingImage());
    HDC sourceDC = _device->GetHDC();
    int width    = GetWidth();
    int height   = GetHeight();

    BLENDFUNCTION blendFunction = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    
    AlphaBlend(targetDC, _rectangle.left, _rectangle.top, width, height, sourceDC,
        0, 0, _image->GetWidth(), _image->GetHeight(), blendFunction);
}