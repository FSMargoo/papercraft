/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file Object.h
 * \brief The game object which compatible the component systems
 */

#pragma once

#include <include/game/component/Component.h>
#include <include/String.h>
#include <unordered_map>

#include <vector>

/**
 * The game object base class, attention, only objects that can have their
 * own position in the game is game objects
 */
class PObject {
public:
	PObject();
	virtual ~PObject() = default;

public:
	template <class Type, class... Parameter>
		requires std::is_base_of_v<PComponent, Type>
	void RegisterComponent(Parameter... parameter) {
		PComponent *component = new Type(parameter...);
		auto		Id		  = component->GetID();
		_list.insert({Id, component});
		_list[Id]->OnPropertyRegistering();
	}

	template <class Type>
	Type *GetComponent(PString Id) {
		return static_cast<Type*>(_list[Id]);
	}

	template <class Type>
	void RegisterProperty(Type *Pointer, PString Id) {
		_propertyMap.insert({Id, reinterpret_cast<void *>(Pointer)});
	}

	template <class Type>
	Type *GetProperty(PString Id) {
		return static_cast<Type *>(_propertyMap[Id]);
	}

public:
	// The bound of the object
	RECT Bound;

public:
	auto begin() {
		return _list.begin();
	}

	auto end() {
		return _list.end();
	}

protected:
	std::unordered_map<PString, PComponent *> _list;
	std::unordered_map<PString, void *> _propertyMap;
};
