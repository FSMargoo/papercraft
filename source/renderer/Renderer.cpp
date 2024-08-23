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
 * \file Renderer.cpp
 * \brief The renderer of the paper craft
 */

#include <include/renderer/renderer.h>

sk_sp<SkImage> PRenderer::Render(const int &Width, const int &Height, sk_sp<VSurface> &GLSurface, PBlockMap *Map) {
	PBlockReputableRenderer		  renderer(*Map);
	PNormalBlockReputableRenderer normalRenderer(*Map);
	auto						  camera = std::make_unique<PCamera>(0, 0, Width, Height);

	// Rendering The Block Union
	auto blockImage = renderer.RenderImage(Width, Height, camera.get());

	// Rendering The Normal Block Union
	auto normalBlockImage = normalRenderer.RenderImage(Width, Height, camera.get());

	// Rendering The Lighting
	PLightRenderer::PLightList list;
	auto					   map = Map->GetBlockMap();
	for (auto &object : map) {
		for (auto &component : *object) {
			auto unit = component->Cast<PLightSourceComponent>()->GetUnit(object);
			list.push_back(unit);
		}
	}
	PLightRenderer lightRenderer(list);
	auto		   surface	  = GLSurface->GetNativeSurface()->makeSurface(Width, Height);
	auto		   lightImage = lightRenderer.RenderImage(Width, Height, camera.get(), surface);

	// Rendering The Bloom
	auto bloomRenderer	  = PBloomRenderer();
	auto lightImageShader = lightImage->makeShader(SkSamplingOptions(SkFilterMode::kLinear));
	auto bloomImage		  = bloomRenderer.RenderImage(surface, lightImageShader);

	// Rendering The Blend
	auto blendRenderer	   = PBlendRenderer();
	auto blockShader	   = blockImage->makeShader(SkSamplingOptions(SkFilterMode::kLinear));
	auto normalBlockShader = normalBlockImage->makeShader(SkSamplingOptions(SkFilterMode::kLinear));
	auto result			   = blendRenderer.RenderImage(surface, lightImageShader, blockShader, normalBlockShader);

	return std::move(result);
}