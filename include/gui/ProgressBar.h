/**
 * \file ProgressBar.h
 * \brief The progress bar control in paper library
 */

#pragma once

#include <include/gui/GUIObject.h>

/**
 * The progres bar control in paper library
 */
class PProgressBar : public PGUIObject {
public:
	/**
	 * Construct the progress bar by default set percentage as 0
	 */
	PProgressBar();
	/**
	 * Construct the progress bar by specified geometry information
	 * @param Width The width of the progress bar
 	 * @param Height The height of the progress bar
 	 */
	PProgressBar(const int &Width, const int &Height);

public:
	void OnDraw() override;

public:
	/**
	 * Set the percentage of the progress bar
	 * @param Percentage The percentage value, in the range of [0, 100]
	 */
	void SetPercentage(const float &Percentage);
	/**
	 * Get the percentage of the progress bar
	 * @return The percentage value
	 */
	int GetPercentage() const;

public:
	COLORREF BoundColor;
	COLORREF FilledColor;

public:
	/**
	 * When percentage changed, this event will be emitted
	 */
	PEvent<int> OnPercentageChanged;
	/**
	 * When percentage equals 100, this event will be emitted
	 */
	PEvent<int> OnFinished;

private:
	/**
	 * Init the style of the progress bar
	 */
	void InitStyle();

private:
	float _percentage;
};
