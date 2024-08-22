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
	_backgroundColor	  = SkColorSetRGB(198, 198, 198);
	_borderColor		  = SkColorSetRGB(219, 221, 222);
	_borderUnderlineColor = SkColorSetRGB(110, 112, 115);
	_rectangleBorderColor = SK_ColorBLACK;

	ParagraphStyle.setTextAlign(skia::textlayout::TextAlign::kCenter);
	TextStyle.setColor(SK_ColorBLACK);
	TextStyle.setFontFamilies({SkString("Minecraft")});
	TextStyle.setFontSize(16);

	_fontManager	= SkFontMgr::RefDefault();
	_fontCollection = sk_make_sp<skia::textlayout::FontCollection>();
	_fontCollection->setDefaultFontManager(_fontManager);

	_colorDelta = 0;
}
void PButton::OnDraw(SkCanvas *Canvas) {
	auto backgroundR = GetRValue(_backgroundColor) - _colorDelta;
	auto backgroundG = GetGValue(_backgroundColor) - _colorDelta;
	auto backgroundB = GetBValue(_backgroundColor) - _colorDelta;

	backgroundR = backgroundR < 0 ? 0 : backgroundR;
	backgroundG = backgroundG < 0 ? 0 : backgroundG;
	backgroundB = backgroundB < 0 ? 0 : backgroundB;

	SkPaint borderPaint;
	SkPaint fillPaint;
	borderPaint.setColor(_rectangleBorderColor);
	borderPaint.setStyle(SkPaint::kStroke_Style);
	fillPaint.setColor(SkColorSetRGB(backgroundR, backgroundG, backgroundB));
	fillPaint.setStyle(SkPaint::kFill_Style);

	borderPaint.setStrokeWidth(1);

	Canvas->drawRect(SkRect(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom), fillPaint);
	Canvas->drawRect(SkRect(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom), borderPaint);

	SkPaint linePaint;
	linePaint.setColor(_borderColor);
	Canvas->drawLine(_rectangle.left + 1, _rectangle.top + 1, _rectangle.right - 1, _rectangle.top + 1, linePaint);
	Canvas->drawLine(_rectangle.left + 1, _rectangle.top + 1, _rectangle.left + 1, _rectangle.bottom - 1, linePaint);

	linePaint.setColor(_borderUnderlineColor);
	linePaint.setStrokeWidth(2);

	Canvas->drawLine(_rectangle.left + 2, _rectangle.bottom - 1, _rectangle.right - 1, _rectangle.bottom - 1, linePaint);

	auto builder = skia::textlayout::ParagraphBuilder::make(ParagraphStyle, _fontCollection);
	builder->pushStyle(TextStyle);
	builder->addText(_text.c_str());

	auto paragraph = builder->Build();
	paragraph->layout(GetWidth());
	paragraph->paint(Canvas, _rectangle.left, _rectangle.top + GetHeight() / 2 - paragraph->getHeight() / 2);
}
bool PButton::OnMessage(const PMessage &Message, PGUIMangerInterface *Interface) {
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
	_backgroundColor	  = SkColorSetRGB(3, 137, 69);
	_borderColor		  = SkColorSetRGB(219, 221, 222);
	_borderUnderlineColor = SkColorSetRGB(110, 112, 115);
	_rectangleBorderColor = SK_ColorWHITE;

	TextStyle.setColor(SK_ColorWHITE);
}