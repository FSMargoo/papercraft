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
	gettextstyle(&FontStyle);
	_tcscpy_s(FontStyle.lfFaceName, _T("Minecraft"));
	FontStyle.lfQuality = PROOF_QUALITY;
	FontStyle.lfHeight	= 16;

	FontColor = WHITE;
}
void PTextLabel::ResizeAsText() {
	settextstyle(&FontStyle);

	auto wideFormat = PStringToCStr<TCHAR>(_text);
	Resize(textwidth(wideFormat), textheight(wideFormat));
}
void PTextLabel::SetText(const PString &Text) {
	_text = Text;
}
PString PTextLabel::GetText() const {
	return _text;
}
void PTextLabel::OnDraw() {
	settextstyle(&FontStyle);
	settextcolor(FontColor);

	drawtext(PStringToCStr<TCHAR>(_text), &_rectangle, DT_LEFT | DT_WORDBREAK);
}