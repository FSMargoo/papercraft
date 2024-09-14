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
 * \file CollisionReactionComponent.h
 * \brief The collision reaction component
 */

#pragma once

#include <include/game/component/Component.h>
#include <thirdparty/vecmath/vecmath.hpp>
#include <thirdparty/vecmath/vecmath_impl.hpp>

/**
 * The collision reaction component for paper craft, this class is the
 * base class of the collision reaction component
 */
class PCollisionReactionComponent : public PComponent {
public:
	PCollisionReactionComponent() = default;
	~PCollisionReactionComponent() override = default;

public:
	PString GetID() const override {
		return "collision_reaction";
	}

public:
	/**
	 * Calculate the collision, if the object has the velocity property, it will adjust the
	 * velocity property
	 * @param Interface The interface of the object
	 * @param Target The object collided with
	 */
	void CalculateCollision(PComponentObjectInterface *Interface, PComponentObjectInterface *Target);

private:
	virtual void ICollisionCalculating(PComponentObjectInterface *Interface, PComponentObjectInterface *Target) = 0;
};