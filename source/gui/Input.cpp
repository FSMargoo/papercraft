/**
* \file Input.cpp
* \brief The input control in paper library
*/

#include <include/gui/Input.h>

PInput::PInput(const int &Width, const int &Height) {
	Resize(Width, Height);
	InitProperty();
	InitStyle();
}
PInput::PInput(const int &Width, const int &Height, const PString &Placeholder) {
	SetPlaceHolder(Placeholder);
	Resize(Width, Height);
	InitProperty();
	InitStyle();
}
void PInput::InitStyle() {
	gettextstyle(&FontStyle);
	_tcscpy_s(FontStyle.lfFaceName, _T("Minecraft"));
	FontStyle.lfHeight	= 20;
	FontStyle.lfQuality = PROOF_QUALITY;
}
void PInput::InitProperty() {
	_inInput	= false;
	_inHover	= false;
	_showCursor = false;
	_maxLength	= -1;
}

void PInput::SetPlaceHolder(const PString &Placeholder) {
	_placeholder = Placeholder;
}
PString PInput::GetText() const {
	return _text;
}
void PInput::SetText(const PString &Text) {
	_text = Text;
}
void PInput::SetMaxLength(const int &Length) {
	_maxLength = Length + 1;
}
void PInput::OnDraw() {
	LINESTYLE LineStyle;

	setlinecolor(BLACK);
	setfillcolor(RGB(110, 112, 115));
	LineStyle.style = PS_SOLID | PS_ENDCAP_SQUARE;
	setlinestyle(&LineStyle);
	LineStyle.thickness = 1;
	setlinestyle(&LineStyle);

	fillrectangle(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom);

	setlinecolor(RGB(110, 112, 115));
	line(_rectangle.left + 1, _rectangle.top + 1, _rectangle.right - 1, _rectangle.top + 1);
	line(_rectangle.left + 1, _rectangle.top + 1, _rectangle.left + 1, _rectangle.bottom - 1);

	LineStyle.thickness = 2;
	setlinestyle(&LineStyle);

	line(_rectangle.left + 2, _rectangle.bottom - 1, _rectangle.right - 1, _rectangle.bottom - 1);

	settextstyle(&FontStyle);
	auto textBound = _rectangle;
	textBound.left += 8;
	textBound.right -= 8;
	if (!_text.is_empty()) {
		settextcolor(WHITE);
		drawtext(PStringToCStr<TCHAR>(_text), &textBound, DT_LEFT | DT_VCENTER | DT_WORD_ELLIPSIS | DT_SINGLELINE);
	} else if (!_inInput) {
		settextcolor(RGB(218, 218, 218));
		drawtext(PStringToCStr<TCHAR>(_placeholder), &textBound,
				 DT_LEFT | DT_VCENTER | DT_WORD_ELLIPSIS | DT_SINGLELINE);
	}

	if (_inInput) {
		if (clock() - _timer > 600) {
			_timer		= clock();
			_showCursor = !_showCursor;
		}

		if (_showCursor) {
			getlinestyle(&LineStyle);
			LineStyle.thickness = 2;
			setlinestyle(&LineStyle);
			setlinecolor(WHITE);
			auto width = _rectangle.left + textwidth(PStringToCStr<TCHAR>(_text)) + 10;
			line(width, _rectangle.top + GetHeight() / 2 - FontStyle.lfHeight / 2, width,
				 _rectangle.top + GetHeight() / 2 + FontStyle.lfHeight / 2);
		}
	}
}
bool PInput::OnMessage(const ExMessage &Message, PGUIMangerInterface *Interface) {
	switch (Message.message) {
	case WM_MOUSEMOVE: {
		if (IsPointInside(Message.x, Message.y) && !_inHover) {
			_inHover = true;

			HCURSOR crossStyle = LoadCursor(nullptr, IDC_IBEAM);
			SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LONG>(crossStyle));
		}
		if (!IsPointInside(Message.x, Message.y) && _inHover) {
			_inHover = false;

			HCURSOR crossStyle = LoadCursor(nullptr, IDC_ARROW);
			SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LONG>(crossStyle));
		}

		return false;
	}
	case WM_CHAR: {
		if (_inInput) {
			switch (Message.ch) {
			case '\r':
			case '\n':
				OnFinish(_text);

				_inInput = false;
				Interface->UnfocusObject(this);

				break;

			case '\b':
				if (_text.size() > 0) {
					_text = _text.subtext(0, _text.size() - 1);
					OnText(_text);
				}

				break;

			default:
				if ((_text.size() + 1 < _maxLength) || _maxLength < 0) {
					_text.append(Message.ch);

					OnText(_text);
				}

				break;
			}

			return true;
		}

		return false;
	}
	case WM_LBUTTONDOWN: {
		if (IsPointInside(Message.x, Message.y)) {
			_timer		= clock();
			_inInput	= true;
			_showCursor = true;
			Interface->RaiseAsFocus(this);
		} else if (Interface->GetFocusingObject() == this) {
			_inInput = false;
			Interface->UnfocusObject(this);
			OnFinish(_text);
		}

		return false;
	}

	default: {
		return false;
	}
	}
}