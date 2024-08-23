/**
 * \file Component.h
 * \brief The component base class in paper library
 */

#pragma once

#include <Windows.h>
#include <type_traits>
#include <include/String.h>

class PObject;

/**
 * The component base class
 */
class PComponent {
public:
	PComponent() = default;
	virtual ~PComponent() = default;

public:
	virtual PString GetID() const = 0;
	virtual void OnPropertyRegistering() {

	}
	/**
	 * The hitbox component virtual method
	 * @return The hitbox area, if it doesn't a component with hitbox, it will return a
	 * rectangle in { 0, 0, 0, 0 }
	 */
	virtual RECT HitBox() {
		return {0, 0, 0, 0};
	}

	/**
	 * Whether this component attend the light rendering
	 * @return If the value is false, it dose not attend the light rendering
	 */
	virtual bool IsLuminous() {
		return false;
	}

public:
	/**
	 * Casting this pointer to the specified type
	 * @tparam Type The type to be converted
	 * @return The pointer in specified type
	 */
	template<class Type>
		requires std::is_base_of_v<PComponent, Type>
	Type* Cast() {
		return static_cast<Type*>(this);
	}
};