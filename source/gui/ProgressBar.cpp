/**
* \file ProgressBar.cpp
* \brief The progress bar control in paper library
*/

#include <include/gui/ProgressBar.h>

PProgressBar::PProgressBar() : _percentage(0) {
	InitStyle();
}
PProgressBar::PProgressBar(const int &Width, const int &Height) : _percentage(0) {
	Resize(Width, Height);
	InitStyle();
}
void PProgressBar::OnDraw(SkCanvas *Canvas) {
	SkPaint borderPaint;

	borderPaint.setColor(BoundColor);
	borderPaint.setStyle(SkPaint::kStroke_Style);
	borderPaint.setStrokeWidth(2);

	Canvas->drawRect(SkRect(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom), borderPaint);

	SkPaint fillPaint;
	borderPaint.setStyle(SkPaint::kFill_Style);
	fillPaint.setColor(FilledColor);

	if (_percentage > 0) {
		auto delta = (_rectangle.right - _rectangle.left - 7) * _percentage / 100.f;
		Canvas->drawRect(SkRect(_rectangle.left + 3, _rectangle.top + 3, _rectangle.left + 3 + delta, _rectangle.bottom - 3), fillPaint);
	}
}
void PProgressBar::SetPercentage(const float &Percentage) {
	_percentage = Percentage;

	if (Percentage < 0)
		_percentage = 0;
	if (Percentage > 100)
		_percentage = 100;

	if (_percentage == 100)
		OnFinished(_percentage);

	OnPercentageChanged(Percentage);
}
int PProgressBar::GetPercentage() const {
	return _percentage;
}
void PProgressBar::InitStyle() {
	BoundColor	= SK_ColorWHITE;
	FilledColor = SK_ColorWHITE;
}