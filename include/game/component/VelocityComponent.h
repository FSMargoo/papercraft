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
 * \file VelocityComponent.h
 * \brief The velocity component of the game
 */

#pragma once

#include <include/game/component/Component.h>

#include <thirdparty/vecmath/vecmath.hpp>

class PVelocityComponent : public PComponent {
public:
	PVelocityComponent() = default;
	~PVelocityComponent() override;

public:
	PString GetID() const {
		return "velocity";
	}
	void OnPropertyRegistering(PComponentObjectInterface *Interface) override;

private:
	vecmath::Vector<float> *_velocity			  = nullptr;
	vecmath::Vector<float> *_accelerationVelocity = nullptr;

private:
	PComponent *IClone() override {
		auto cloneObject = new PVelocityComponent;

		cloneObject->_velocity			   = new vecmath::Vector<float>;
		cloneObject->_accelerationVelocity = new vecmath::Vector<float>;

		*cloneObject->_velocity				= *_velocity;
		*cloneObject->_accelerationVelocity = *_accelerationVelocity;

		return cloneObject;
	}
};