///
/// The component base class in paper library
///

#pragma once

#include <Windows.h>

/// The component base class
class PComponent {
public:
	PComponent()		  = default;
	virtual ~PComponent() = default;

public:
	/// The hitbox component virtual method
	/// \return The hitbox area, if it doesn't a component with hitbox, it will return a
	/// rectangle in { 0, 0, 0, 0 }
	[[maybe_unused]] virtual RECT HitBox() {
		return {0, 0, 0, 0};
	}
};