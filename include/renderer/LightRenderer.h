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
 * \file LightRenderer.h
 * \brief The renderer of light
 */

#pragma once

#include <include/game/block.h>
#include <include/renderer/Camera.h>

#include <thirdparty/PaperRenderer/include/renderer/vSurface.h>

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
	float			X;
	float			Y;
};

/**
 * The shader manager of the lighting shader
 */
class PLightShader {

};

/**
 * The reputable renderer of the blocks
 */
class PLightRenderer {
public:
	using PLightList = std::vector<PLightUnit>;

public:
	/**
	 * Construct the block-reputable renderer with the block map
	 * @param Map The map of the block
	 */
	explicit PLightRenderer(PLightList &Map);

public:
	/**
	 * Rendering the image of the lighting image in the rendering pipeline
	 * @param Width The width of the lighting image
	 * @param Height The height of the lighting image
	 * @param Camera The camera of the player view
	 * @return The lighting surface
	 */
	sk_sp<SkImage> RenderImage(const int &Width, const int &Height, PCamera *Camera);

private:
	/**
	 * Compile the SKSL++ shader
	 */
	void CompileShader();

private:
	PLightList &_list;
};