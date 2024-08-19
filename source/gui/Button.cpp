/**
* \file Button.cpp
* \brief The button control in paper library
*/

#include <include/gui/Button.h>

PButton::PButton(const int &Width, const int &Height, const PString &String) {
	Resize(Width, Height);
	_text	= String;
	_status = PButtonStatus::Normal;

	InitStyle();
}
void PButton::InitStyle() {
	_backgroundColor	  = RGB(198, 198, 198);
	_textcolor			  = BLACK;
	_borderColor		  = RGB(219, 221, 222);
	_borderUnderlineColor = RGB(110, 112, 115);
	_rectangleBorderColor = BLACK;

	gettextstyle(&FontStyle);
	_tcscpy_s(FontStyle.lfFaceName, _T("Minecraft"));
	FontStyle.lfHeight	= 20;
	FontStyle.lfQuality = PROOF_QUALITY;

	_colorDelta = 0;
}
void PButton::OnDraw() {
	LINESTYLE LineStyle;

	auto backgroundR = GetRValue(_backgroundColor) - _colorDelta;
	auto backgroundG = GetGValue(_backgroundColor) - _colorDelta;
	auto backgroundB = GetBValue(_backgroundColor) - _colorDelta;

	backgroundR = backgroundR < 0 ? 0 : backgroundR;
	backgroundG = backgroundG < 0 ? 0 : backgroundG;
	backgroundB = backgroundB < 0 ? 0 : backgroundB;

	setlinecolor(_rectangleBorderColor);
	setfillcolor(RGB(backgroundR, backgroundG, backgroundB));
	LineStyle.style = PS_SOLID | PS_ENDCAP_SQUARE;
	setlinestyle(&LineStyle);
	LineStyle.thickness = 1;
	setlinestyle(&LineStyle);

	fillrectangle(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom);

	setlinecolor(_borderColor);
	line(_rectangle.left + 1, _rectangle.top + 1, _rectangle.right - 1, _rectangle.top + 1);
	line(_rectangle.left + 1, _rectangle.top + 1, _rectangle.left + 1, _rectangle.bottom - 1);

	LineStyle.thickness = 2;
	setlinestyle(&LineStyle);
	setlinecolor(_borderUnderlineColor);

	line(_rectangle.left + 2, _rectangle.bottom - 1, _rectangle.right - 1, _rectangle.bottom - 1);

	settextstyle(&FontStyle);
	settextcolor(_textcolor);

	drawtext(PStringToCStr<TCHAR>(_text), &_rectangle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
bool PButton::OnMessage(const ExMessage &Message, PGUIMangerInterface *Interface) {
	switch (Message.message) {
	case WM_MOUSEMOVE: {
		if (IsPointInside(Message.x, Message.y)) {
			if (_status != PButtonStatus::OnHover) {
				_colorDelta = 20;
				_status		= PButtonStatus::OnHover;
				OnHover();
			}

			return true;
		} else {
			if (_status != PButtonStatus::OnHandle) {
				_colorDelta = 0;
				_status		= PButtonStatus::Normal;
			}
		}

		return false;
	}
	case WM_LBUTTONDOWN: {
		if (IsPointInside(Message.x, Message.y)) {
			_status		= PButtonStatus::OnHover;
			_colorDelta = 40;

			Interface->RaiseAsFocus(this);

			return true;
		}

		return false;
	}
	case WM_LBUTTONUP: {
		if (Interface->GetFocusingObject() == this) {
			_status		= PButtonStatus::Normal;
			_colorDelta = 0;

			OnClick();

			auto instance = PGetSingleton<PAssetManager>().GetSoundInstance("click");

			if (instance) {
				instance->Update();
				PMCIManager::Play(instance, nullptr);
			}

			Interface->UnfocusObject(this);
		}
	}
	default: {
		return false;
	}
	}
}
void PButton::Reset() {
	_colorDelta = 0;
}
void PButton::SetText(const PString &String) {
	_text = String;
}
PString PButton::GetText() const {
	return _text;
}
PStressButton::PStressButton(const int &Width, const int &Height, const PString &String)
	: PButton(Width, Height, String) {
	InitStyle();
}
void PStressButton::InitStyle() {
	_backgroundColor	  = RGB(3, 137, 69);
	_textcolor			  = WHITE;
	_borderColor		  = RGB(219, 221, 222);
	_borderUnderlineColor = RGB(110, 112, 115);
	_rectangleBorderColor = BLACK;

	gettextstyle(&FontStyle);
	_tcscpy_s(FontStyle.lfFaceName, _T("Minecraft"));
	FontStyle.lfHeight	= 20;
	FontStyle.lfQuality = PROOF_QUALITY;
}