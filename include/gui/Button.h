///
/// The button control in paper library
///

#pragma once

#include <include/assets/AssetManager.h>
#include <include/mic/MCIManager.h>
#include <include/singleton/Singleton.h>
#include <include/gui/GUIObject.h

/// The inner status of the button
enum class PButtonStatus {
	Normal,
	OnHover,
	OnHandle
};

/// The button control in paper library
class PButton : public PGUIObject {
public:
	/// The button control in paper library
	/// \param Width The width of the button object
	/// \param Height The height of the button object
	/// \param String The text on the button
	PButton(const int &Width, const int &Height, const PString &String);
	virtual ~PButton() override = default;

public:
	LOGFONT FontStyle;

public:
	void OnDraw() override;
	bool OnMessage(const ExMessage &Message, PGUIMangerInterface *Interface) override;
	void Reset() override;

public:
	/// Set the text of the button
	/// \param String The new text of the button
	void SetText(const PString &String);
	/// Get the text of the button
	/// \return The current text of the button
	[[nodiscard]] PString GetText() const;

public:
	/// When the button was clicked, this event will be emitted
	PEvent<> OnClick;
	/// When the button was hovered, this event will be emitted
	PEvent<> OnHover;

protected:
	/// Init the style of the button
	virtual void InitStyle();

protected:
	COLORREF _backgroundColor;
	COLORREF _borderColor;
	COLORREF _borderUnderlineColor;
	COLORREF _textcolor;
	COLORREF _rectangleBorderColor;

private:
	int			  _colorDelta;
	PString		  _text;
	PButtonStatus _status;
};

/// The button with stress style
class PStressButton : public PButton {
public:
	/// The button control in paper library
	/// \param Width The width of the button object
	/// \param Height The height of the button object
	/// \param String The text on the button
	PStressButton(const int &Width, const int &Height, const PString &String);
	virtual ~PStressButton() override = default;

protected:
	void InitStyle() override;
};