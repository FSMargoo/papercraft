/**
 * \file EasyXBase.cpp
 * \brief The basic definition of EasyX
 */

#include <include/EasyXBase.h>

#ifdef _PAPER_USE_EASYX

COLORREF GetARGBColor(const int &a, const int &r, const int &g, const int &b) {
	// Check for color over range

	auto alpha = a > 255 ? 255 : a;
	auto red   = r > 255 ? 255 : r;
	auto green = g > 255 ? 255 : g;
	auto blue  = b > 255 ? 255 : b;

	alpha = alpha < 0 ? 0 : alpha;
	red	  = red < 0 ? 0 : red;
	blue  = blue < 0 ? 0 : blue;
	green = green < 0 ? 0 : green;

	return (alpha << 24) | (green << 16) | (blue << 8) | (red << 0);
}

PImage::PImage(const int &Width, const int &Height) {
	// Creating image in such size is obviously wrong
	if (Width <= 0 || Height <= 0) {
		throw PImageCreationFailure(ostr::format("Can't create a image in size {}x{}", Width, Height));
	}

	_image = new IMAGE(Width, Height);
}
PImage::PImage(const PString &Path) {
	_image = new IMAGE();

	loadimage(_image, PStringToCStr<TCHAR>(Path));

	// If the width or height is zero, we suppose it met creation failure
	if ((!_image->getheight()) || (!_image->getwidth())) {
		throw PImageCreationFailure(ostr::format("Path <{}> dose not exists!", Path));
	}
}
PImage::~PImage() {
	delete _image;
}
int PImage::GetWidth() const {
	return _image->getwidth();
}
int PImage::GetHeight() const {
	return _image->getheight();
}

PDevice::PDevice() {
	_image = nullptr;
	_hdc   = GetImageHDC();
}
PDevice::PDevice(PImage *Image) {
	if (!Image || !Image->_image) {
		throw PDeviceCreationFailure(ostr::format("Image cannot be NULL!"));
	}

	_image = Image->_image;
	_hdc   = GetImageHDC(Image->_image);
}
void PDevice::RaiseAsWorking() const {
	SetWorkingImage(_image);
}
HDC PDevice::GetHDC() const {
	return _hdc;
}
void PDevice::Flush() const {
	if (!_image) {
		FlushBatchDraw();
	}
}
PDevice *PDevice::MakeFromImage(PImage *Image) {
	return new PDevice(Image);
}
PDevice *PDevice::MakeFromScreen() {
	BeginBatchDraw();

	return new PDevice();
}

PWindow::PWindow(const int &Width, const int &Height, const PString &Title) {
	SetWindowText(initgraph(Width, Height), PStringToCStr<TCHAR>(Title));
	setbkmode(TRANSPARENT);

	RECT rectangle;
	GetWindowRect(GetHWnd(), &rectangle);

	MoveWindow(GetHWnd(), GetSystemMetrics(SM_CXSCREEN) / 2 - Width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - Height / 2,
			   rectangle.right - rectangle.left, rectangle.bottom - rectangle.top, FALSE);
}
PDevice *PWindow::GetDevice() {
	return PDevice::MakeFromScreen();
}

#endif