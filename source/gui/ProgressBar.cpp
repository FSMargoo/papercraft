#include "../../include/gui/ProgressBar.h"

PProgressBar::PProgressBar() : _percentage(0) {
	InitStyle();
}
PProgressBar::PProgressBar(const int &Width, const int &Height) : _percentage(0) {
	Resize(Width, Height);
	InitStyle();
}
void PProgressBar::OnDraw() {
	LINESTYLE lineStyle;
	getlinestyle(&lineStyle);
	setlinecolor(WHITE);

	lineStyle.thickness = 2;

	setlinestyle(&lineStyle);
	setlinecolor(BoundColor);

	rectangle(_rectangle.left, _rectangle.top, _rectangle.right, _rectangle.bottom);

	setfillcolor(FilledColor);

	if (_percentage > 0) {
		auto delta = (_rectangle.right - _rectangle.left - 7) * _percentage / 100.f;
		solidrectangle(_rectangle.left + 3, _rectangle.top + 3, _rectangle.left + 3 + delta, _rectangle.bottom - 4);
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
	BoundColor	= WHITE;
	FilledColor = WHITE;
}