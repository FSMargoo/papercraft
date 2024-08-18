#include "../../include/game/Block.h"

PBlock::PBlock(const PString &Id, const int &X, const int &Y, PImage *Texture)
	: _texture(Texture), _x(X), _y(Y), _id(Id) {
}
