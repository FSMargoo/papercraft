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
 * \file Renderer.h
 * \brief The renderer of the paper craft
 */

#pragma once

#include "include/game/component/LightSourceComponent.h"
#include <include/renderer/BlendRender.h>
#include <include/renderer/BlockRender.h>
#include <include/renderer/BloomRender.h>
#include <include/renderer/LightRenderer.h>

/**
 * The renderer of the PaperCraft
 */
class PRenderer {
public:
	/**
	 * Construct the renderer by the parameter
	 * @param Width The width of the renderer window
	 * @param Height The height of the renderer window
	 * @param Surface The surface of the OpenGL surface
	 * @param Map The map of the blocks
	 */
	static sk_sp<SkImage> Render(const int &Width, const int &Height, sk_sp<VSurface> &GLSurface, PBlockMap *Map);
};