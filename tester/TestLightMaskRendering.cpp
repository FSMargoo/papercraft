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
 * \file TestLightMaskRendering.cpp
 * \brief Test Light Mask Rendering
 */

#include <include/assets/AssetManager.h>
#include <include/renderer/BlendRender.h>
#include <include/renderer/BlockRender.h>
#include <include/renderer/BloomRender.h>
#include <include/game/blocks/LightSourceComponent.h>
#include <include/renderer/LightRenderer.h>
#include <include/singleton/Singleton.h>

#include <easyx.h>

sk_sp<VRenderInterface> GLInterface;

#define WIDTH  800
#define HEIGHT 800

sk_sp<SkTextBlob> TextBlob;
SkPaint			 *Paint;
SkPaint			 *BlurPaint;

PBlockMap* blockMap;

GLFWwindow *GLWindow;

/**
 * Init OpenGL interface object
 */
void InitGLInterface();
/**
 * Create GLFW window
 */
void InitWindow();
/**
 * Init Skia resource
 */
void InitResource();
/**
 * Init the block map
 */
void InitBlockMap();
/**
 * The frame buffer call back function
 */
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height);
/**
 * The error call back function of GLFW
 * @param Error The error code
 * @param Description The description string of the error
 */
void ErrorCallBack(int Error, const char *Description);
/**
 * Call Skia API to draw context
 */
void Draw(int Width, int Height);

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	InitWindow();
	InitResource();
	InitBlockMap();
	InitGLInterface();

	Draw(WIDTH, HEIGHT);
	while (!glfwWindowShouldClose(GLWindow)) {
		glfwPollEvents();
	}

	return 0;
}

