/**
 * \file Block.cpp
 * \breif The block structure in the Game
 */

#include <include/game/Blocks/Block.h>

PBlock::PBlock(const PString &Id, const int &X, const int &Y, PImage *Texture, PImage *NormalTexture)
	: _x(X), _y(Y), _id(Id), _texture(Texture), _normalTexture(NormalTexture) {
	Bound = {_x, _y, _x + _texture->GetWidth(), _y + _texture->GetHeight()};
}
PBlockMap::PBlockMap(const BlockMap &Map) : _blockMap(Map) {
}