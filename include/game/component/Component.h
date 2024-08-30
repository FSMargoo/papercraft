/**
 * \file Component.h
 * \brief The component base class in paper library
 */

#pragma once

#include <include/String.h>

#include <windows.h>
#include <type_traits>

/**
 * The interface for component property registering
 */
class PComponentObjectInterface {
public:
	template <class Type>
	void RegisterProperty(Type *Pointer, PString Id) {
		IRegisterVoidProperty(reinterpret_cast<void *>(Pointer), Id);
	}

protected:
	/**
	 * The interface of registering property
	 * @param Pointer The pointer of the property
	 * @param Id The ID of the property
	 */
	virtual void IRegisterVoidProperty(void *Pointer, const PString &Id) = 0;
};

/**
 * The component base class
 */
class PComponent {
public:
	PComponent() = default;
	virtual ~PComponent() = default;

public:
	virtual PString GetID() const = 0;
	virtual void OnPropertyRegistering(PComponentObjectInterface *Interface) {

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