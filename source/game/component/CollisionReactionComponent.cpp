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
 * \file CollisionReactionComponent.cpp
 * \brief The collision reaction component
 */

#include <include/game/component/CollisionReactionComponent.h>

void PCollisionReactionComponent::CalculateCollision(PComponentObjectInterface *Interface, PComponentObjectInterface *Target) {
	if (Interface->HasProperty("velocity_vector")) {
		ICollisionCalculating(Interface, Target);
	}

	vecmath::Point<float>& Position = *Interface->GetProperty<vecmath::Point<float>>("position");
	vecmath::Point<float>& TargetPosition = *Target->GetProperty<vecmath::Point<float>>("position");

	vecmath::Vector<float> difference = TargetPosition - Position;
	float distance = difference.length();
	float radius = 1.0f; // Example radius
	float penetrationDepth = std::max(0.0f, 2 * radius - distance);

	vecmath::Vector<float>& Normal = *Interface->GetProperty<vecmath::Vector<float>>("normal_vector");
	Position = Position + Normal * penetrationDepth;
	TargetPosition = TargetPosition - Normal * penetrationDepth;
}



void PCollisionReactionComponent::ICollisionCalculating(PComponentObjectInterface *Interface, PComponentObjectInterface *Target) {
	vecmath::Vector<float>& Velocity = *Interface->GetProperty<vecmath::Vector<float>>("velocity_vector");
	vecmath::Vector<float>& TargetVelocity = *Target->GetProperty<vecmath::Vector<float>>("velocity_vector");

	vecmath::Vector<float>& Normal = *Interface->GetProperty<vecmath::Vector<float>>("normal_vector");

	float dotResult = dot(Velocity, Normal);
	Velocity = Velocity - 2 * dotResult * Normal;

	dotResult = dot(TargetVelocity, Normal);
	TargetVelocity = TargetVelocity - 2 * dotResult * Normal;
}