void InitBlockMap() {
	auto redstoneDiffuse = new PLightSourceComponent;
	redstoneDiffuse->Level = 0.4;
	redstoneDiffuse->Color = SK_ColorRED;
	auto diamondDiffuse = new PLightSourceComponent;
	diamondDiffuse->Level = 0.4;
	diamondDiffuse->Color = SkColorSetRGB(84, 214, 172);
	auto emeraldDiffuse = new PLightSourceComponent();
	emeraldDiffuse->Level = 0.4;
	emeraldDiffuse->Color = SK_ColorGREEN;

	auto stone			  = PGetSingleton<PAssetManager>().GetBlock("cobbled_deepslate");
	auto redstone		  = PGetSingleton<PAssetManager>().GetBlock("redstone_block");
	auto emerald		  = PGetSingleton<PAssetManager>().GetBlock("emerald_block");
	auto diamond		  = PGetSingleton<PAssetManager>().GetBlock("diamond_block");
	auto stoneNormal	  = PGetSingleton<PAssetManager>().GetBlock("cobbled_deepslate_n");
	auto redstoneNormal	  = PGetSingleton<PAssetManager>().GetBlock("redstone_block_n");
	auto emeraldNormal	  = PGetSingleton<PAssetManager>().GetBlock("emerald_block_n");
	auto diamondNormal	  = PGetSingleton<PAssetManager>().GetBlock("diamond_block_n");
	auto stoneBlock = PBlock::RegisterBlock<PBlock>("papercraft:stone", stone, stoneNormal);
	auto redstoneBlock = PBlock::RegisterBlock<PBlock>("papercraft:redstone_block", redstone, redstoneNormal);
	auto emeraldBlock  = PBlock::RegisterBlock<PBlock>("papercraft:emerald_block", emerald, emeraldNormal);
	auto diamondBlock  = PBlock::RegisterBlock<PBlock>("papercraft:diamond_block", diamond, diamondNormal);
	redstoneBlock->AddComponent(redstoneDiffuse);
	redstoneBlock->AddComponent(redstoneDiffuse);
	diamondBlock->AddComponent(diamondDiffuse);
	emeraldBlock->AddComponent(emeraldDiffuse);
	PBlockMap::BlockMap map;
	IMAGE image;
	loadimage(&image, L"./testMap.png");
	auto buffer = GetImageBuffer(&image);

	for (auto y = 0; y < image.getheight(); ++y) {
		for (auto x = 0; x < image.getwidth(); ++x) {
			auto color = BGR(buffer[x + y * image.getwidth()]);
			auto bufColor = RGB(GetRValue(color), GetGValue(color), GetBValue(color));
			if (bufColor == BLACK) {
				auto block = stoneBlock->Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (bufColor == RGB(32, 32, 255)) {
				auto block = diamondBlock->PBlock::Clone<PBlock>(x * 40, y * 40);
				_flushall();
				map.push_back(block);
			}
			if (bufColor == RGB(96, 255, 96)) {
				auto block = emeraldBlock->PBlock::Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
			if (bufColor == RGB(255, 32, 32)) {
				auto block = redstoneBlock->PBlock::Clone<PBlock>(x * 40, y * 40);
				map.push_back(block);
			}
		}
	}

	blockMap = new PBlockMap(map);
}
void InitGLInterface() {
	GLInterface = sk_make_sp<VRenderInterface>();
}
void InitWindow() {
	glfwSetErrorCallback(ErrorCallBack);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	GLWindow = glfwCreateWindow(WIDTH, HEIGHT, "Skia Tester", nullptr, nullptr);
	if (!GLWindow) {
		printf("Failed to create GLFW window!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetFramebufferSizeCallback(GLWindow, FrameBufferCallBack);

	glfwMakeContextCurrent(GLWindow);
}
void InitResource() {
}
void FrameBufferCallBack(GLFWwindow *Window, int Width, int Height) {
	Draw(Width, Height);
}
void Draw(int Width, int Height) {
	sk_sp<VRenderTarget> glRenderTarget =
		sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = Width, .Height = Height, .X = 0, .Y = 0});
	sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(GLInterface);
	sk_sp<VSurface>		  glSurface =
		sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);

	auto canvas = glSurface->GetNativeSurface()->getCanvas();
	PBlockReputableRenderer renderer(*blockMap);
	PNormalBlockReputableRenderer normalRenderer(*blockMap);
	auto camera = std::make_unique<PCamera>(0, 0, 800, 800);
	auto blockCanvas = renderer.RenderImage(800, 800, camera.get());
	auto normalBlockCanvas = normalRenderer.RenderImage(800, 800, camera.get());

	PLightRenderer::PLightList list;

	auto map = blockMap->GetBlockMap();
	for (auto& object : map) {
		for (auto& component : *object) {
			if (component->IsLuminous()) {
				auto unit = component->Cast<PLightSourceComponent>()->GetUnit(object);
				list.push_back(unit);
			}
		}
	}

	PLightRenderer lightRenderer(list);
	auto surface = glSurface->GetNativeSurface()->makeSurface(800, 800);
	auto lightImage = lightRenderer.RenderImage(800, 800, camera.get(), surface);
	auto bloomRenderer = PBloomRenderer();
	auto lightImageShader = lightImage->makeShader(SkSamplingOptions(SkFilterMode::kLinear));
	auto blockSurface = glSurface->GetNativeSurface()->makeSurface(800, 800);
	auto bloomImage = bloomRenderer.RenderImage(blockSurface, lightImageShader);
	auto blendRenderer = PBlendRenderer();
	auto blockShader = blockCanvas->makeShader(SkSamplingOptions(SkFilterMode::kLinear));
	auto normalBlockShader = normalBlockCanvas->makeShader(SkSamplingOptions(SkFilterMode::kLinear));
	auto resultSurface = glSurface->GetNativeSurface()->makeSurface(800, 800);
	auto result = blendRenderer.RenderImage(resultSurface, lightImageShader, blockShader, normalBlockShader);

	canvas->clear(SK_ColorBLACK);
	canvas->drawImage(result, 0, 0);
	canvas->flush();
	glContext->GetNativeContext()->flushAndSubmit();

	glfwSwapBuffers(GLWindow);
}
void ErrorCallBack(int Error, const char *Description) {
	fputs(Description, stderr);
}