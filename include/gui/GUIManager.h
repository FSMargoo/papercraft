/**
 * \file GUIManager.h
 * \brief The GUI manager class definition
 */

#pragma once

#include <include/gui/GUIObject.h>

class PGUIManagerInvalidObject : public std::exception {
public:
	PGUIManagerInvalidObject(const PString &Info) {
		_info = ostr::format("PGUIManager failure: {}", Info);
	}

	const char *what() const throw() override {
		return _info.c_str();
	}

private:
	PString _info;
};

/**
 * The GUI object manager, it will send message to every control;
 * It is the executor in paper library GUI
 */
class PGUIManager : public PGUIMangerInterface {
public:
	PGUIManager();
	~PGUIManager() override = default;

public:
	/**
	 * Added GUI object into the manager, if a nullptr object was given
	 * a PGUIManagerInvalidObject exception will be thrown
	 * @param Object The object pointer
	 */
	void AddObject(PGUIObject *Object);
	/**
	 * Remove the GUI object from the manager
	 * @param Object The object pointer
	 */
	void RemoveObject(PGUIObject *Object);

public:
	void		RaiseAsFocus(PGUIObject *Object) override;
	void		UnfocusObject(PGUIObject *Object) override;
	PGUIObject *GetFocusingObject() override;

public:
	/**
	 * Let manager send the message to serveral object
	 * @param Message The message to be sent
	 */
	void OnMessage(PMessage &Message);
	/**
	 * Draw the GUI on the specified device
	 * @param Canvas The canvas to be drawn
	 */
	void OnDraw(SkCanvas *Canvas);

private:
	PGUIObject				 *_focus;
	std::vector<PGUIObject *> _child;
};