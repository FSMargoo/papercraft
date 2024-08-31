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
 * \file CollisionManager.cpp
 * \brief The collision manager of the game, this manager class will update the status of the velocity info
 */

#include <include/game/manager/CollisionManager.h>

void PCollisionManager::CalculateCollision() {
	if (ObjectList.empty()) {
		return;
	}

	for (auto object1 = ObjectList.begin(), object2 = object1 + 1; object1 != ObjectList.end(); ++object1, ++object2) {
		PCollisionReactionComponent *object1Reaction = nullptr;
		PCollisionReactionComponent *object2Reaction = nullptr;
		for (auto &component : *(*object1)) {
			if (component.first == "collision_reaction") {
				object1Reaction = component.second->Cast<PCollisionReactionComponent>();
			}
		}
		for (auto &component : *(*object2)) {
			if (component.first == "collision_reaction") {
				object2Reaction = component.second->Cast<PCollisionReactionComponent>();
			}
		}
		for (auto &component : *(*object1)) {
			if (component.first == "collision") {
				auto collisionComponent = component.second->Cast<PCollisionComponent>();
				if (collisionComponent->Overlap((*object1)->Bound, (*object2)->Bound)) {
					if (object1Reaction != nullptr) {
						object1Reaction->CalculateCollision(*object1, *object2);
					}
					if (object2Reaction != nullptr) {
						object2Reaction->CalculateCollision(*object2, *object1);
					}
				}
			}
		}
	}
}