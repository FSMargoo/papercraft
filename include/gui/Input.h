/**
 * \file Input.h
 * \brief The input control in paper library
 */

#pragma once

#include <include/gui/GUIObject.h>

/**
 * PInput provide a simple text input with only single line
 * editing is supported
 */
class PInput : public PGUIObject {
public:
	/**
	 * Construct the input with specified geometry information
	 * @param Width The width of the input
	 * @param Height The height of the input
	 */
	PInput(const int &Width, const int &Height);
	/**
	 * Construct the input with specified geometry information and
	 * the speciifed placeholder text
	 * @param Width The width of the input
	 * @param Height The height of the input
	 * @param Placeholder The placholder of the input
	 */
	PInput(const int &Width, const int &Height, const PString &Placeholder);

public:
	/**
	 * Get the placeholder text of the input
	 * @return The placeholder text of the input
	 */
	[[nodiscard]] PString GetPlaceHolder() const;
	/**
	 * Set the placeholder text of the input
	 * @param Placeholder The placeholder text of the input
	 */
	void SetPlaceHolder(const PString &Placeholder);
	/**
	 * Get the current context of the input
	 * @return The text of the input
	 */
	[[nodiscard]] PString GetText() const;
	/**
	 * Set the text of the input
	 * @param Text The specified to be set as input text
	 */
	void SetText(const PString &Text);
	/**
	 * Set the max length of the input
	 * @param Length The max length, when it set to -1, disable max length limited
	 */
	void SetMaxLength(const int &Length);

public:
	void OnDraw() override;
	bool OnMessage(const ExMessage &Message, PGUIMangerInterface *Interface) override;

public:
	/**
	 * When the user finished the input task, this event will be emitted
	 */
	PEvent<PString> OnFinish;
	/**
	 * When the new text was inputted, this event will be emitted
	 */
	PEvent<PString> OnText;

public:
	LOGFONT FontStyle;

private:
	/**
	 * Init the style of the input
	 */
	void InitStyle();
	/**
	 * Init the property of the input
	 */
	void InitProperty();

private:
	int		_maxLength;
	bool	_showCursor;
	bool	_inHover;
	bool	_inInput;
	time_t	_timer;
	PString _text;
	PString _placeholder;
};