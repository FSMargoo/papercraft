//
// Created by 范毅伟 on 24-9-12.
//

#pragma once
#include "Entity.h"

class PBehaviorTree {
public:
	PBehaviorTree() = default;

	virtual ~PBehaviorTree() = default;

	virtual void Update(float deltaTime) = 0;

	virtual void HandleMessage(const PMessage &Message) = 0;

	void setEntity(PEntity *entity) {
		_entity = entity;
	}

protected:
	PEntity *_entity = nullptr;

};

