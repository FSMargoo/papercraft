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
 * \file BlockRender.cpp
 * \brief {YOUR BRIEF}
 */

#include <include/renderer/BlockRender.h>

PBlockReputableRenderer::PBlockReputableRenderer(PBlockMap &Map) : _map(Map) {
}
sk_sp<VSurface> &&PBlockReputableRenderer::RenderImage(const int &Width, const int &Height, PCamera *Camera) {
	auto  surface	  = sk_make_sp<VSurface, const int &, const int &>(Width, Height);
	auto &blocks	  = _map.GetBlockMap();
	auto  cameraBound = *Camera;
	auto  canvas	  = surface->GetNativeSurface()->getCanvas();
	for (auto &block : blocks) {
		auto blockBound = block.GetBound();
		if (!(blockBound.left >= cameraBound.right || blockBound.right <= cameraBound.left ||
			  blockBound.top >= cameraBound.bottom || blockBound.bottom <= cameraBound.top)) {
			auto relativeX = block.GetX() - Camera->left;
			auto relativeY = block.GetY() - Camera->top;
			canvas->drawImage(block.GetTexture()->GetNativeImage(), block.GetX(), block.GetY());
		}
	}

	surface->GetNativeSurface()->flush();
	return std::move(surface);
}