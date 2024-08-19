/**
* \file Input.cpp
* \brief The input control in paper library
*/

#define GLFW_EXPOSE_NATIVE_WIN32

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
	ParagraphStyle.setTextAlign(skia::textlayout::TextAlign::kLeft);
	TextStyle.setColor(SK_ColorWHITE);
	TextStyle.setFontFamilies({SkString("Minecraft")});
	TextStyle.setFontSize(16);

	_fontManager	= SkFontMgr::RefDefault();
	_fontCollection = sk_make_sp<skia::textlayout::FontCollection>();
	_fontCollection->setDefaultFontManager(_fontManager);
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
void PInput::OnDraw(SkCanvas *Canvas) {
	SkPaint linePaint;
	SkPaint fillPaint;

	fillPaint.setColor(SkColorSetRGB(110, 112, 115));
	linePaint.setColor(SK_ColorBLACK);
	linePaint.setStyle(SkPaint::kStroke_Style);
	fillPaint.setStyle(SkPaint::kFill_Style);

	Canvas->drawRect(SkRect(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom), fillPaint);
	Canvas->drawRect(SkRect(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom), linePaint);

	linePaint.setColor(SkColorSetRGB(110, 112, 115));
	linePaint.setStrokeWidth(1);
	Canvas->drawLine(_rectangle.left + 1, _rectangle.top + 1, _rectangle.right - 1, _rectangle.top + 1, linePaint);
	Canvas->drawLine(_rectangle.left + 1, _rectangle.top + 1, _rectangle.left + 1, _rectangle.bottom - 1, linePaint);

	linePaint.setColor(SkColorSetRGB(110, 112, 115));
	linePaint.setStrokeWidth(2);

	Canvas->drawLine(_rectangle.left + 2, _rectangle.bottom - 1, _rectangle.right - 1, _rectangle.bottom - 1, linePaint);

	auto textBound = _rectangle;
	textBound.left += 8;
	textBound.right -= 8;

	int textWidth  = 0;
	int textHeight = 0;
	if (!_text.is_empty()) {
		TextStyle.setColor(SK_ColorWHITE);

		auto builder = skia::textlayout::ParagraphBuilder::make(ParagraphStyle, _fontCollection);
		builder->pushStyle(TextStyle);
		builder->addText(_text.c_str());

		auto paragraph = builder->Build();
		paragraph->layout(GetWidth());
		paragraph->paint(Canvas, _rectangle.left + 8, _rectangle.top + GetHeight() / 2 - paragraph->getHeight() / 2);

		textWidth  = paragraph->getMaxIntrinsicWidth();
		textHeight = paragraph->getHeight();
	} else if (!_inInput) {
		TextStyle.setColor(SkColorSetRGB(218, 218, 218));

		auto builder = skia::textlayout::ParagraphBuilder::make(ParagraphStyle, _fontCollection);
		builder->pushStyle(TextStyle);
		builder->addText(_placeholder.c_str());

		auto paragraph = builder->Build();
		paragraph->layout(GetWidth());
		paragraph->paint(Canvas, _rectangle.left, _rectangle.top + GetHeight() / 2 - paragraph->getHeight() / 2);
	}

	if (_inInput) {
		if (clock() - _timer > 600) {
			_timer		= clock();
			_showCursor = !_showCursor;
		}

		if (_showCursor) {
			SkPaint cursorPaint;
			cursorPaint.setColor(SK_ColorWHITE);
			cursorPaint.setStrokeWidth(2);

			auto width = _rectangle.left + textWidth + 10;
			Canvas->drawLine(width, _rectangle.top + GetHeight() / 2 - textHeight / 2, width,
				 _rectangle.top + GetHeight() / 2 + textHeight / 2, cursorPaint);
		}
	}
}
bool PInput::OnMessage(const ExMessage &Message, PGUIMangerInterface *Interface) {
	static auto normalCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	static auto IBEAMCursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	switch (Message.message) {
	case WM_MOUSEMOVE: {
		if (IsPointInside(Message.x, Message.y) && !_inHover) {
			_inHover = true;

			glfwSetCursor(Message.window, IBEAMCursor);
		}
		if (!IsPointInside(Message.x, Message.y) && _inHover) {
			_inHover = false;

			glfwSetCursor(Message.window, normalCursor);
		}

		return false;
	}
	case WM_KEYDOWN: {
		if (Message.vkcode == GLFW_KEY_BACKSPACE && _inInput) {
			if (_text.size() > 0) {
				_text = _text.subtext(0, _text.size() - 1);
				OnText(_text);
			}
		}

		break;
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