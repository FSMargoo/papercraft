/**
 * \file GUIObject.h
 * \brief The GUI base object definition
 */

#pragma once

#include <include/EasyXBase.h>
#include <include/event/Event.h>

class PGUIObject;

/**
 * The interface base class for GUI manager
 */
class PGUIMangerInterface {
public:
	PGUIMangerInterface()		   = default;
	virtual ~PGUIMangerInterface() = default;

public:
	/**
	 * Raise up an object as the focusing object in the manager
	 * @param Object The object to be raised
	 */
	virtual void RaiseAsFocus(PGUIObject *Object) = 0;
	/**
	 * Remove the object from the focus object
	 * @param Object The object be removed
	 */
	virtual void UnfocusObject(PGUIObject *Object) = 0;
	/**
	 * Get the focusing object pointer
	 * @return If the manager has not focusing object, it will return nullptr
	 */
	virtual PGUIObject *GetFocusingObject() = 0;
};

/**
 * The basic GUI object in GUI system
 */
class PGUIObject {
public:
	PGUIObject();
	virtual ~PGUIObject() = default;

public:
	/**
	 * When the object was on drawing this function will be called
	 */
	 virtual void OnDraw() = 0;
	/**
	 * When the object was on the message, this function
	 * will be called
	 * @param Message The message on called
	 * @param Interface The interface of manager
	 * @return If the control will take over this message, the return
	 * should be true, nor false
	 */
	virtual bool OnMessage(const ExMessage &Message, PGUIMangerInterface *Interface) {
		return false;
	}

public:
	/**
	 * Set the control in specified size
	 * @param Width The width of the control
	 * @param Height The height of the control
	 */
	void Resize(const int &Width, const int &Height);
	/**
	 * Set the width of the control
	 * @param Width The new width of the control
	 */
	void SetWidth(const int &Width);
	/**
	 * Set the height of the control
	 * @param Height The new width of the control
	 */
	void SetHeight(const int &Height);
	/**
	 * Get the width of the control
	 * @return The width of the control
	 */
	[[nodiscard]] int GetWidth() const;
	/**
	 * Get the height of the control
	 * @return The height of the control
	 */
	[[nodiscard]] int GetHeight() const;
	/**
	 * Move the control to specified position
	 * @param X The X position
	 * @param Y The Y position
	 */
	void Move(const int &X, const int &Y);
	/**
	 * Get the X position of the object
	 * @return The X position of the object
	 */
	[[nodiscard]] int GetX() const;
	/**
	 * Get the Y position of the object
	 * @return The Y position of the object
	 */
	[[nodiscard]] int GetY() const;
	/**
	 * Set the object visible
	 */
	void Show();
	/**
	 * Set the object invisible
	 */
	void Hide();
	/**
	 * Get whther the object is visible
	 * @return If the object is visible, return true, nor false
	 */
	[[nodiscard]] bool IsVisible() const;

public:
	/**
	 * When the object's focus was taken over by other object, the rest function will be called
	 */
	virtual void Reset() {
	}

protected:
	/**
	 * Judge whether a point is inside the object rectangle bound
	 * @param X The X position of the point
	 * @param Y The Y position of the point
	 * @return If the point is inside the rectangle, return true, nor false
	 */
	bool IsPointInside(const int &X, const int &Y) const;

protected:
	bool _visible;
	RECT _rectangle;
};