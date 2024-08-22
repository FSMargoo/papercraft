/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is herebgranted, free of charge, to any person obtaining a copy
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
 * \file BlockRender.h
 * \brief The block render of the PaperCraft
 */

#pragma once

#include <include/game/block.h>
#include <include/renderer/Camera.h>

#include <thirdparty/PaperRenderer/include/renderer/vSurface.h>

/**
 * The reputable renderer of the blocks
 */
class PBlockReputableRenderer {
public:
	/**
	 * Construct the block-reputable renderer with the block map
	 * @param Map The map of the block
	 */
	explicit PBlockReputableRenderer(PBlockMap &Map);

public:
	/**
	 * Rendering the image of the basic block image in the rendering pipeline
	 * @param Width The width of the basic block image
	 * @param Height The height of the basic block image
	 * @param Camera The camera of the player view
	 * @return The block combination surface
	 */
	sk_sp<VSurface> RenderImage(const int &Width, const int &Height, PCamera *Camera);

private:
	PBlockMap &_map;
};