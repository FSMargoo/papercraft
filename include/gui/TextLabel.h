/**
 * \file TextLabe.h
 * \brief The poor text label in paper library
 */

#pragma once

#include <include/gui/GUIObject.h>

class PTextLabel : public PGUIObject {
public:
	/**
	 * Construct the text label by default style, the text label
	 * will automatically adapt the size of the text
	 * @param Text The text to be set
	 */
	PTextLabel(const PString &Text);

public:
	/**
	 * Resize the text label as the size of the text
	 */
	void ResizeAsText();
	/**
	 * Set the text of the text label
	 * @param Text The new text of the text label
	 */
	void SetText(const PString &Text);
	/**
	 * Get the text of the text label
	 * @return The text of the text label
	 */
	PString GetText() const;

public:
	void OnDraw() override;

public:
	LOGFONT	 FontStyle;
	COLORREF FontColor;

private:
	/**
	 * Init the style of the text label
	 */
	void InitStyle();

private:
	PString _text;
};