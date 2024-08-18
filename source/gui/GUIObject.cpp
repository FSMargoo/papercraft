#include <stdexcept>

#include "../../include/gui/GUIObject.h"

PGUIObject::PGUIObject() : _rectangle({0, 0, 0, 0}), _visible(true) {
}
void PGUIObject::Resize(const int &Width, const int &Height) {
	if (Width < 0 || Height < 0) {
		throw std::invalid_argument("PGUIObject failure: Width and Height must be greater than 0.");
	}

	_rectangle.right  = _rectangle.left + Width;
	_rectangle.bottom = _rectangle.top + Height;
}
void PGUIObject::SetWidth(const int &Width) {
	if (Width < 0) {
		throw std::invalid_argument("PGUIObject failure: Width and Height must be greater than 0.");
	}

	_rectangle.right = _rectangle.left + Width;
}
void PGUIObject::SetHeight(const int &Height) {
	if (Height < 0) {
		throw std::invalid_argument("PGUIObject failure: Width and Height must be greater than 0.");
	}

	_rectangle.bottom = _rectangle.top + Height;
}
int PGUIObject::GetWidth() const {
	return _rectangle.right - _rectangle.left;
}
int PGUIObject::GetHeight() const {
	return _rectangle.bottom - _rectangle.top;
}
void PGUIObject::Move(const int &X, const int &Y) {
	auto width	= GetWidth();
	auto height = GetHeight();

	_rectangle.left	  = X;
	_rectangle.top	  = Y;
	_rectangle.right  = X + width;
	_rectangle.bottom = Y + height;
}
int PGUIObject::GetX() const {
	return _rectangle.left;
}
int PGUIObject::GetY() const {
	return _rectangle.top;
}
void PGUIObject::Show() {
	_visible = true;
}
void PGUIObject::Hide() {
	_visible = false;
}
bool PGUIObject::IsVisible() const {
	return _visible;
}
bool PGUIObject::IsPointInside(const int &X, const int &Y) const {
	return X >= _rectangle.left && X <= _rectangle.right && Y >= _rectangle.top && Y <= _rectangle.bottom;
}