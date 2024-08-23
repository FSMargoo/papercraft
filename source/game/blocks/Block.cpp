/**
 * \file Block.cpp
 * \breif The block structure in the Game
 */

#include <include/game/Blocks/Block.h>

RECT PBlockHitboxComponents::HitBox() {
	return hitbox;
}

bool PBlockHitboxComponents::IsOverlap(const RECT &rect) {
	// 判断是否不重叠
	if (rect.right <= hitbox.left || rect.left >= hitbox.right || rect.bottom <= hitbox.top ||
		rect.top >= hitbox.bottom) {
		return false; // 不重叠
	}
	return true; // 重叠
}

PBlock::PBlock(const PString &Id, const int &X, const int &Y, PImage *Texture)
	: _x(X), _y(Y), _id(Id), _texture(Texture) {
	Bound = {_x, _y, _x + _texture->GetWidth(), _y + _texture->GetHeight()};
}
PBlockMap::PBlockMap(const BlockMap &Map) : _blockMap(Map) {
}