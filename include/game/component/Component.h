/**
 * \file Component.h
 * \brief The component base class in paper library
 */

#pragma once

#include <Windows.h>

class PObject;

/**
 * The abstract class for map interface
 */
class PComponentMapInterface {
public:
	virtual PObject* Get(const int &X, const int &Y);
};

/**
 * The component base class
 */
class PComponent {
public:
	PComponent()		  = default;
	virtual ~PComponent() = default;

public:
	/**
	 * The hitbox component virtual method
	 * @return The hitbox area, if it doesn't a component with hitbox, it will return a
	 * rectangle in { 0, 0, 0, 0 }
	 */
	[[maybe_unused]] virtual RECT HitBox() {
		return {0, 0, 0, 0};
	}

public:
	/**
	 * When a tick passed, this function will be called
	 * @param Object The object of the component
	 * @param Interface The interface of the component map
	 */
	virtual void OnTickUpdated(PObject *Object, PComponentMapInterface *Interface) {

	}
};