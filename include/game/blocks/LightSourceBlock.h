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
 * \file LightSourceBlock.h
 * \brief The light source block base class
 */

#pragma once

#include <include/game/blocks/Block.h>

/**
 * The shape of light
 */
enum class PLightShapeType {
	Rectangle, Circle
};

/**
 * The unit of a light source
 */
struct PLightUnit {
	float			Brightness;
	SkColor			Color;
	PLightShapeType Shape;
	float			Radius;
	float 			Range;
	float			X;
	float			Y;
};

/**
 * The light source block
 */
class PLightSourceBlock : public PBlock {
public:
	bool IsLightSource() override {
		return true;
	}
	[[nodiscard]] virtual const SkColor& GetLightColor() const {
		return Color;
	}
	[[nodiscard]] virtual float GetLightLevel() const {
		return Level;
	}

public:
	PLightUnit GetUnit() const {
		return { .Brightness = 0.4f, .Color = Color, .Shape = PLightShapeType::Rectangle, .Radius = 20, .Range = 120, .X = static_cast<float>(Bound.left) + 20, .Y = static_cast<float>(Bound.top) + 20 };
	}

private:
	PLightSourceBlock(const PString &Id, const int &X, const int &Y, PImage *Texture, SkColor LColor) : PBlock(Id, X, Y, Texture), Color(LColor), Level(0.8f) {

	}

public:
	static PLightSourceBlock *Clone(PLightSourceBlock *Block, const int &X, const int &Y) {
		return new PLightSourceBlock(Block->_id, X, Y, Block->_texture, Block->Color);
	}

public:
	float	Level;
	SkColor Color;
};