/**
 * \file Component.h
 * \brief The component base class in paper library
 */

#pragma once

#include <include/String.h>

#include <unordered_map>
#include <type_traits>
#include <stacktrace>
#include <windows.h>
#include <typeinfo>

#if _DEBUG
class PComponentDbgPropertyTypeInvalid : public std::exception {
public:
	PComponentDbgPropertyTypeInvalid(const PString &Info) {
		_info = ostr::format("PComponent debug property type invalid: {}", Info);
	}

	const char *what() const throw() override {
		return _info.c_str();
	}

private:
	PString _info;
};
#endif

/**
 * The interface for component property registering
 */
class PComponentObjectInterface {
public:
	template <class Type> void RegisterProperty(Type *Pointer, PString Id) {
#ifdef _DEBUG
		_dbg_hashMapping.insert({ Id, typeid(Type).hash_code() });
		_dbg_typeMapping.insert({ Id, typeid(Type).name() });
#endif

		IRegisterVoidProperty(reinterpret_cast<void *>(Pointer), Id);
	}
	template <class Type> Type *GetProperty(const PString &Id) {
#ifdef _DEBUG
		if (_dbg_hashMapping[Id] != typeid(Type).hash_code()) {
			auto stack = std::stacktrace::current();
			int count = stack.size();
			for (auto &file : stack) {
				printf(std::format("[{}] At file(line : {}) {}:{}\n", count, file.source_line(), file.source_file(), file.description()).c_str());
				--count;
			}
			throw PComponentDbgPropertyTypeInvalid(ostr::format(R"(Type "{}" dose not match "{}" in property "{}")", typeid(Type).name(), _dbg_typeMapping[Id], Id));
		}
#endif

		return static_cast<Type *>(IGetVoidProperty(Id));
	}
	/**
	 * Judge whether there exists a property
	 * @param Id The Id of the property
	 * @return If it has, returning true, nor false
	 */
	virtual bool HasProperty(const PString &Id) = 0;

protected:
	/**
	 * The interface of registering property
	 * @param Pointer The pointer of the property
	 * @param Id The ID of the property
	 */
	virtual void IRegisterVoidProperty(void *Pointer, const PString &Id) = 0;
	/**
	 * Get the property in a void type pointer
	 * @param Id The Id of the property
	 * @return The pointer referred to the property
	 */
	virtual void *IGetVoidProperty(const PString &Id) = 0;

#ifdef _DEBUG
private:
	std::unordered_map<PString, size_t>	 _dbg_hashMapping;
	std::unordered_map<PString, PString> _dbg_typeMapping;
#endif
};

/**
 * The component base class
 */
class PComponent {
public:
	PComponent()		  = default;
	virtual ~PComponent() = default;

public:
	virtual PString GetID() const = 0;
	virtual void	OnPropertyRegistering(PComponentObjectInterface *Interface) {
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
	template <class Type>
		requires std::is_base_of_v<PComponent, Type>
	Type *Cast() {
		return static_cast<Type *>(this);
	}
};