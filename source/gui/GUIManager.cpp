﻿/**
* \file GUIManager.cpp
* \brief The GUI manager class definition
*/

#include <include/gui/GUIManager.h>

PGUIManager::PGUIManager() {
	_focus = nullptr;
}
void PGUIManager::AddObject(PGUIObject *Object) {
	if (!Object) {
		throw PGUIManagerInvalidObject("Object should not be nullptr!");
	}

	_child.emplace_back(Object);
}
void PGUIManager::RaiseAsFocus(PGUIObject *Object) {
	_focus = Object;
}
void PGUIManager::UnfocusObject(PGUIObject *Object) {
	_focus = nullptr;
}
PGUIObject *PGUIManager::GetFocusingObject() {
	return _focus;
}
void PGUIManager::RemoveObject(PGUIObject *Object) {
	for (auto child = _child.begin(); child != _child.end(); ++child) {
		if (*child == Object) {
			_child.erase(child);
		}
	}
}
void PGUIManager::OnDraw(SkCanvas *Canvas) {
	for (auto &object : _child) {
		if (object->IsVisible()) {
			object->OnDraw(Canvas);
		}
	}
}
void PGUIManager::OnMessage(PMessage &Message) {
	if (_focus) {
		if (!_focus->OnMessage(Message, this)) {
			for (auto &object : _child) {
				if (!object->IsVisible()) {
					continue;
				}
				if (object != _focus && object->OnMessage(Message, this)) {
					break;
				}
			}
		} else {
			return;
		}
	}
	for (auto &object : _child) {
		if (!object->IsVisible()) {
			continue;
		}
		if (object->OnMessage(Message, this)) {
			break;
		}
	}
}