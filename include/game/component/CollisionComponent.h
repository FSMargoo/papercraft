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
 * \file CollisionComponent.h
 * \brief The collision component
 */

#pragma once

#include <include/game/component/Component.h>

/**
 * The component of the HitBox
 */
class PCollisionComponent : public PComponent {
public:
	PCollisionComponent() = default;
	~PCollisionComponent() override = default;

public:
	PString GetID() const override {
		return "collision";
	}

public:
	/**
	 * Judge whether two hitbox was overlapped
	 * @return If two hitbox are overlapped, returning true, nor false
	 */
	bool Overlap(RECT& HitBox1, RECT &HitBox2) const;

private:
	PComponent *IClone() override {
		return new PCollisionComponent;
	}
};