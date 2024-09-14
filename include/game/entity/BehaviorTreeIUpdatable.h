
/**
 *\file BehaviorTreeIUpdatable.h
 * \brief IUpdatable with BehaviorTree
 */


#pragma once

#include <include/game/entity/IUpdatable.h>
#include <include/game/behavior/BehaviorTree.h>

class PBehaviorTreeUpdatable : public PIUpdatable {
public:
	void Update(float deltaTime) override {
		if (_behaviorTree) {
			_behaviorTree->Update(deltaTime);
		}
	}

	void On_Message(const PMessage &Message) override {
		if (_behaviorTree) {
			_behaviorTree->HandleMessage(Message);
		}
	}

	void SetBehaviorTree(BehaviorTree *behaviorTree) {
		_behaviorTree = behaviorTree;
	}

protected:
	PBehaviorTree *_behaviorTree = nullptr;

};