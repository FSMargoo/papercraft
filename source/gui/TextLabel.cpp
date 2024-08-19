/**
* \file TextLabe.cpp
* \brief The poor text label in paper library
*/

#include <include/gui/TextLabel.h>

PTextLabel::PTextLabel(const PString &Text) : _text(Text) {
	InitStyle();
	ResizeAsText();
}
void PTextLabel::InitStyle() {
	ParagraphStyle.setTextAlign(skia::textlayout::TextAlign::kLeft);
	TextStyle.setColor(SK_ColorWHITE);
	TextStyle.setFontFamilies({SkString("Minecraft")});
	TextStyle.setFontSize(14);

	_fontManager	= SkFontMgr::RefDefault();
	_fontCollection = sk_make_sp<skia::textlayout::FontCollection>();
	_fontCollection->setDefaultFontManager(_fontManager);
}
void PTextLabel::ResizeAsText() {
	auto builder = skia::textlayout::ParagraphBuilder::make(ParagraphStyle, _fontCollection);
	builder->pushStyle(TextStyle);
	builder->addText(_text.c_str());

	auto paragraph = builder->Build();
	// Set the value large enough for measure size
	paragraph->layout(1000.f);
	Resize(paragraph->getMaxWidth(), paragraph->getHeight());
}
void PTextLabel::SetText(const PString &Text) {
	_text = Text;
}
PString PTextLabel::GetText() const {
	return _text;
}
void PTextLabel::OnDraw(SkCanvas *Canvas) {
	SkPaint textPaint;

	textPaint.setColor(SK_ColorWHITE);
	textPaint.setStrokeWidth(2);

	auto builder = skia::textlayout::ParagraphBuilder::make(ParagraphStyle, _fontCollection);
	builder->pushStyle(TextStyle);
	builder->addText(_text.c_str());

	auto paragraph = builder->Build();
	paragraph->layout(GetWidth());
	paragraph->paint(Canvas, _rectangle.left, _rectangle.top);
}