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
 * \file CollisionManager.h
 * \brief The collision manager of the game, this manager class will update the status of the velocity info
 */

#pragma once

#include <include/game/object/Object.h>
#include <include/game/component/CollisionComponent.h>
#include <include/game/component/CollisionReactionComponent.h>

/*+
 * The collision manager of the game, this manager will update the velocity status of the
 * object in the game, the user should maintain the lifetime of the PObject pointer
 */
class PCollisionManager : public PComponent {
public:
	PCollisionManager() = default;
	~PCollisionManager() override = default;

public:
	/**
	 * Calculate the collision and update the velocity status of the object
	 */
	void CalculateCollision();

public:
	std::vector<PObject *> ObjectList;
};