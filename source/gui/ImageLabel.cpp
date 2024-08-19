/**
 * \file ImageLabel.cpp
 * \brief The image label control
 */

#include <include/gui/ImageLabel.h>

#pragma comment(lib, "MSIMG32.LIB")

PImageLabel::PImageLabel(const PString &ImagePath) {
	_image	= new PImage(ImagePath);
	_destructImage = true;

	Resize(_image->GetWidth(), _image->GetHeight());
}
PImageLabel::PImageLabel(PImage *Image) {
	_image	= Image;

	_destructImage = false;

	Resize(_image->GetWidth(), _image->GetHeight());
}
PImageLabel::~PImageLabel() {
	if (_destructImage) {
		delete _image;
	}
}
void PImageLabel::OnDraw(SkCanvas *Canvas) {
	Canvas->drawImageRect(_image->GetNativeImage(), SkRect(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom), SkSamplingOptions());
}